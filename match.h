#ifndef MATCH_H
#define MATCH_H
#include <iostream>

using namespace std;

class Match
{
public:
    Match(string buyer, string seller, string instrument, unsigned int quantity, float matchPrice);
    void displayMatchResult();
    string getMatchResult();
private:
    string matchDisplayString;
};

#endif // MATCH_H
