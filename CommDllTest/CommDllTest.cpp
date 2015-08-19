// CommDllTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../CommDll/CommDll.h"
#include "../CommDll/AsyncIO.h"

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


TEST_F(CommDllTest, open_success) {
    ASSERT_TRUE(commUnit->Open(3));
    ASSERT_TRUE(commUnit->Close());
}
TEST_F(CommDllTest, open_fail) {
    ASSERT_FALSE(commUnit->Open(1));
    ASSERT_TRUE(commUnit->Close());
}

TEST_F(CommDllTest, send_recv) {
    ASSERT_TRUE(commUnit->Open(3));

    const char* msg = "Hello world\n";
    commUnit->Send(msg, strlen(msg), 1000);

    char buf[BUFSIZ] = { 0 };
    commUnit->Recv(buf, strlen(msg), 1000);
    std::cout << buf;

    const char* msg2 = "Good Night\n";
    commUnit->Send(msg2, strlen(msg2), 1000);

    ASSERT_TRUE(commUnit->Close());
}
