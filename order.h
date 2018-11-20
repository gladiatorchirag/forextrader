#ifndef ORDER_H
#define ORDER_H

#include <string>

using namespace std;

class BaseOrder
{
public:

    BaseOrder(string aCustID, string aConvDetails, int aUnits, float aLPrice, unsigned int aOrderID);

    const string getCustomerID(){return customerID;}
    const string getInstrumentDetails(){return instrumentDetails;}
    const int getInstrumentUnits(){return instrumentUnits;}
    const float getLimitPrice(){ return limitPrice;}
    const unsigned int getOrderID(){ return orderID;}
    void setInstrumentUnits(int aUnits){instrumentUnits = aUnits;}
    void displayOrderDetails();

private:
    string customerID;
    string instrumentDetails;
    int instrumentUnits;
    float limitPrice;
    unsigned int orderID;
};

#endif // ORDER_H
