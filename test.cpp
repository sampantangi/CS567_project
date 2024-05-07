#include <deepstate/DeepState.hpp>
#include "bank.cpp"  // Replace with the actual name of your code file

using namespace deepstate;

// Define a DeepState test suite for the Account class
TEST(Account, BasicFunctionality) {
  // Create an instance of SavingsAccount
  SavingsAccount savings("SA001", "John Doe", 1000.0, 5.0);
  ASSERT_EQ(savings.getBalance(), 1000.0);

  // Test deposit function
  savings.deposit(500.0);
  ASSERT_EQ(savings.getBalance(), 1525.0);

  // Test withdraw function
  savings.withdraw(300.0);
  ASSERT_EQ(savings.getBalance(), 1225.0);

  // Test transfer function
  CheckingAccount checking("CA001", "Jane Smith", 500.0, 100.0);
  savings.transfer(checking, 200.0);
  ASSERT_EQ(savings.getBalance(), 1025.0);
  ASSERT_EQ(checking.getBalance(), 700.0);

  // Test closeAccount function
  savings.closeAccount();
  ASSERT_EQ(savings.getBalance(), 0.0);

  // Test hasHolderName function
  ASSERT_TRUE(savings.hasHolderName("John Doe"));
  ASSERT_FALSE(savings.hasHolderName("Jane Smith"));
}

// Define a DeepState test suite for the Bank class
TEST(Bank, BasicFunctionality) {
  Bank bank;
  SavingsAccount savings("SA001", "John Doe", 1000.0, 5.0);
  CheckingAccount checking("CA001", "Jane Smith", 500.0, 100.0);

  // Add accounts to the bank
  bank.addAccount(&savings);
  bank.addAccount(&checking);

  // Test findAccount function
  ASSERT_NE(bank.findAccount("SA001"), nullptr);
  ASSERT_EQ(bank.findAccount("XYZ"), nullptr);

  // Test findAccountsByHolderName function
  auto accounts = bank.findAccountsByHolderName("John Doe");
  ASSERT_EQ(accounts.size(), 1);
  ASSERT_EQ(accounts[0]->getAccountNumber(), "SA001");

  // Test getTotalBalance function
  ASSERT_EQ(bank.getTotalBalance(), 1500.0);

  // Test closeAccount function
  bank.closeAccount("SA001");
  ASSERT_EQ(bank.findAccount("SA001"), nullptr);
  ASSERT_EQ(bank.getTotalBalance(), 500.0);
}
