#include "account.h"

// Constructors
Account::Account() {

    this->funds_[0].set_dollars(200);
    this->account_name_ = " ";
    this->account_key_ = 0;
}

Account::Account(const string name, const int key) {

    this->funds_[0].set_dollars(200);
    this->account_name_ = name;
    this->account_key_ = key;
}

// Getters - Setters
string Account::account_name() const {

    return this->account_name_;
}

int Account::account_key() const {

    return this->account_key_;
}

void Account::set_account_name(const string name) {

    this->account_name_ = name;
}

void Account::set_account_key(const int key) {

    this->account_key_ = key;
}

void Account::DepositFunds(Transaction log) { // ACCEPTABLE
    
    if (this->CheckTransferAmount(log.transfer_amount())) {

        cerr << "ERROR: Attempting To Deposit Negative Cash Into " << this->account_name() << " (#" << this->account_key() << ") "
             << this->fund_names_[log.from_fundID()] << '\n';
        
        log.set_failed("(Failed)");
    } else {

        this->funds_[log.from_fundID()].set_dollars(this->funds_[log.from_fundID()].dollars() + log.transfer_amount());
    }

    this->funds_[log.from_fundID()].LogPushBack(log);
}

void Account::WithdrawFunds(Transaction log) { // REFACTOR

    bool covering_flag = false;

    if (this->CheckTransferAmount(log.transfer_amount())) {

        cerr << "ERROR: Attempting To Withdraw Negative Cash From " << this->account_name() << " (#" << this->account_key() << ") "
             << this->fund_names_[log.from_fundID()] << '\n';
        
        log.set_failed("(Failed)");

    } else {

        int modifier = (log.from_fundID() % 2 == 0) ? 1 : -1;
        
        // If from_fundID == a Covering Fund && Both Covering funds can't cover, No Withdraw
        // if from_fundID == a non Covering Fund && Withdraw Exceeds Fund dollars(), No Withdraw
        if (log.from_fundID() < 4 && (this->funds_[log.from_fundID()].dollars() + this->funds_[log.from_fundID() + modifier].dollars() < log.transfer_amount()) ||
            log.from_fundID() >= 4 && this->funds_[log.from_fundID()].dollars() < log.transfer_amount()) {

            cerr << "ERROR: Unable to Withdraw $" << log.transfer_amount() << " From " << this->account_name() << " (#"
                << this->account_key() << ") " << this->fund_names_[log.from_fundID()] << '\n';

            log.set_failed("(Failed)");

        // ACCEPTABLE - MOST READABLE VERSION
        // If from_fundID == a Covering Fund && from_fundID + modifier HAS TO Cover, Execute Withdraw
        } else if (log.from_fundID() < 4 && this->funds_[log.from_fundID()].dollars() < log.transfer_amount()) {

            this->CoveringFunds(this, log, modifier);

            covering_flag = true;

        // this->funds_[log.from_fundID()] CAN Withdraw transfer_amount - Execute Withdraw
        } else { 

            this->funds_[log.from_fundID()].set_dollars(this->funds_[log.from_fundID()].dollars() - log.transfer_amount());
        }
    }

    if (!covering_flag) {

        this->funds_[log.from_fundID()].LogPushBack(log);
    }
}

void Account::TransferFunds(Account* to_account, Transaction log) { // TEST + REFACTOR

    bool covering_flag = false;

    if (this->CheckTransferAmount(log.transfer_amount())) {

        cerr << "ERROR: Attempting To Transfer Negative Cash From " << this->account_name() << " (#" << this->account_key() << ") "
             << this->fund_names_[log.from_fundID()] << " To " << to_account->account_name() << " (#" << to_account->account_key() 
             << ")" << " " << this->fund_names_[log.to_fundID()] << '\n';
        
        log.set_failed("(Failed)");

    } else {

        int modifier = (log.from_fundID() % 2 == 0) ? 1 : -1;

        // If from_fundID == a Covering Fund && Both Covering funds can't cover, No Transfer
        // if from_fundID == a non Covering Fund && Withdraw Exceeds Fund dollars(), No Transfer
        if (log.from_fundID() < 4 && (this->funds_[log.from_fundID()].dollars() + this->funds_[log.from_fundID() + modifier].dollars() < log.transfer_amount()) ||
            (log.from_fundID() >= 4 && this->funds_[log.from_fundID()].dollars() < log.transfer_amount())) {
            
            cerr << "ERROR: Unable to Transfer $" << log.transfer_amount() << " From " 
                 << this->account_name() << " " << "(#" << this->account_key() << ")" << " " 
                 << this->fund_names_[log.from_fundID()] << " To " << to_account->account_name() << " (#" << to_account->account_key() 
                 << ")" << " " << this->fund_names_[log.to_fundID()] << '\n';

            log.set_failed("(Failed)");

        } else if (log.from_fundID() < 4 && this->funds_[log.from_fundID()].dollars() < log.transfer_amount()) { // Funds 0-3 Covering
            
            this->CoveringFunds(to_account, log, modifier);

            to_account->funds_[log.to_fundID()].set_dollars(this->funds_[log.to_fundID()].dollars() + log.transfer_amount());  

            covering_flag = true;

        } else { // Straight Transfer

            this->funds_[log.from_fundID()].set_dollars(this->funds_[log.from_fundID()].dollars() - log.transfer_amount());
            to_account->funds_[log.to_fundID()].set_dollars(to_account->funds_[log.to_fundID()].dollars() + log.transfer_amount());
        }
    }

    if (this->account_key() == to_account->account_key() && log.from_fundID() == log.to_fundID()) {

        this->funds_->LogPushBack(log);

    } else if (!covering_flag) {
        
        this->funds_[log.from_fundID()].LogPushBack(log);
        to_account->funds_[log.to_fundID()].LogPushBack(log);
    } 
}

