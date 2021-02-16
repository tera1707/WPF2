
// PowerRegisterForEffectivePowerModeNotificationsに必要
#pragma comment(lib, "Powrprof.lib")

#include "framework.h"
#include "PInvokeTestCpp.h"
#include "resource.h"
#include <powersetting.h>
#include <string>

// グローバル変数:
HINSTANCE hInst;

// このコード モジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    hInst = hInstance;

    DialogBox(hInst, L"MyTestDlgBase_Main", NULL, (DLGPROC)MyDlgProc);

    return (int)0;
}


// Powrprof.libをリンカの入力に設定必要
// PowerRegisterForEffectivePowerModeNotifications
// https://docs.microsoft.com/ja-jp/windows/win32/api/powersetting/nf-powersetting-powerregisterforeffectivepowermodenotifications
// EFFECTIVE_POWER_MODE_CALLBACK 
// https://docs.microsoft.com/ja-jp/windows/win32/api/powersetting/nf-powersetting-effective_power_mode_callback

void OnEffectivePowerModeChanged(EFFECTIVE_POWER_MODE mode, void* context)
{
    std::wstring buf;
    buf = L"現在のEFFECTIVE_POWER_MODE：" + std::to_wstring(mode) + L"\r\n";
    OutputDebugString(buf.c_str());
}

// EFFECTIVE_POWER_MODE変化のイベント登録ハンドル
void* RegistrationHandle = NULL;

// ダイアログプロシージャ
BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_INITDIALOG:
    {
        std::wstring buf;
        HANDLE* Context = NULL;
        auto ret = PowerRegisterForEffectivePowerModeNotifications(EFFECTIVE_POWER_MODE_V1, (EFFECTIVE_POWER_MODE_CALLBACK*)OnEffectivePowerModeChanged, Context, &RegistrationHandle);

        buf = L"- PowerRegisterForEffectivePowerModeNotifications() の戻り値：" + std::to_wstring(ret) + L"\r\n";
        OutputDebugString(buf.c_str());
        break;
    }
    case WM_DESTROY:
    {
        if (RegistrationHandle != NULL)
            PowerUnregisterFromEffectivePowerModeNotifications(RegistrationHandle);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDOK:
            EndDialog(hDlg, IDOK);
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        case IDC_BUTTON1:
            MessageBox(NULL, L"", L"", MB_OK);
            break;
        }
        return FALSE;
    }
    return FALSE;
}