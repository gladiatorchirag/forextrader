#include "match.h"

Match::Match(string buyer, string seller, string instrument, unsigned int quantity, float matchPrice)
{
    matchDisplayString = buyer;
    matchDisplayString.append(":");
    matchDisplayString.append(seller);
    matchDisplayString.append(":");
    matchDisplayString.append(instrument);
    matchDisplayString.append(":");
    matchDisplayString.append(std::to_string(quantity));
    matchDisplayString.append(":");
    matchDisplayString.append(std::to_string(matchPrice));
}

void Match::displayMatchResult()
{
    cout << matchDisplayString << endl;
}

string Match::getMatchResult()
{
    return matchDisplayString;
}
