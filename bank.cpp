#include "bank.h"

#include <fstream>

Bank::~Bank() {

    this->FlushQueue();
}

void Bank::ReadTransactions(const string file_name) {

    ifstream read_transactions;

    read_transactions.open(file_name);

    if (!read_transactions.is_open()) {

        cerr << "ERROR: Unable to Open Test File (" << file_name << ")";
        return;
    }

    while (!read_transactions.eof()) {

        Transaction temp;

        read_transactions >> temp;
        
        transaction_list_.push(temp);
    }

    read_transactions.close();
}

void Bank::ExecuteTransactions() {

    Transaction* object_ptr;

    while (!transaction_list_.empty()) {

        object_ptr = &transaction_list_.front();

        Account* account = nullptr;

        if (object_ptr->transaction_type() != 'O' && object_ptr->transaction_type() != 'T') {

            // Locate Account 
            if ( !this->client_list_.Retrieve(object_ptr->from_accountID(), account) ) {

                cerr << "ERROR: Account #" << std::to_string(object_ptr->from_accountID()) << " Not Found. Transaction Refused." << '\n';

                transaction_list_.pop();              
                continue;
            }

            // Error Checking - Fine - Only 'D', 'W', 'F' allowed
            if (object_ptr->transaction_type() != 'H' && object_ptr->CheckFundID(1) ) {
                
                cerr << "ERROR: Fund ID " << std::to_string(object_ptr->from_fundID()) << " Is Invalid. Transaction Refused" << '\n';    
                transaction_list_.pop();            
                continue;
            }
        }

        switch (object_ptr->transaction_type()) {

            case 'O': { // Fine

                if (object_ptr->CheckAccountID(1)) {

                    cerr << "ERROR: Account ID's Cannot Be Negative Or 0. Transaction Refused" << '\n';
                    break;
                }

                account = new Account(object_ptr->account_name(), object_ptr->from_accountID());
                
                // Open an Account + Error Checking
                if ( !(this->client_list_.Insert(account)) ) {

                    cerr << "ERROR: Account #"  << object_ptr->from_accountID() << " Is Already Open. Account Opening Refused." << '\n';
                    // Unique Error Message
                }

                delete account;
                break;
            }
            case 'D': {

                account->DepositFunds(*object_ptr);
                break;
            }
            case 'W': {
                
                account->WithdrawFunds(*object_ptr);
                break;
            }
            case 'T': {
                
                bool from_account_found = false;
                bool to_account_found = false;

                Account* to_account = nullptr;

                // Transfer Error Checking That Maintains Fund Log Correctness

                // Possibly Find object_ptr->from_accountID in client_list_, Store Results in account
                if (this->client_list_.Retrieve(object_ptr->from_accountID(), account)) {

                    from_account_found = true;
                }

                // Possibly Find object_ptr->to_accountID in client_list_, Store Results in to_account
                if (this->client_list_.Retrieve(object_ptr->to_accountID(), to_account)) {

                    to_account_found = true;
                }

                // Neither Accounts Were Found In Tree
                if (!from_account_found && !to_account_found) { 

                    cerr << "ERROR: Account #" << std::to_string(object_ptr->from_accountID()) << " AND Account #" 
                         << std::to_string(object_ptr->to_accountID()) << " Not Found. Transaction Refused." << '\n';
                         
                } else if (from_account_found && !to_account_found) { // account Was Found In Tree - But Not to_account

                    account->DetermineTransferErrorMessageAndStorage(*object_ptr, 1);

                } else if (!from_account_found && to_account_found) { // to_account Was Found In Tree - But Not account
                    
                    to_account->DetermineTransferErrorMessageAndStorage(*object_ptr, 2);
                    
                } else {   // Both Accounts Were Found in Tree

                    if (object_ptr->CheckFundID(1)) { // If from_fundID() is Invalid

                        cerr << "ERROR: Fund ID " << std::to_string(object_ptr->from_fundID()) << " Is Invalid. Transaction Refused" << '\n';

                        object_ptr->set_failed("(Failed)");

                        to_account->FundLogPushBack(*object_ptr, object_ptr->to_fundID());

                    } else if (object_ptr->CheckFundID(2)) { // if to_fundID() is Invalid

                        cerr << "ERROR: Fund ID " << std::to_string(object_ptr->to_fundID()) << " Is Invalid. Transaction Refused" << '\n';

                        object_ptr->set_failed("(Failed)");

                        account->FundLogPushBack(*object_ptr, object_ptr->from_fundID());

                    } else {    // If Both Accounts are Found & Both Funds IDs Are Valid

                        if (object_ptr->from_accountID() == object_ptr->to_accountID()) { // Single Client

                            account->TransferFunds(account, *object_ptr);

                        } else { // Multi Client

                            account->TransferFunds(to_account, *object_ptr);
                        }
                    }   
                }

                break;
            }

            case 'H': { // Error Checking Already Complete - If Conditionals Before Switch Statements

                account->DisplayAccountHistory();
                break;

            }
            case 'F': { // Error Checking Already Complete - If Conditionals Before Switch Statements

                account->DisplayFundHistory(object_ptr->from_fundID());
                break;
            }
        }

        transaction_list_.pop();   
    }
}

void Bank::DisplayOpenAccounts() const {

    cout << "\nOpen Accounts: " << '\n' << '\n';

    this->client_list_.Display();
}

void Bank::FlushQueue() {

    while (!this->transaction_list_.empty()) {

        this->transaction_list_.pop();
    }
}