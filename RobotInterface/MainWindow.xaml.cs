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


namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
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
            string objTextBox = textBoxEmission.Text;
            textBoxEmission.Text = " ";
            textBoxReception.Text += objTextBox + "\n";
            
        }

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }

        ReliableSerialPort serialPort1;
        serialPort1 = new ReliableSerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
        serialPort1.Open();





    }
}
