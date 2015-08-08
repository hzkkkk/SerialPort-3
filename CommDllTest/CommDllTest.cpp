// CommDllTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "../CommDll/CommDll.h"

TEST(Open, success) {
	CommUnit *commUnit = new CommUnit();
	ASSERT_TRUE(commUnit->Open(3));
	delete commUnit;
}
TEST(Open, fail) {
	CommUnit *unit = new CommUnit();
	ASSERT_FALSE(unit->Open(1));
	delete unit;
	unit = NULL;
}
TEST(Close, success) {
	CommUnit *unit = new CommUnit();
	ASSERT_TRUE(unit->Open(3));
	ASSERT_TRUE(unit->Close());
	delete unit;
	unit = NULL;
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	getchar();
	return 0;
}

