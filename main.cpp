#include <iostream>
#include <fstream>
#include "forexmanager.h"

using namespace std;


int main(int argc, char *argv[])
{
    cout << "Welcome to Foreign Exhange trading services!" << endl;
    if(argc < 3){
        cout << "Please give correct arguments" << endl;
        cout << "USAGE: ./Forextrader inputfile.txt outputfile.txt" << endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if(!inputFile.is_open()){
        cout << "Error! Cannot open input file for reading. Exiting..." << endl;
        return 1;
    }
    string inputStr;
    ForexManager mgr;
    int orderID = 0;
    while(getline(inputFile, inputStr)){
        mgr.addOrder(inputStr, orderID++);
    }

    inputFile.close();

    mgr.displayOrdersList();

    mgr.executeTradingSession();

    mgr.displayTradingDoneForThisSession();

    list<Match*> tradesList = mgr.getTradingDoneForThisSession();

    ofstream outFile(argv[2]);
    if(!outFile.is_open()){
        cout << "Error! Cannot open output file for reading. Exiting..." << endl;
        return 1;
    }
    if(!tradesList.empty()){

        list<Match*>::iterator iter = tradesList.begin();
        while(iter != tradesList.end()){
            Match* trade = *iter;
            outFile << trade->getMatchResult().c_str() << endl;
            iter++;
        }
    }

    outFile.close();

    return 0;
}
