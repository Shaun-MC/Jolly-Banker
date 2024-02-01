#ifndef BANK_H
#define BANK_H

#include <queue>

#include "bstree.h"
#include "transaction.h"

class Bank {
 public:

 // Constructors / Destructors
  ~Bank();

 // Actions
 void ReadTransactions(const string file_name);

 void ExecuteTransactions();

 void DisplayOpenAccounts() const;

 private:
  
  void FlushQueue();

  queue<Transaction> transaction_list_;
  BSTree client_list_;
};

#endif