void Account::DisplayAccount() const {

    cout << this->account_name() << " Account ID: " << this->account_key() << '\n';

    for (int i = 0; i < kFundsAmount; i++) {

        cout << "    " << this->fund_names_[i] << ": $" 
             << this->funds_[i].dollars() << '\n';
    }

    cout << '\n';
}

void Account::DisplayAccountHistory() const {

    cout << "\nTransaction History For " << this->account_name() << " (#" << this->account_key() << ")" << " By Fund" << '\n';

    for (int i = 0; i < kFundsAmount; i++) {

        cout << this->fund_names_[i] << ": $" << this->funds_[i].dollars() << '\n';

        this->funds_[i].DisplayFundHistory();

        cout << '\n';
    }
}

void Account::DisplayFundHistory(const int fundID) const {

    cout << "\nTransaction History For " << this->account_name() << " (#" << this->account_key() << ")" << " " << this->fund_names_[fundID];

    cout << ": $" << this->funds_[fundID].dollars() << '\n';
    
    this->funds_[fundID].DisplayFundHistory();
}

void Account::DetermineTransferErrorMessageAndStorage(Transaction& log, const int direction) { // Make Private

    // If function is called w/ account, what happens
    // If function is called w/ to_account what happens

    log.set_failed("(Failed)");

    if (! log.CheckFundID(direction)) { // If from_fundID or to_fundID is Valid - Store the Failed Transaction

        switch (direction) {

            case 1: 
            this->funds_[log.from_fundID()].LogPushBack(log);
            break;
            
            case 2:
            this->funds_[log.to_fundID()].LogPushBack(log);
            break;
        }
    }

    if (log.CheckAccountID(direction)) { // from_fundID or to_fundID Not Found in Tree Because Key is Negative

        cerr << "ERROR: Account #'s Cannot Be Negative. Transaction Refused" << '\n';
    } else {

        string error_msg = "ERROR: Account #";

        switch (direction) {

            case 1:
            error_msg += std::to_string(log.to_accountID());
            break;
            
            case 2:
            error_msg += std::to_string(log.from_accountID());
        }

        error_msg += " Not Found. Transaction Refused. \n";
        
        cerr << error_msg;
    }
}

void Account::FundLogPushBack(Transaction& log, const int fundID) {

    this->funds_[fundID].LogPushBack(log);
}

bool Account::CheckTransferAmount(const int cash) {

    return (cash < 0) ? true : false;
}

void Account::CoveringFunds(Account* to_account, const Transaction& log, const int modifier) {

    int diff = log.transfer_amount() - this->funds_[log.from_fundID()].dollars(); // Transfer

    Transaction log_transaction2 = log;
    Transaction log_transaction3 = log;

    log_transaction2.set_transfer_amount(this->funds_[log.from_fundID()].dollars()); // Transfer/Withdraw As Much as Possible From from_fundID();
    
    log_transaction3.set_from_fundID(log.from_fundID() + modifier); // Setting Transfer/Withdraw Amount For from_fundID + modifier
    log_transaction3.set_transfer_amount(diff);                     // Setting Transfer/Withdraw Amount For from_fundID + modifier

    this->funds_[log.from_fundID()].set_dollars(0); // Setting from_fundID to 0

    // Setting from_fundID + modifier to Remaining Amount
    this->funds_[log.from_fundID() + modifier].set_dollars(this->funds_[log.from_fundID() + modifier].dollars() - diff);

    this->funds_[log.from_fundID() + modifier].LogPushBack(log_transaction3);
    this->funds_[log.from_fundID()].LogPushBack(log_transaction2);

    if (log.transaction_type() == 'T') {
        
        to_account->funds_[log.to_fundID()].LogPushBack(log_transaction2);
        to_account->funds_[log.to_fundID()].LogPushBack(log_transaction3);
    }
}