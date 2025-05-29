#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Transaction.h"
#include "Account.h"

using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}

    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
    MOCK_METHOD(int, id, (), (const));
};

class TestTransaction : public Transaction {
public:
    MOCK_METHOD(void, SaveToDataBase, (Account&, Account&, int), (override));
};

TEST(TransactionTest, SuccessTransferOver100) {
    MockAccount from(1, 1000);
    MockAccount to(2, 300);
    TestTransaction txn;

    EXPECT_CALL(from, id()).WillRepeatedly(Return(1));
    EXPECT_CALL(to, id()).WillRepeatedly(Return(2));

    EXPECT_CALL(from, Lock());
    EXPECT_CALL(to, Lock());
    EXPECT_CALL(from, Unlock());
    EXPECT_CALL(to, Unlock());

    EXPECT_CALL(to, ChangeBalance(200));
    EXPECT_CALL(to, GetBalance()).WillOnce(Return(500));
    EXPECT_CALL(to, ChangeBalance(-201));

    EXPECT_CALL(txn, SaveToDataBase(_, _, 200));

    EXPECT_TRUE(txn.Make(from, to, 200));
}

TEST(TransactionTest, SameAccountThrows) {
    MockAccount acc(1, 1000);
    TestTransaction txn;

    EXPECT_CALL(acc, id()).WillRepeatedly(Return(1));
    EXPECT_THROW(txn.Make(acc, acc, 150), std::logic_error);
}

TEST(TransactionTest, NegativeAmountThrows) {
    MockAccount from(1, 1000), to(2, 0);
    TestTransaction txn;

    EXPECT_CALL(from, id()).WillRepeatedly(Return(1));
    EXPECT_CALL(to, id()).WillRepeatedly(Return(2));
    EXPECT_THROW(txn.Make(from, to, -1), std::invalid_argument);
}

TEST(TransactionTest, TooSmallAmountThrows) {
    MockAccount from(1, 1000), to(2, 0);
    TestTransaction txn;

    EXPECT_CALL(from, id()).WillRepeatedly(Return(1));
    EXPECT_CALL(to, id()).WillRepeatedly(Return(2));
    EXPECT_THROW(txn.Make(from, to, 99), std::logic_error);
}

TEST(TransactionTest, FeeTooBigReturnsFalse) {
    MockAccount from(1, 1000), to(2, 0);
    TestTransaction txn;
    txn.set_fee(100);

    EXPECT_CALL(from, id()).WillRepeatedly(Return(1));
    EXPECT_CALL(to, id()).WillRepeatedly(Return(2));
    EXPECT_FALSE(txn.Make(from, to, 100));
}

TEST(TransactionTest, InsufficientFundsInFromReturnsFalse) {
    MockAccount from(1, 100), to(2, 0);
    TestTransaction txn;

    EXPECT_CALL(from, id()).WillRepeatedly(Return(1));
    EXPECT_CALL(to, id()).WillRepeatedly(Return(2));
    EXPECT_FALSE(txn.Make(from, to, 100));
}

TEST(TransactionTest, RollbackOnDebitFailure) {
    MockAccount from(1, 1000);
    MockAccount to(2, 0);
    TestTransaction txn;

    EXPECT_CALL(from, id()).WillRepeatedly(Return(1));
    EXPECT_CALL(to, id()).WillRepeatedly(Return(2));

    EXPECT_CALL(from, Lock());
    EXPECT_CALL(to, Lock());
    EXPECT_CALL(from, Unlock());
    EXPECT_CALL(to, Unlock());

    EXPECT_CALL(to, ChangeBalance(1100));
    EXPECT_CALL(to, GetBalance()).WillOnce(Return(0));
    EXPECT_CALL(to, ChangeBalance(-1100));

    EXPECT_CALL(txn, SaveToDataBase(_, _, 1100));

    EXPECT_FALSE(txn.Make(from, to, 1100));
}
