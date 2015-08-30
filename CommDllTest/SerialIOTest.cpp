// CommDllTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "TestServer.h"
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

TEST_F(SerialIOTest, open_error) {
	ASSERT_FALSE(io->Open(_T("COM255"), _T("baud=115200 parity=N data=8 stop=1")));
	ASSERT_TRUE(io->Close());
}

TEST_F(SerialIOTest, open_1byte_write_read) {
	StartServer(&Echo, _T("COM5"), _T("baud=115200 parity=N data=8 stop=1"));

	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));

	char c = 'a';
	DWORD w;
	ASSERT_EQ(SerialIO::SUCCESS, io->Write(&c, 1, &w, 100));

	int r;
	char * buf;
	ASSERT_EQ(SerialIO::SUCCESS, io->ReadChunk(&buf, &r, 1000));
	ASSERT_EQ(1, r);
	ASSERT_EQ('a', buf[0]);


	ThreadShutdown();
	ASSERT_EQ(SerialIO::SUCCESS, io->Write("Q", 1, &w, 100));
	ASSERT_TRUE(io->Close());

	delete[] buf;
}

TEST_F(SerialIOTest, open_multibyte_write_read) {
	StartServer(&Echo, _T("COM5"), _T("baud=115200 parity=N data=8 stop=1"));

	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));

	char data[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	DWORD w;
	ASSERT_EQ(SerialIO::SUCCESS, io->Write(data, sizeof(data), &w, 100));

	int r;
	int total = 0;
	char stock[26];
	char* buf;
	while (total < 26) {
		ASSERT_EQ(SerialIO::SUCCESS, io->ReadChunk(&buf, &r, 1000));
		::MoveMemory(stock + total, buf, r);
		total += r;
		delete[] buf;
	}

	for (int i = 0; i < r; i++) {
		ASSERT_EQ(data[i], stock[i]);
	}


	ThreadShutdown();
	ASSERT_EQ(SerialIO::SUCCESS, io->Write("Q", 1, &w, 100));
	ASSERT_TRUE(io->Close());
}

TEST_F(SerialIOTest, open_multibyte_wait_read) {

	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));

	StartServer(&Bark3Times, _T("COM5"), _T("baud=115200 parity=N data=8 stop=1"));
	char expect[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	int r;
	int total = 0;
	char stock[26];
	char* buf;
	while (total < 26) {
		ASSERT_EQ(SerialIO::SUCCESS, io->ReadChunk(&buf, &r, 1000));
		::MoveMemory(stock + total, buf, r);
		total += r;
		delete[] buf;
	}

	for (int i = 0; i < r; i++) {
		ASSERT_EQ(expect[i], stock[i]);
	}

	ThreadShutdown();
	ASSERT_TRUE(io->Close());
}

TEST_F(SerialIOTest, open_read_timeout) {

	ASSERT_TRUE(io->Open(_T("COM3"), _T("baud=115200 parity=N data=8 stop=1")));

	int r;
	char* buf;
	ASSERT_EQ(SerialIO::TIME_OUT, io->ReadChunk(&buf, &r, 500));

	ASSERT_TRUE(io->Close());
}
TEST_F(SerialIOTest, read_error) {

	int r;
	char* buf;
	ASSERT_EQ(SerialIO::FAIL, io->ReadChunk(&buf, &r, 500));

	ASSERT_TRUE(io->Close());
}
TEST_F(SerialIOTest, write_error) {

	DWORD r;
	ASSERT_EQ(SerialIO::FAIL, io->Write("q",1, &r, 500));

	ASSERT_TRUE(io->Close());
}



