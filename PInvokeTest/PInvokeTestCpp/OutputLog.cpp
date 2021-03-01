#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <shlobj.h>
#include <time.h>
#include <thread>   // std::this_thread::get_id()���g���̂ɕK�v
#include <fstream>  // std::wofstream���g���̂ɕK�v

void OutputLogToCChokka(std::wstring txt)
{
    //FILE* fp = NULL;
    auto t = time(nullptr);
    auto tmv = tm();
    auto error = localtime_s(&tmv, &t); // ���[�J������(�^�C���]�[���ɍ��킹������)���擾

    WCHAR buf[256] = { 0 };
    wcsftime(buf, 256, L"%Y/%m/%d %H:%M:%S ", &tmv);

    // ���݂̃X���b�hID���o��
    auto thId = std::this_thread::get_id();

    // ���O�o��
    std::wstring logtxt = buf + txt;

    // �p�X�擾�o�b�t�@
    TCHAR waFolderPath[MAX_PATH];
    // �f�X�N�g�b�v�̃p�X���擾
    SHGetSpecialFolderPath(NULL, waFolderPath, CSIDL_DESKTOP, 0);
    auto logpath = std::wstring( waFolderPath) + L"\\mylog.log";

    // �t�@�C�����J��(�Ȃ���΍쐬)
    // C�����̃t�@�C���ɏ����ɂ�exe���s���ɊǗ��Ҍ����ɂ���K�v�A��
    std::wofstream ofs(logpath, std::ios::app);
    if (!ofs)
    {
        return;
    }
    // ���ݎ����ƃX���b�hID��t�������O���t�@�C���ɏ�������
    ofs << thId << L"  " << logtxt.c_str() << std::endl;
    std::wcout << thId << L"  " << logtxt.c_str() << std::endl;
    // �t�@�C������
    ofs.close();
}

