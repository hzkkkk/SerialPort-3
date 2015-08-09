// CommDllTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../CommDll/CommDll.h"

class CommDllTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        commUnit = new CommUnit();
    }
    virtual void TearDown() {
        delete commUnit;
    }

    CommUnit *commUnit;
};

class AsyncIOTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        ASSERT_EQ(0, _ttmpnam_s(tempFilePath, MAX_PATH));
        HANDLE hFile = CreateFile(tempFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        ASSERT_NE(hFile, INVALID_HANDLE_VALUE);


        for (int i = 0; i < sizeof(data); i++) {
            data[i] = i + 1;
        }

        DWORD dwWritten;
        WriteFile(hFile, data, sizeof(data), &dwWritten, NULL);
        ASSERT_EQ(sizeof(data), dwWritten);
        CloseHandle(hFile);

        asyncIO = new AsyncIO();
    }
    virtual void TearDown() {
        ASSERT_TRUE(DeleteFile(tempFilePath));
    }
    AsyncIO *asyncIO;
    TCHAR tempFilePath[MAX_PATH];
    static const int BUFSIZE = 8192;
    BYTE data[BUFSIZE];
};

TEST_F(CommDllTest, open_success) {
    ASSERT_TRUE(commUnit->Open(3));
    ASSERT_TRUE(commUnit->Close());
}
TEST_F(CommDllTest, open_fail) {
    ASSERT_FALSE(commUnit->Open(1));
    ASSERT_TRUE(commUnit->Close());
}

TEST_F(CommDllTest, getLastError) {
    ASSERT_FALSE(commUnit->Open(1));
    ASSERT_NE(commUnit->GetLastError(), 0);
    ASSERT_TRUE(commUnit->Close());
}

TEST_F(AsyncIOTest, read) {
    HANDLE hFile = CreateFile(tempFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    ASSERT_NE(hFile, INVALID_HANDLE_VALUE);

    BYTE buf[BUFSIZE] = { 0 };
    ASSERT_EQ(BUFSIZE, asyncIO->Read(hFile, buf, BUFSIZE, 1000));
    ASSERT_EQ(1, buf[0]);

    CloseHandle(hFile);
}
//TEST_F(AsyncIOTest, read_timeout) {
//    HANDLE hFile = CreateFile(_T("COM3"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
//    ASSERT_NE(hFile, INVALID_HANDLE_VALUE);
//
//    BYTE buf[BUFSIZE] = { 0 };
//    ASSERT_EQ(BUFSIZE, asyncIO->Read(hFile, buf, BUFSIZE, 1000));
//    ASSERT_EQ(1, buf[0]);
//
//    CloseHandle(hFile);
//}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    getchar();
    return 0;
}
