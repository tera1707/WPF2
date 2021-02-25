#include "framework.h"
#include <string>
#include <powrprof.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <locale.h>
#include <rpc.h>

#pragma comment( lib, "Rpcrt4.lib" )

// GUIDを文字列に変換する
// https://www.wabiapp.com/WabiSampleSource/windows/uuid_to_string.html
std::wstring GuidToString
(
    GUID* pGuid
)
{
    std::wstring oGuidString;
    RPC_WSTR waString;

    // GUIDを文字列へ変換する
    if (RPC_S_OK == ::UuidToString(pGuid, &waString)) {

        // GUIDを結果にセット
        oGuidString = (WCHAR*)waString;

        // GUID文字列の破棄
        RpcStringFree(&waString);
    }

    // GUIDを返す
    return(oGuidString);
}

void PowerEnumerateTest()
{
    ULONG index1 = 0;
    ULONG index2 = 0;
    ULONG index3 = 0;

    while (1)
    {
        UCHAR buf[256] = { 0 };
        DWORD size = sizeof(buf);
        GUID sheme = { 0 };
        GUID subgr = { 0 };
        GUID indv = { 0 };

        // 電源プランのGUIDを取得
        if (PowerEnumerate(NULL, NULL, NULL, ACCESS_SCHEME, index1++, buf, &size))
            break;
        memcpy_s(&sheme, sizeof(sheme), buf, sizeof(sheme));
        OutputDebugString((GuidToString(&sheme) + L"\r\n").c_str());

        index2 = 0;
        while (1)
        {
            // サブグループのGUIDを取得
            if (PowerEnumerate(NULL, &sheme, NULL, ACCESS_SUBGROUP, index2++, buf, &size))
                break;
            memcpy_s(&subgr, sizeof(subgr), buf, sizeof(subgr));
            OutputDebugString((L" " + GuidToString(&subgr) + L"\r\n").c_str());

            index3 = 0;
            while (1)
            {
                // 個別の設定のGUIDを取得
                if (PowerEnumerate(NULL, &sheme, &subgr, ACCESS_INDIVIDUAL_SETTING, index3++, buf, &size))
                    break;
                memcpy_s(&indv, sizeof(indv), buf, sizeof(indv));
                OutputDebugString((L"  " + GuidToString(&indv)).c_str());

                // AC/DCの設定値を取得
                DWORD ac, dc = 0;
                PowerReadACValueIndex(NULL, &sheme, &subgr, &indv, &ac);
                PowerReadDCValueIndex(NULL, &sheme, &subgr, &indv, &dc);

                OutputDebugString((L"  ac:" + std::to_wstring(ac) + L" dc:" + std::to_wstring(dc) + L"\r\n").c_str());
            }
        }
    }
}
