#ifndef BSTREE_H
#define BSTREE_H

#include <string>
#include "account.h"

class BSTree {
 public:
  
  // Constructors, Destructor
  BSTree();
  ~BSTree();

  // Actions
  bool Insert(Account* account);

  bool Retrieve(const int account_id, Account* &account) const;
  
  void Display() const;

 private:

  struct Node {

   Account* p_acct_ = nullptr;
   Node* right_ = nullptr;
   Node* left_ = nullptr;
  };

  bool InsertHelper(Node* &traversal, Account* &account);
  void InsertNode(Node* &traversal, Account* &account);

  bool RetrieveHelper(Node* traversal, const int& account_id, Account* &account) const;

  void InOrderPrint(Node* current_node) const;

  void FlushTree(Node* traversal_node);

  Node* root_;
};

#endif