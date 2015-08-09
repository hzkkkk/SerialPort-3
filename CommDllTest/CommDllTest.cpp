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
        CreateTmpFileToRead();
        CreateTmpFileToWrite();

        asyncIO = new AsyncIO();
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
    BYTE data[BUFSIZE];

    static const int WRITEDATASZ = 5;
    BYTE writeData[WRITEDATASZ];

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
    HANDLE hFile = CreateFile(tmpFilePathToRead, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    ASSERT_NE(hFile, INVALID_HANDLE_VALUE);

    BYTE buf[BUFSIZE] = { 0 };
    ASSERT_EQ(BUFSIZE, asyncIO->Read(hFile, buf, BUFSIZE, 1000));
    ASSERT_EQ(1, buf[0]);

    CloseHandle(hFile);
}
TEST_F(AsyncIOTest, read_timeout) {
    CommUnit unit;
    unit.Open(3);
    HANDLE hFile = unit.GetHandle();
    BYTE buf[BUFSIZE] = { 0 };
    ASSERT_EQ(0, asyncIO->Read(hFile, buf, BUFSIZE, 1000));
}

TEST_F(AsyncIOTest, write) {
    HANDLE hFile = CreateFile(tmpFilePathToWrite, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    ASSERT_NE(hFile, INVALID_HANDLE_VALUE);

    //int w = asyncIO->Write(hFile, writeData, WRITEDATASZ, 1000);
    //std::cout << w << std::endl;
    ASSERT_EQ(WRITEDATASZ, asyncIO->Write(hFile, writeData, WRITEDATASZ, 1000));
    CloseHandle(hFile);

    hFile = CreateFile(tmpFilePathToWrite, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ASSERT_NE(hFile, INVALID_HANDLE_VALUE);

    BYTE readbuf[BUFSIZ] = { 0 };
    DWORD dwNumberOfBytesRead;
    ASSERT_TRUE(::ReadFile(hFile, readbuf, WRITEDATASZ, &dwNumberOfBytesRead, NULL));
    ASSERT_EQ(WRITEDATASZ, dwNumberOfBytesRead);
    for (int i = 0; i < WRITEDATASZ; i++) {
        ASSERT_EQ(readbuf[i], writeData[i]);
    }

    CloseHandle(hFile);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    getchar();
    return 0;
}
