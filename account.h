#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>

#include "funds.h"
#include "transaction.h"

const int kFundsAmount = 9;

using namespace std;

class Account {
 public:

  // Constructors
  Account();
  Account(const string name, const int key);

  // Getters-Setters
  string account_name() const;
  int account_key() const;

  void set_account_name(const string name);
  void set_account_key(const int key);

  // Actions
  void DepositFunds(Transaction log);
  
  void WithdrawFunds(Transaction log);
                     
  void TransferFunds(Account* to_account, Transaction log);

  void DisplayAccount() const;
  
  void DisplayAccountHistory() const;
  void DisplayFundHistory(const int fundID) const;

  void FundLogPushBack(Transaction& log, const int fundID);

  void DetermineTransferErrorMessageAndStorage(Transaction& log, const int direction);

 private:

  bool CheckTransferAmount(const int cash);
  void CoveringFunds(Account* to_account, const Transaction& log, const int modifier);

  Funds funds_ [kFundsAmount];
  const string fund_names_ [kFundsAmount] = {"Money Market", "Prime Money Market", 
                                             "Long-Term Bond", "Short-Term Bond", 
                                             "500 Index Fund", "Capital Value Fund", 
                                             "Growth Equity Fund", "Growth Index Fund", 
                                             "International Fund"};
  
  string account_name_;
  int account_key_;
};

#endif