#include "transaction.h"

Transaction::Transaction() {    

    this->account_name_ = " ";
    this->transaction_type_ = ' ';
    
    this->from_accountID_ = 0;
    this->to_accountID_ = 0;

    this->from_fundID_ = 0;
    this->to_fundID_ = 0;

    this->transfer_amount_ = 0;

    this->failed_ = " ";
}

// Getters
string Transaction::account_name() const {

    return this->account_name_;
}
char Transaction::transaction_type() const {

    return this->transaction_type_;
}
int Transaction::from_accountID() const {

    return this->from_accountID_;
}
int Transaction::to_accountID() const {

    return this->to_accountID_;
}
int Transaction::from_fundID() const {

    return this->from_fundID_;
}
int Transaction::to_fundID() const {

    return this->to_fundID_;
}
int Transaction::transfer_amount() const{ 

    return this->transfer_amount_;
}
string Transaction::failed() const {

    return this->failed_;
}

// Setters
void Transaction::set_transaction_type(const char t_type) { 

    this->transaction_type_ = t_type;
}
void Transaction::set_account_name(const string first_name, const string last_name) {     

    this->account_name_ = first_name + ' ' + last_name;
}
void Transaction::set_from_accountID(const int accountID) { 

    this->from_accountID_ = accountID;
}
void Transaction::set_to_accountID(const int accountID) {

    this->to_accountID_ = accountID;
}
void Transaction::set_from_fundID(const int fundID) {

    this->from_fundID_ = fundID;
}
void Transaction::set_to_fundID(const int fundID) {

    this->to_fundID_ = fundID;
}
void Transaction::set_transfer_amount(const int money_amount) {

    this->transfer_amount_ = money_amount;
}

void Transaction::set_failed(const string failed) {

    this->failed_ = failed;
}

// Actions
bool Transaction::CheckAccountID(int accountID) const {

    bool ret = false;
    
    switch (accountID) {

        case 1:
        if (this->from_accountID() <= 0) {

            ret = true;
        } 
        break;

        case 2: 
        if (this->to_accountID() <= 0) {

            ret = true;
        } 
        break;
    }

    return ret;
}

bool Transaction::CheckFundID(int fund_num) {

    bool ret = false;

    switch(fund_num) {

        case 1: 
        if (this->from_fundID() < 0 || this->from_fundID() >= 9) {

            ret = true;
        }
        break;

        case 2:
        if (this->to_fundID() < 0 || this->to_fundID() >= 9) {

            ret = true;
        }
        break;
    }

    return ret;
}

istream& operator >> (istream& istrm, Transaction& input) {

    char transaction_type = ' ';
    int account_id = 0;

    istrm >> transaction_type;
    input.set_transaction_type(transaction_type);

    istrm >> account_id;
    input.set_from_accountID(account_id);

    switch (transaction_type) {

        case 'O': {
            
            string last_name = " ";
            string first_name = " ";

            istrm >> last_name >> first_name;

            input.set_account_name(first_name, last_name);
            break;
        }

        case 'W':
        case 'D': {

            int fundID = 0; 
            int transfer_amount = 0;

            istrm >> fundID >> transfer_amount;

            input.set_from_fundID(fundID);
            input.set_transfer_amount(transfer_amount);
            break;
        }

        case 'T': {
            
            int from_fundID = 0; 
            int to_accountID = 0; 
            int to_fundID = 0; 
            int transfer_amount = 0;

            istrm >> from_fundID >> to_accountID >> to_fundID >> transfer_amount;

            input.set_from_fundID(from_fundID);
            input.set_to_accountID(to_accountID);
            input.set_to_fundID(to_fundID);
            input.set_transfer_amount(transfer_amount);
            
            break;
        }

        case 'H':
        break;

        case 'F': {
            
            int fund_ID = 0;
        
            istrm >> fund_ID;

            input.set_from_fundID(fund_ID);
            break;
        }
    }

    return istrm;
}

ostream& operator << (ostream& ostrm, const Transaction& output) {

    ostrm << output.transaction_type() << " " << output.from_accountID() << " ";

    switch (output.transaction_type()) {

        case 'O':
        ostrm << output.account_name();
        break;

        case 'D':
        case 'W':
        ostrm << output.from_fundID() << " " << output.transfer_amount();
        break;

        case 'T':
        ostrm << output.from_fundID() << " " << output.to_accountID() << " " << output.to_fundID() << " ";
        ostrm << output.transfer_amount();
        break;

        case 'H':
        case 'F':
        break;
    }

    ostrm << " " << output.failed();

    return ostrm;
}