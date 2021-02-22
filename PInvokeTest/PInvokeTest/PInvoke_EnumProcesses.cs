using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PInvokeTest
{
    class PInvoke_EnumProcesses
    {

        public class NativeMethods
        {
            // https://docs.microsoft.com/ja-jp/windows/win32/api/psapi/nf-psapi-enumprocesses
            // https://www.pinvoke.net/default.aspx/psapi.EnumProcesses
            [DllImport("psapi.dll")]
            internal static extern uint EnumProcesses(
                   [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.U4)][In][Out] Int32[] processIds,
                   Int32 arraySizeBytes,
                   [MarshalAs(UnmanagedType.I4)] out Int32 bytesCopied
                );


        }

    }
}
