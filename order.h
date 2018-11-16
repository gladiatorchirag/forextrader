#ifndef ORDER_H
#define ORDER_H
#include <string>

using namespace std;

class Order
{
public:
    Order(string aCustID, string aConvDetails, int aUnits, float aLPrice);

    const string getCustomerID(){return customerID;}
    const string getConversionDetails(){return conversionDetails;}
    const int getUnits(){return units;}
    const float getLimitPrice(){ return limitPrice;}

    virtual void execute(int units);

private:
    string customerID;
    string conversionDetails;
    int units;
    float limitPrice;
};

#endif // ORDER_H
