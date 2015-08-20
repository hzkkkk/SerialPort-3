// CommDllTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "EchoComm.h"
#include "../CommDll/CommDll.h"
#include "../CommDll/SerialIO.h"

class SerialIOTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        io = new SerialIO();
    }
    virtual void TearDown() {
        delete io;
    }
    SerialIO* io;

    AsyncIO *asyncIO;

    TCHAR tmpFilePathToRead[MAX_PATH];
    TCHAR tmpFilePathToWrite[MAX_PATH];

    static const int BUFSIZE = 8192;
    char data[BUFSIZE];

    static const int WRITEDATASZ = 5;
    char writeData[WRITEDATASZ];

private:
    void CreateTmpFileToRead() {
        ASSERT_EQ(0, _ttmpnam_s(tmpFilePathToRead, MAX_PATH));
        HANDLE hFile = CreateFile(tmpFilePathToRead, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        ASSERT_NE(hFile, INVALID_HANDLE_VALUE);


        for (int i = 0; i < sizeof(data); i++) {
            data[i] = i + 1;
        }

        DWORD dwWritten;
        WriteFile(hFile, data, sizeof(data), &dwWritten, NULL);
        ASSERT_EQ(sizeof(data), dwWritten);
        CloseHandle(hFile);
    }
    void CreateTmpFileToWrite() {
        ASSERT_EQ(0, _ttmpnam_s(tmpFilePathToWrite, MAX_PATH));
        HANDLE hFile = CreateFile(tmpFilePathToWrite, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        ASSERT_NE(hFile, INVALID_HANDLE_VALUE);
        CloseHandle(hFile);

        for (int i = 0; i < sizeof(WRITEDATASZ); i++) {
            writeData[i] = i + 1;
        }
    }
};

TEST_F(SerialIOTest, read) {
    StartBarkServer(_T("COM4"), _T("baud=115200 parity=N data=8 stop=1"));
    ASSERT_TRUE(io->Open(3));
    char * buf;
    int readlen;
    ASSERT_TRUE(io->ReadByEvent(&buf, &readlen, 2000));
    ASSERT_EQ('a', buf[0]);
    delete[] buf;
}

//TEST_F(SerialIOTest, read) {
//    ASSERT_TRUE(io->Open(3));
//    char * buf;
//    int readlen;
//    while (true) {
//        io->ReadByEvent(&buf, &readlen, 5000);
//        if (readlen == 0) {
//            std::cout << "timeout" << std::endl;
//            continue;
//        }
//        std::cout << readlen << ":";
//        for (int i = 0; i < readlen; i++) {
//            std::cout << buf[i];
//
//        }
//        std::cout << std::endl;
//        delete[] buf;
//    }
//}

