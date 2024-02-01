#include "bstree.h"

// Constructor - Deconstructor
BSTree::BSTree() {

    this->root_ = nullptr;
}

BSTree::~BSTree() {

    Node* traversal_node = this->root_;
    
    this->FlushTree(traversal_node);
}

// Actions
bool BSTree::Insert(Account* account) {

    return this->InsertHelper(this->root_, account);
}

bool BSTree::Retrieve(const int account_id, Account* & account) const {

    return this->RetrieveHelper(this->root_, account_id, account);
}

void BSTree::Display() const {

    this->InOrderPrint(this->root_);
}

// Private Member Functions
bool BSTree::InsertHelper(Node* &traversal, Account* &account) {

    if (traversal == nullptr) {

        InsertNode(traversal, account);

        return true;
    } else if (traversal->p_acct_->account_key() == account->account_key()) {

        return false;
    }

    if (account->account_key() < traversal->p_acct_->account_key()) {

        if (traversal->left_ == nullptr) {

            InsertNode(traversal->left_, account);

            return true;
        } else {

            return InsertHelper(traversal->left_, account);
        }
    } else {

        if (traversal->right_ == nullptr) {

            InsertNode(traversal->right_, account);

            return true;
        } else {

            return InsertHelper(traversal->right_, account);
        }
    }
} 

void BSTree::InsertNode(Node* &traversal, Account* &account) {

    Node* insert_node = new Node;
    insert_node->p_acct_ = new Account(*account);

    traversal = insert_node;
}

bool BSTree::RetrieveHelper(Node* traversal, const int& account_id, Account* &account) const {

    if (traversal == nullptr) {

        return false;
    } else if (account_id == traversal->p_acct_->account_key()) {

        account = traversal->p_acct_;
        
        return true;
    } 

    if (account_id < traversal->p_acct_->account_key()) {

        return RetrieveHelper(traversal->left_, account_id, account);
    } else {

        return RetrieveHelper(traversal->right_, account_id, account);
    }
}

void BSTree::InOrderPrint(Node* current_node) const {

    if (current_node == nullptr) {

        return;
    }

    this->InOrderPrint(current_node->left_);

    current_node->p_acct_->DisplayAccount();

    this->InOrderPrint(current_node->right_);
}

void BSTree::FlushTree(Node* traversal_node) { 

    if (traversal_node == nullptr){
        return;
    }

    FlushTree(traversal_node->left_);

    if (traversal_node->right_ == nullptr) {

        delete traversal_node->p_acct_;
        delete traversal_node;
        return;
    }

    FlushTree(traversal_node->right_);

    delete traversal_node->p_acct_;
    delete traversal_node;
    
    return;
}