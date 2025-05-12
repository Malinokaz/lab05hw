#include <gtest/gtest.h>
#include "Account.h"
#include "Transaction.h"

TEST(TransactionTest, SuccessTransferOver100) {
    Account from(1, 1000);
    Account to(2, 300);
    Transaction t;
    EXPECT_TRUE(t.Make(from, to, 200));
    EXPECT_EQ(from.GetBalance(), 1000); 
    EXPECT_EQ(to.GetBalance(), 300 + 200 - 201); 
}

TEST(TransactionTest, SameAccountThrows) {
    Account acc(1, 1000);
    Transaction t;
    EXPECT_THROW(t.Make(acc, acc, 150), std::logic_error);
}

TEST(TransactionTest, NegativeAmountThrows) {
    Account from(1, 1000), to(2, 0);
    Transaction t;
    EXPECT_THROW(t.Make(from, to, -1), std::invalid_argument);
}

TEST(TransactionTest, TooSmallAmountThrows) {
    Account from(1, 1000), to(2, 0);
    Transaction t;
    EXPECT_THROW(t.Make(from, to, 99), std::logic_error);
}

TEST(TransactionTest, FeeTooBigReturnsFalse) {
    Account from(1, 1000), to(2, 0);
    Transaction t;
    EXPECT_FALSE(t.Make(from, to, 1));  
}

TEST(TransactionTest, InsufficientFundsInFromReturnsFalse) {
    Account from(1, 100);
    Account to(2, 0);
    Transaction t;
    EXPECT_FALSE(t.Make(from, to, 100)); 
}


TEST(TransactionTest, RollbackOnDebitFailure) {
    Account from(1, 1000);
    Account to(2, 0); 
    Transaction t;
    EXPECT_FALSE(t.Make(from, to, 1100));  
    EXPECT_EQ(from.GetBalance(), 1000);    
    EXPECT_EQ(to.GetBalance(), 0);        
}

TEST(TransactionTest, AllBranchesInMakeCovered) {
    Account a1(1, 500);
    Account a2(2, 0);
    Transaction t;
    EXPECT_FALSE(t.Make(a1, a2, 1));    // fee*2 > sum
    EXPECT_THROW(t.Make(a1, a2, -1), std::invalid_argument); // negative
    EXPECT_THROW(t.Make(a1, a2, 99), std::logic_error);      // too small
}
