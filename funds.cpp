#include "funds.h"

#include <iostream>

using namespace std;

// Getter - Setter
int Funds::dollars() const {

    return this->dollars_;
}

void Funds::set_dollars(const int dollars) {

    this->dollars_ = dollars;
}

// Actions
void Funds::DisplayFundHistory() const {

    for (int i = 0; i < this->fund_transaction_logs.size(); i++) {

        cout << "  " << this->fund_transaction_logs[i] << '\n';
    }
}

void Funds::LogPushBack(Transaction& log) {

    this->fund_transaction_logs.push_back(log);
}
