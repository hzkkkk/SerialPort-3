// CommDllTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
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

TEST_F(CommDllTest, open_success) {
	ASSERT_TRUE(commUnit->Open(3));
	ASSERT_TRUE(commUnit->Close());
}
TEST_F(CommDllTest, open_fail) {
	ASSERT_FALSE(commUnit->Open(1));
	ASSERT_TRUE(commUnit->Close());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	getchar();
	return 0;
}
