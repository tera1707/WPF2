#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <shlobj.h>
#include <time.h>
#include <thread>   // std::this_thread::get_id()を使うのに必要
#include <fstream>  // std::wofstreamを使うのに必要

void OutputLogToCChokka(std::wstring txt)
{
    //FILE* fp = NULL;
    auto t = time(nullptr);
    auto tmv = tm();
    auto error = localtime_s(&tmv, &t); // ローカル時間(タイムゾーンに合わせた時間)を取得

    WCHAR buf[256] = { 0 };
    wcsftime(buf, 256, L"%Y/%m/%d %H:%M:%S ", &tmv);

    // 現在のスレッドIDを出力
    auto thId = std::this_thread::get_id();

    // ログ出力
    std::wstring logtxt = buf + txt;

    // パス取得バッファ
    TCHAR waFolderPath[MAX_PATH];
    // デスクトップのパスを取得
    SHGetSpecialFolderPath(NULL, waFolderPath, CSIDL_DESKTOP, 0);
    auto logpath = std::wstring( waFolderPath) + L"\\mylog.log";

    // ファイルを開く(なければ作成)
    // C直下のファイルに書くにはexe実行時に管理者権限にする必要アリ
    std::wofstream ofs(logpath, std::ios::app);
    if (!ofs)
    {
        return;
    }
    // 現在時刻とスレッドIDを付けたログをファイルに書き込み
    ofs << thId << L"  " << logtxt.c_str() << std::endl;
    std::wcout << thId << L"  " << logtxt.c_str() << std::endl;
    // ファイル閉じる
    ofs.close();
}

