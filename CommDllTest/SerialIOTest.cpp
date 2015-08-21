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
};

TEST_F(SerialIOTest, open_1byte_read) {
    StartBarkServer(_T("COM4"), _T("baud=115200 parity=N data=8 stop=1"));
    ASSERT_TRUE(io->Open(_T("COM3"),_T("baud=115200 parity=N data=8 stop=1")));
    char * buf;
    int readlen;
    ASSERT_TRUE(io->ReadByEvent(&buf, &readlen, 2000));
    ASSERT_EQ('a', buf[0]);
    delete[] buf;
    ThreadShutdown();
}
TEST_F(SerialIOTest, substitute_port) {
    StartBarkServer(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1"));
    ASSERT_TRUE(io->Open(_T("COM4"),_T("baud=115200 parity=N data=8 stop=1")));
    char * buf;
    int readlen;
    ASSERT_TRUE(io->ReadByEvent(&buf, &readlen, 3000));
    ASSERT_EQ('a', buf[0]);
    delete[] buf;
    ThreadShutdown();
}


