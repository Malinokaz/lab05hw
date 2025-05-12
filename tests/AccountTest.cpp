#include <gtest/gtest.h>
#include "Account.h"

TEST(AccountTest, ConstructorInitializesCorrectly) {
    Account acc(1, 100);
    EXPECT_EQ(acc.GetBalance(), 100);
}

TEST(AccountTest, ChangeBalanceThrowsIfNotLocked) {
    Account acc(1, 100);
    EXPECT_THROW(acc.ChangeBalance(50), std::runtime_error);
}

TEST(AccountTest, ChangeBalanceWorksWhenLocked) {
    Account acc(1, 100);
    acc.Lock();
    EXPECT_NO_THROW(acc.ChangeBalance(50));
    EXPECT_EQ(acc.GetBalance(), 150);
}

TEST(AccountTest, ChangeBalanceNegativeAmount) {
    Account acc(1, 100);
    acc.Lock();
    acc.ChangeBalance(-30);
    EXPECT_EQ(acc.GetBalance(), 70);
}

TEST(AccountTest, LockUnlockSequence) {
    Account acc(1, 100);
    EXPECT_NO_THROW(acc.Lock());
    EXPECT_THROW(acc.Lock(), std::runtime_error);
    EXPECT_NO_THROW(acc.Unlock());
    EXPECT_NO_THROW(acc.Lock());
}

TEST(AccountTest, UnlockWithoutLockDoesNotThrow) {
    Account acc(1, 100);
    EXPECT_NO_THROW(acc.Unlock());
}

TEST(AccountTest, MultipleChangesAfterLock) {
    Account acc(1, 0);
    acc.Lock();
    acc.ChangeBalance(10);
    acc.ChangeBalance(20);
    acc.ChangeBalance(-5);
    EXPECT_EQ(acc.GetBalance(), 25);
}
