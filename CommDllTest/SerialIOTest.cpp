// CommDllTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "EchoComm.h"
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

TEST_F(SerialIOTest, open_close) {
	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));
	ASSERT_TRUE(io->Close());
}

TEST_F(SerialIOTest, open_only) {
	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));
}
TEST_F(SerialIOTest, close_only) {
	ASSERT_TRUE(io->Close());
}

TEST_F(SerialIOTest, open_close_open_close) {
	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));
	ASSERT_TRUE(io->Close());
	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));
	ASSERT_TRUE(io->Close());
}
TEST_F(SerialIOTest, open_close_diff_open_close) {
	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));
	ASSERT_TRUE(io->Close());
	ASSERT_TRUE(io->Open(_T("COM5"), _T("baud=115200 parity=N data=8 stop=1")));
	ASSERT_TRUE(io->Close());
}

TEST_F(SerialIOTest, open_1byte_read) {
	StartBarkServer(_T("COM5"), _T("baud=115200 parity=N data=8 stop=1"));
	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));
	char * buf;
	int readlen;
	ASSERT_EQ(SerialIO::SUCCESS, io->ReadChunk(&buf, &readlen, 2000));
	ASSERT_EQ('a', buf[0]);
	delete[] buf;
	ThreadShutdown();
}

TEST_F(SerialIOTest, open_multibyte_read) {
	StartMoreBarkServer(_T("COM5"), _T("baud=115200 parity=N data=8 stop=1"));
	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));
	char * buf;
	int readlen;
	ASSERT_EQ(SerialIO::SUCCESS, io->ReadChunk(&buf, &readlen, 3000));
	char expect[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	for (int i = 0; i < readlen; i++) {
		ASSERT_EQ(buf[i], expect[i]);
	}
	delete[] buf;
	ThreadShutdown();
}
TEST_F(SerialIOTest, substitute_port) {
	StartBarkServer(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1"));
	ASSERT_TRUE(io->Open(_T("COM5"), _T("baud=115200 parity=N data=8 stop=1")));
	char * buf;
	int readlen;
	ASSERT_EQ(SerialIO::SUCCESS, io->ReadChunk(&buf, &readlen, 3000));
	ASSERT_EQ('a', buf[0]);
	delete[] buf;
	ThreadShutdown();
}

TEST_F(SerialIOTest, open_readtimeout) {
	StartEchoServer(_T("COM5"), _T("baud=115200 parity=N data=8 stop=1"));
	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));
	char * buf;
	int readlen;

	ASSERT_EQ(SerialIO::TIME_OUT, io->ReadChunk(&buf, &readlen, 2000));

	char c = 'q';
	DWORD written;
	ASSERT_EQ(SerialIO::SUCCESS, io->Write(&c, 1, &written, 1000));
}


