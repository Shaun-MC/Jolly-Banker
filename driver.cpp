#include <iostream>
#include <string>

#include "bank.h"

using namespace std;

int main(int args, char* argv[]) {

    Bank jolly_banker;

    jolly_banker.ReadTransactions(argv[1]);

    jolly_banker.ExecuteTransactions();

    jolly_banker.DisplayOpenAccounts();

    return 0;
}
