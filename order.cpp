#include "order.h"
#include <iostream>
BaseOrder::BaseOrder(string aCustID, string aConvDetails, int aUnits, float aLPrice, unsigned int aOrderID)
{
    customerID = aCustID;
    instrumentDetails = aConvDetails;
    instrumentUnits = aUnits;
    limitPrice = aLPrice;
    orderID = aOrderID;
}

void BaseOrder::displayOrderDetails()
{
    cout << "ORDER " << orderID << " -- " << customerID.c_str() << ":"
         << instrumentDetails.c_str() << ":"
         << instrumentUnits << ":" << limitPrice  << ":" << std::endl;
}
