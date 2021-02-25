#include "framework.h"
#include <powersetting.h>
#include <string>
#include <psapi.h>

// EnumProcessの動作確認
void EnumProcessTest()
{
    // EnumProcesses
    // https://docs.microsoft.com/ja-jp/windows/win32/api/psapi/nf-psapi-enumprocesses
    DWORD procId[256] = { 0 };
    DWORD cbNeeded = 0;
    std::wstring buf;

    auto ret = EnumProcesses(procId, sizeof(procId) / sizeof(DWORD), &cbNeeded);
    buf = L"- EnumProcesses() の戻り値：" + std::to_wstring(ret) + L"\r\n";
    OutputDebugString(buf.c_str());

    for (int i = 0; i < cbNeeded; i++)
    {
        // 取得したIDのプロセス情報を開く(管理者で起動しないと成功しないっぽい)
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId[i]);
        auto err = GetLastError();

        if (NULL != hProcess)
        {
            // プロセス名を取得
            HMODULE hMod;
            DWORD cbNeeded;
            TCHAR procName[MAX_PATH] = TEXT("---");

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            {
                GetModuleBaseName(hProcess, hMod, procName, sizeof(procName) / sizeof(TCHAR));
            }
            buf = L"- ProcId：" + std::to_wstring(i) + L" ID:" + std::to_wstring(procId[i]) + L" Name:" + procName + L"\r\n";
            OutputDebugString(buf.c_str());

            CloseHandle(hProcess);
        }
    }
}
