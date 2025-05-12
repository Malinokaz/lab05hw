#include <gtest/gtest.h>
#include "Account.h"
#include "Transaction.h"

TEST(TransactionTest, SuccessTransferOver100) {
    Account a1(1, 1000);
    Account a2(2, 100);
    Transaction t;
    EXPECT_TRUE(t.Make(a1, a2, 200));
    EXPECT_EQ(a1.GetBalance(), 799);
    EXPECT_EQ(a2.GetBalance(), 300);
}

TEST(TransactionTest, FailIfSameAccount) {
    Account a(1, 1000);
    Transaction t;
    EXPECT_THROW(t.Make(a, a, 200), std::logic_error);
}

TEST(TransactionTest, FailIfNegativeAmount) {
    Account a1(1, 1000), a2(2, 500);
    Transaction t;
    EXPECT_THROW(t.Make(a1, a2, -50), std::invalid_argument);
}

TEST(TransactionTest, FailIfAmountLessThan100) {
    Account a1(1, 1000), a2(2, 500);
    Transaction t;
    EXPECT_THROW(t.Make(a1, a2, 99), std::logic_error);
}

TEST(TransactionTest, FailIfFeeTooBig) {
    Account a1(1, 1000), a2(2, 500);
    Transaction t;
    EXPECT_FALSE(t.Make(a1, a2, 1));
}

TEST(TransactionTest, FailIfNotEnoughAfterFee) {
    Account a1(1, 150), a2(2, 50);
    Transaction t;
    EXPECT_FALSE(t.Make(a1, a2, 150));
    EXPECT_EQ(a1.GetBalance(), 150);
    EXPECT_EQ(a2.GetBalance(), 50);
}

TEST(TransactionTest, MultipleTransactions) {
    Account a1(1, 1000), a2(2, 500);
    Transaction t;
    EXPECT_TRUE(t.Make(a1, a2, 200));
    EXPECT_TRUE(t.Make(a1, a2, 300));
    EXPECT_TRUE(t.Make(a2, a1, 250));
    EXPECT_EQ(a1.GetBalance(), 748);
    EXPECT_EQ(a2.GetBalance(), 749);
}
