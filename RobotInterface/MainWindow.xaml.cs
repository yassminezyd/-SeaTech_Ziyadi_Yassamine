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
                textBoxReception.Text += robot.byteListReceived.Dequeue().ToString();
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
            //textBoxEmission.Text = " ";
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
            for (int i = 0; i < byteList.Length; i++)
            {
                byteList[i] = (byte)(2 * i);
            }

            serialPort1.Write(byteList, 0, byteList.Length);
        }
    }
}
