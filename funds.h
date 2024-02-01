#ifndef FUNDS_H
#define FUNDS_H

#include <vector>
#include <string>

#include "transaction.h"

using namespace std;

class Funds {
 public:
  
  // Constructors
  Funds() : dollars_(0) {};
  Funds(int dollars) : dollars_(dollars) {};

  // Getters - Setters
  int dollars() const; 

  void set_dollars(int dollars);

  // Actions
  void LogPushBack(Transaction& log);
  void DisplayFundHistory() const;

 private:

  int dollars_;
  vector<Transaction> fund_transaction_logs;
}; 

#endif