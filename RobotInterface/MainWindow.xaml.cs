using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;


namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;

        Robot robot = new Robot();
        byte[] byteList;

        public MainWindow()
        {
            InitializeComponent();

            serialPort1 = new ReliableSerialPort("COM5", 115200, System.IO.Ports.Parity.None, 8, System.IO.Ports.StopBits.One);
            serialPort1.OnDataReceivedEvent += SerialPort1_OnDataReceivedEvent;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick; 
            timerAffichage.Start();

            byteList = new byte[20];


        }



        private void TimerAffichage_Tick(object sender, EventArgs e)
        {  
            while(robot.byteListReceived.Count>0)
            {
                DecodeMessage(robot.byteListReceived.Dequeue());
                //textBoxReception.Text += robot.byteListReceived.Dequeue().ToString();
            }
        }


        private void SerialPort1_OnDataReceivedEvent(object sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);

            for (int i = 0; i < e.Data.Length; i++)
            {
                byte b = e.Data[i];
                robot.byteListReceived.Enqueue(b);
            }
        }

        

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (buttonEnvoyer.Background == Brushes.RoyalBlue)
            {
                buttonEnvoyer.Background = Brushes.Red;
            }
            else buttonEnvoyer.Background = Brushes.RoyalBlue;

            SendMessage();

        }

        private void SendMessage()
        {
            //string objTextBox = textBoxEmission.Text;
            //textBoxReception.Text += objTextBox + "\n";
            serialPort1.WriteLine(textBoxEmission.Text);
            textBoxEmission.Text = " ";
            //textBoxReception.Text += receivedText + "\n";


        }

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
               
            }
        }

        private void Test_Click(object sender, RoutedEventArgs e)
        {

            byte[] payload = Encoding.ASCII.GetBytes("Bonjour");
            int function = 0x0080;
            UartEncodeAndSendMessage(function, payload.Length, payload);
            ProcessDecodedMessage(function, payload.Length, payload);




        }

        byte CalculateChecksum(int msgFunction,int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);
            for(int i=0; i<msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }

            //textBoxReception.Text = checksum.ToString("X");
            return checksum;
        }


        void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, byte[] msgPayload)
        {
            var message = new byte[msgPayloadLength + 6];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (byte)(msgFunction >> 8);
            message[pos++] = (byte)(msgFunction >> 0);
            message[pos++] = (byte)(msgPayloadLength >> 8);
            message[pos++] = (byte)(msgPayloadLength >> 0);

            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] = msgPayload[i];
            }

            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength,msgPayload);

            serialPort1.Write(message, 0, message.Length);
        }

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }
        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                   if(c == 0xFE)
                    {
                        rcvState = StateReception.FunctionMSB;
                    }
                   break;
                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;
                case StateReception.FunctionLSB:
                    msgDecodedFunction += c << 0;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;
                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;
                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c << 0;                    
                    if (msgDecodedPayloadLength > 0)
                    {
                        rcvState = StateReception.Payload;
                        msgDecodedPayloadIndex = 0;
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                    }
                    else
                    {
                        rcvState = StateReception.CheckSum;
                    }
                    break;
                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex] = c;
                    msgDecodedPayloadIndex++;
                    if(msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                        rcvState = StateReception.CheckSum;
                    break;
                case StateReception.CheckSum:
                    Byte calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    Byte receivedChecksum = c;
                    if (calculatedChecksum == receivedChecksum)
                      {
                        //Success, on a un message valide
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                      }
                    rcvState = StateReception.Waiting;
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        public enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,
            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,
            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,
            STATE_ARRET = 12,
            STATE_ARRET_EN_COURS = 13,
            STATE_RECULE = 14,
            STATE_RECULE_EN_COURS = 15
        }


        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch (msgFunction)
            {
                case (0x0080):
                    //for (int i = 0; i < msgPayloadLength; i++)
                    //{
                    //    textBoxEmission.Text += msgPayload[i].ToString("X");
                    //}
                    break;
                case (0x0020):
                    textBoxEmission.Text += msgPayload[0].ToString("X");
                    textBoxEmission.Text += msgPayload[1].ToString("X");
                    break;
                case (0x0030):
                    labelIrGauche.Content = msgPayload[0].ToString("X");
                    //textBoxReception.Text += msgPayload[1].ToString("X");
                    //textBoxReception.Text += msgPayload[2].ToString("X");
                    break;
                case (0x0040):
                    textBoxEmission.Text += msgPayload[0].ToString("X");
                    textBoxEmission.Text += msgPayload[1].ToString("X");
                    break;
                /*case MsgFunction.RobotState:
                    int instant = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16) + (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);
                    textBoxReception.Text += "\nRobot␣State␣:␣" +((StateRobot)(msgPayload[0])).ToString() +
                    "␣-␣" + instant.ToString() + "␣ms";
                    break;*/
            }
                





        }



    }
}
