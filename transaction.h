#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>

using namespace std;

class Transaction {
 public:

  // Contstructor
  Transaction();

  // Getters 
  string account_name() const;
  char transaction_type() const;

  int from_accountID() const;
  int to_accountID() const;

  int from_fundID() const;
  int to_fundID() const;

  int transfer_amount() const;

  string failed() const;

  // Setters
  void set_transaction_type(const char t_type);
  void set_account_name(const string first_name, const string last_name);
 
  void set_from_accountID(const int accountID);
  void set_to_accountID(const int accountID);

  void set_from_fundID(const int fundID);
  void set_to_fundID(const int fundID);
 
  void set_transfer_amount(const int money_amount);

  void set_failed(const string failed);

  // Actions
  bool CheckAccountID(int account) const;
  bool CheckFundID (int fund_num);

  // Operator Overloads
  friend istream& operator >> (istream& istrm, Transaction& input);
  friend ostream& operator << (ostream& ostrm, const Transaction& output);

 private:

  string account_name_;
  char transaction_type_;

  int from_accountID_;
  int to_accountID_;

  int from_fundID_;
  int to_fundID_;

  int transfer_amount_;

  string failed_;
};

#endif