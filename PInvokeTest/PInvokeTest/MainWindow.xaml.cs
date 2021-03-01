using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
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

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            // EnumProcess
            Int32 arraySize = 500;
            Int32 arrayBytesSize = arraySize * sizeof(UInt32); 
            Int32[] processIds = new Int32[arraySize];
            Int32 bytesCopied;

            PInvoke_EnumProcesses.NativeMethods.EnumProcesses(
                processIds,
                arrayBytesSize,
                out bytesCopied
                );

            Debug.WriteLine("arrayBytesSize : " + arrayBytesSize + "  bytesCopied : " + bytesCopied);

            processIds.ToList()
                .Where((x) => x != 0)
                .Select((x, i) => (x, i))
                .ToList()
                .ForEach((x) =>
            {
                Debug.WriteLine("No." + x.i + "  id : " + x.x);
            });
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            // バランスの電力プランの名前を取る
            var balancePlanGuid = new Guid("381b4222-f694-41f0-9685-ff5bb260df2e");
            var array = balancePlanGuid.ToByteArray();
            IntPtr parray = Marshal.AllocCoTaskMem(array.Length);
            Marshal.Copy(array, 0, parray, array.Length);

            // IntPtrで受ける
            uint buffSize = 256;
            IntPtr ptrName = Marshal.AllocHGlobal((int)buffSize);
            var ret = PInvokeTestModel.NativeMethods.PowerReadFriendlyName(IntPtr.Zero, ref balancePlanGuid, IntPtr.Zero, IntPtr.Zero, ptrName, ref buffSize);
            if (ret == 0)
            {
                var name = Marshal.PtrToStringUni(ptrName);
                Marshal.FreeHGlobal(ptrName);
                Debug.WriteLine(name);
            }

            // byteで受ける
            var nameString = new byte[256];
            ret = PInvokeTestModel.NativeMethods.PowerReadFriendlyName2(IntPtr.Zero, ref balancePlanGuid, IntPtr.Zero, IntPtr.Zero, nameString, ref buffSize);
            Debug.WriteLine(Encoding.Unicode.GetString(nameString));
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            var ret = Guid.Empty;
            uint indvctr = 0;
            uint subctr = 0;

            while (true)
            {
                var subGuid = PInvoke_PowerEnumerationTest.PowerEnumerate(
                    new Guid("381b4222-f694-41f0-9685-ff5bb260df2e"),
                    Guid.Empty,
                    PInvoke_PowerEnumerationTest.PowerDataAccessor.ACCESS_SUBGROUP,
                    subctr++
                    );
                if (subGuid == Guid.Empty)
                {
                    break;
                }

                Debug.WriteLine(subGuid);

                indvctr = 0;
                while (true)
                {
                    var indvGuid = PInvoke_PowerEnumerationTest.PowerEnumerate(
                        new Guid("381b4222-f694-41f0-9685-ff5bb260df2e"),
                        subGuid,
                        PInvoke_PowerEnumerationTest.PowerDataAccessor.ACCESS_INDIVIDUAL_SETTING,
                        indvctr++
                        );

                    if (indvGuid == Guid.Empty)
                    {
                        break;
                    }

                    Debug.WriteLine(" " + indvGuid);

                }
            }


            
        }
    }
}
