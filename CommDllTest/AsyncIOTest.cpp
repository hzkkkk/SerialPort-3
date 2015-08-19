// CommDllTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../CommDll/CommDll.h"
#include "../CommDll/AsyncIO.h"

class AsyncIOTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        CreateTmpFileToRead();
        CreateTmpFileToWrite();

        asyncIO = new AsyncIO();
        asyncIO->Init();
    }
    virtual void TearDown() {
        ASSERT_TRUE(DeleteFile(tmpFilePathToRead));
        ASSERT_TRUE(DeleteFile(tmpFilePathToWrite));

        delete asyncIO;
    }

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


TEST_F(AsyncIOTest, read) {
    HANDLE hFile = CreateFile(tmpFilePathToRead, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    ASSERT_NE(hFile, INVALID_HANDLE_VALUE);

    char buf[BUFSIZE] = { 0 };
    ASSERT_EQ(BUFSIZE, asyncIO->Read(hFile, buf, BUFSIZE, 1000));
    ASSERT_EQ(1, buf[0]);

    CloseHandle(hFile);
}
TEST_F(AsyncIOTest, read_timeout) {
    CommUnit unit;
    unit.Open(3);
    HANDLE hFile = unit.GetHandle();
    char buf[BUFSIZE] = { 0 };
    ASSERT_EQ(0, asyncIO->Read(hFile, buf, BUFSIZE, 1000));
}

TEST_F(AsyncIOTest, write) {
    HANDLE hFile = CreateFile(tmpFilePathToWrite, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    ASSERT_NE(hFile, INVALID_HANDLE_VALUE);

    ASSERT_EQ(WRITEDATASZ, asyncIO->Write(hFile, writeData, WRITEDATASZ, 1000));
    CloseHandle(hFile);

    hFile = CreateFile(tmpFilePathToWrite, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ASSERT_NE(hFile, INVALID_HANDLE_VALUE);

    char readbuf[BUFSIZ] = { 0 };
    DWORD dwNumberOfBytesRead;
    ASSERT_TRUE(::ReadFile(hFile, readbuf, WRITEDATASZ, &dwNumberOfBytesRead, NULL));
    ASSERT_EQ(WRITEDATASZ, dwNumberOfBytesRead);
    for (int i = 0; i < WRITEDATASZ; i++) {
        ASSERT_EQ(readbuf[i], writeData[i]);
    }

    CloseHandle(hFile);
}

