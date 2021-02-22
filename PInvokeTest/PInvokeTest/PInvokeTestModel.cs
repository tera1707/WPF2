using System;
using System.Runtime.InteropServices;

namespace PInvokeTest
{
    public static class PInvokeTestModel
    {
        // (powersetting.hより)
        // https://docs.microsoft.com/ja-jp/windows/win32/api/powersetting/ne-powersetting-effective_power_mode
        public enum EFFECTIVE_POWER_MODE
        {
            EffectivePowerModeBatterySaver,
            EffectivePowerModeBetterBattery,
            EffectivePowerModeBalanced,
            EffectivePowerModeHighPerformance,
            EffectivePowerModeMaxPerformance,   // v1 last supported (Windows 10, version 1809-)
            EffectivePowerModeGameMode,
            EffectivePowerModeMixedReality,     // v2 last supported (Windows 10, version 1903-)
        }

        // モードが変わったときのハンドラ
        private static Action<EFFECTIVE_POWER_MODE> OnEffectivePowerModeChanged = null;

        // EffectivePowerModeの変化イベントの登録ハンドラ(登録解除時に使う)
        private static IntPtr RegistrationHandle = Marshal.AllocHGlobal((int)sizeof(int));

        // 開始時に呼ぶメソッド(イベント登録＆使う人にハンドラを登録してもらう)
        public static void PowerRegisterForEffectivePowerModeNotifications(Action<EFFECTIVE_POWER_MODE> action)
        {
            OnEffectivePowerModeChanged = action;

            NativeMethods.PowerRegisterForEffectivePowerModeNotifications(
                NativeMethods.EFFECTIVE_POWER_MODE_V1,
                (mode, context) =>
                {
                    OnEffectivePowerModeChanged?.Invoke(mode);
                },
                IntPtr.Zero,
                ref RegistrationHandle
            );
        }

        // 終了時に呼ぶメソッド(登録解除)
        public static void PowerUnregisterFromEffectivePowerModeNotifications()
        {
            NativeMethods.PowerUnregisterFromEffectivePowerModeNotifications(RegistrationHandle);
        }

        // pinvoke用
        public class NativeMethods
        {
            // (powersetting.hより)
            // https://docs.microsoft.com/ja-jp/windows/win32/api/powersetting/nf-powersetting-effective_power_mode_callback
            public delegate void EFFECTIVE_POWER_MODE_CALLBACK(EFFECTIVE_POWER_MODE mode, IntPtr context);

            // (powersetting.hより)
            // https://docs.microsoft.com/en-us/windows/win32/api/powersetting/nf-powersetting-powerregisterforeffectivepowermodenotifications#parameters
            public const int EFFECTIVE_POWER_MODE_V1 = 0x00000001;
            public const int EFFECTIVE_POWER_MODE_V2 = 0x00000002;

            // パワースライダーの変更イベントの登録
            // https://docs.microsoft.com/ja-jp/windows/win32/api/powersetting/nf-powersetting-powerregisterforeffectivepowermodenotifications
            [DllImport("powrprof.dll")]
            internal static extern uint PowerRegisterForEffectivePowerModeNotifications(
                uint Version,
                EFFECTIVE_POWER_MODE_CALLBACK Callback,
                IntPtr Context,
                ref IntPtr RegistrationHandle
            );

            // パワースライダーの変更イベントの解除
            // https://docs.microsoft.com/ja-jp/windows/win32/api/powersetting/nf-powersetting-powerunregisterfromeffectivepowermodenotifications
            [DllImport("powrprof.dll")]
            internal static extern uint PowerUnregisterFromEffectivePowerModeNotifications(
                IntPtr RegistrationHandle
            );

            [DllImport("powrprof.dll", CharSet = CharSet.Unicode)]
            public static extern uint PowerReadFriendlyName(
               IntPtr RootPowerKey,
               ref Guid SchemeGuid,
               IntPtr SubGroupOfPowerSettingsGuid,
               IntPtr PowerSettingGuid,
               IntPtr Buffer,
               ref uint BufferSize
           );

            [DllImport("powrprof.dll", EntryPoint = "PowerReadFriendlyName", CharSet = CharSet.Ansi)]
            public static extern uint PowerReadFriendlyName2(
               IntPtr RootPowerKey,
               ref Guid SchemeGuid,
               IntPtr SubGroupOfPowerSettingsGuid,
               IntPtr PowerSettingGuid,
               byte[] Buffer,
               ref uint BufferSize
           );
        }
    }
}
