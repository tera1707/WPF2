#include "framework.h"
#include <powersetting.h>
#include <string>
#include <psapi.h>

// EnumProcess�̓���m�F
void EnumProcessTest()
{
    // EnumProcesses
    // https://docs.microsoft.com/ja-jp/windows/win32/api/psapi/nf-psapi-enumprocesses
    DWORD procId[256] = { 0 };
    DWORD cbNeeded = 0;
    std::wstring buf;

    auto ret = EnumProcesses(procId, sizeof(procId) / sizeof(DWORD), &cbNeeded);
    buf = L"- EnumProcesses() �̖߂�l�F" + std::to_wstring(ret) + L"\r\n";
    OutputDebugString(buf.c_str());

    for (int i = 0; i < cbNeeded; i++)
    {
        // �擾����ID�̃v���Z�X�����J��(�Ǘ��҂ŋN�����Ȃ��Ɛ������Ȃ����ۂ�)
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId[i]);
        auto err = GetLastError();

        if (NULL != hProcess)
        {
            // �v���Z�X�����擾
            HMODULE hMod;
            DWORD cbNeeded;
            TCHAR procName[MAX_PATH] = TEXT("---");

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            {
                GetModuleBaseName(hProcess, hMod, procName, sizeof(procName) / sizeof(TCHAR));
            }
            buf = L"- ProcId�F" + std::to_wstring(i) + L" ID:" + std::to_wstring(procId[i]) + L" Name:" + procName + L"\r\n";
            OutputDebugString(buf.c_str());

            CloseHandle(hProcess);
        }
    }
}
