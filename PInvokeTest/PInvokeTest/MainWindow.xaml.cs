using System;
using System.Collections.Generic;
using System.Diagnostics;
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

namespace PInvokeTest
{
    public partial class MainWindow : Window
    {
        public MainWindow() => InitializeComponent();

        private void Window_Loaded(object sender, RoutedEventArgs e) { }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            PInvokeTestModel.PowerRegisterForEffectivePowerModeNotifications((newMode) =>
            {
                Debug.WriteLine("モード：" + newMode);
            });
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            // 登録解除の実験
            PInvokeTestModel.PowerUnregisterFromEffectivePowerModeNotifications();
        }
    }
}
