using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PInvokeTest
{
    public static class PInvoke_PowerEnumerationTest
    {
        public enum PowerDataAccessor : uint
        {
            ACCESS_AC_POWER_SETTING_INDEX = 0,
            ACCESS_DC_POWER_SETTING_INDEX = 1,
            ACCESS_SCHEME = 16,
            ACCESS_SUBGROUP = 17,
            ACCESS_INDIVIDUAL_SETTING = 18,
            ACCESS_ACTIVE_SCHEME = 19,
            ACCESS_CREATE_SCHEME = 20
        }

        public static Guid PowerEnumerate(Guid scheme, Guid sub, PowerDataAccessor flag, uint index)
        {
            var buf = Guid.Empty;
            var size = (uint)Marshal.SizeOf(buf);
            NativeMethods.PowerEnumerate(IntPtr.Zero, ref scheme, ref sub, flag, index, ref buf, ref size);
            return buf;
        }


        internal class NativeMethods
        {
            [DllImport("powrprof.dll", SetLastError = true)]
            internal static extern uint PowerEnumerate(IntPtr RootPowerKey,
                     ref Guid scheme,
                     ref Guid subgroup,
                     PowerDataAccessor AccessFlags,
                     uint Index,
                     ref Guid Buffer,
                     ref uint BufferSize);
        }

    }
}
