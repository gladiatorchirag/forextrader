#include "forexmanager.h"
#include <vector>
#include <sstream>

ForexManager::ForexManager()
{

}

void ForexManager::addOrder(string orderDetails, unsigned int orderID)
{
    stringstream ss(orderDetails);
    string item;
    vector<string> splittedStrings;
    while (getline(ss, item, ':'))
        splittedStrings.push_back(item);
    if(splittedStrings.size() == 4){
        BaseOrder* order = new BaseOrder(splittedStrings.at(0),
                                        splittedStrings.at(1),
                                        atoi(splittedStrings.at(2).c_str()),
                                       atof(splittedStrings.at(3).c_str()), orderID);
        ordersList.push_back(order);
    }
}

void ForexManager::executeTradingSession()
{
    list<BaseOrder*>::iterator iter;
    for(iter = ordersList.begin(); iter != ordersList.end(); iter++) {
        BaseOrder* orderPtr = *iter;
        if(orderPtr->getInstrumentUnits() < 0){
            //means this is a seller, we need to find a buyer for it
            BaseOrder* bestAvailableBuyer = findBestBuyer(orderPtr);
            while((abs(orderPtr->getInstrumentUnits()) > 0)
                  && (bestAvailableBuyer != NULL)){
                float tradePrice = 0.0;
                if(orderPtr->getOrderID() < bestAvailableBuyer->getOrderID()){
                    tradePrice = orderPtr->getLimitPrice();
                }
                else
                {
                    tradePrice = bestAvailableBuyer->getLimitPrice();
                }
                tradeList.push_back(executeTrade(bestAvailableBuyer,
                                                 orderPtr,
                                                 tradePrice));
                bestAvailableBuyer = findBestBuyer(orderPtr);
            }
        }
        else{
            //We have a buyer, lets look for most aggressive sellers
            BaseOrder* bestAvailableSeller = findBestSeller(orderPtr);
            while((orderPtr->getInstrumentUnits() > 0)
                  && (bestAvailableSeller != NULL)){
                float tradePrice = 0.0;
                if(orderPtr->getOrderID() < bestAvailableSeller->getOrderID()){
                    tradePrice = orderPtr->getLimitPrice();
                }
                else
                {
                    tradePrice = bestAvailableSeller->getLimitPrice();
                }

                tradeList.push_back(executeTrade(orderPtr,
                                                 bestAvailableSeller,
                                                 tradePrice));
                bestAvailableSeller = findBestSeller(orderPtr);
            }
        }
    }
}

//First find all the sellers with limit price lower than buyer
//Sort them to find the lowest one
//If multiple 'lowest' sellers found, then find the one with earliest Order ID.
BaseOrder* ForexManager::findBestSeller(BaseOrder *buyer)
{
    vector<BaseOrder*> bestMatchedSellers;
    list<BaseOrder*>::iterator matchingSellerIter;
    for(matchingSellerIter = ordersList.begin(); matchingSellerIter != ordersList.end(); matchingSellerIter++){
        BaseOrder* seller = *matchingSellerIter;
        if(!buyer->getInstrumentDetails().compare(seller->getInstrumentDetails())){
            //we have the same instrument
            if(seller->getInstrumentUnits() < 0){
                //we have found a seller, check if it is willing to sell at this price
                if(seller->getLimitPrice() <= buyer->getLimitPrice()){
                    bestMatchedSellers.push_back(seller);
                }
            }
        }
    }

    if(bestMatchedSellers.empty()){
        //cout << "No seller found for buyer " << buyer->getCustomerID() << endl;
        return NULL;
    }

    if(bestMatchedSellers.size() == 1){
        //cout << "Found the best seller " << " for buyer " << buyer->getCustomerID() << endl;
        return bestMatchedSellers[0];
    }

    // Now we have a list containing best available sellers
    // Lets sort it to get the most aggressive seller
    // Doing a simple Bubble sort
    for(int j = 0; j < bestMatchedSellers.size() ; j++){
        for(int i = 0; i < bestMatchedSellers.size() - 1 ; i++){
            BaseOrder* firstOrder = bestMatchedSellers[i];
            BaseOrder* secondOrder = bestMatchedSellers[i+1];
            if(firstOrder->getLimitPrice() > secondOrder->getLimitPrice()){
                swap(firstOrder, secondOrder);
            }
        }
    }

    //Perform a sort again, this time for the order with lowest limit price and orderID
    for(int i = 1; i < bestMatchedSellers.size(); i++){
        BaseOrder* firstOrder = bestMatchedSellers[0];
        BaseOrder* nextOrder = bestMatchedSellers[i];
        if(firstOrder->getLimitPrice() == nextOrder->getLimitPrice()){
            if(firstOrder->getOrderID() > nextOrder->getOrderID())
                swap(firstOrder, nextOrder);
        }
        else{
            // means we already have the best order now, lets stop our sort here
            break;
        }
    }

    return bestMatchedSellers[0];
}

//First find all the buyers with limit price lower than buyer
//Sort them to find the highest one
//If multiple 'highest' buyers found, then find the one with earliest Order ID.
BaseOrder* ForexManager::findBestBuyer(BaseOrder *seller)
{
    vector<BaseOrder*> bestMatchedBuyers;
    list<BaseOrder*>::iterator iter;
    for(iter = ordersList.begin(); iter != ordersList.end(); iter++){
        BaseOrder* buyer = *iter;
        if(!seller->getInstrumentDetails().compare(buyer->getInstrumentDetails())){
            if(buyer->getInstrumentUnits() > 0){
                if(seller->getLimitPrice() < buyer->getLimitPrice()){
                    bestMatchedBuyers.push_back(buyer);
                }
            }
        }
    }

    if(bestMatchedBuyers.empty()){
//        cout << "No buyer found for seller " << seller->getCustomerID() << endl;
        return NULL;
    }

    if(bestMatchedBuyers.size() == 1){
//        cout << "Found the best buyer " << " for seller " << seller->getCustomerID() << endl;
        return bestMatchedBuyers[0];
    }

    // Now we have a list containing best available BUYERS
    // Lets sort it to get the MOST AGGRESSIVE BUYER
    // Doing a simple Bubble sort
    for(int j = 0; j < bestMatchedBuyers.size(); j++){
        for(int i = 0; i < bestMatchedBuyers.size() - 1 ; i++){
            BaseOrder* firstOrder = bestMatchedBuyers[i];
            BaseOrder* secondOrder = bestMatchedBuyers[i+1];
            if(firstOrder->getLimitPrice() < secondOrder->getLimitPrice()){
                swap(firstOrder, secondOrder);
            }
        }
    }

    //Perform a sort again, this time for the BUYER with HIGHEST limit price and lowest orderID
    for(int i = 1; i < bestMatchedBuyers.size(); i++){
        BaseOrder* firstOrder = bestMatchedBuyers[0];
        BaseOrder* nextOrder = bestMatchedBuyers[i];
        if(firstOrder->getLimitPrice() == nextOrder->getLimitPrice()){
            if(firstOrder->getOrderID() > nextOrder->getOrderID())
                swap(firstOrder, nextOrder);
        }
        else{
            // means we already have the best BUYER now, lets stop our sort here
            break;
        }
    }

    return bestMatchedBuyers[0];
}

Match* ForexManager::executeTrade(BaseOrder* buyer, BaseOrder* seller, float aLimitPrice)
{
    int unitsTraded = 0;
    if(abs(seller->getInstrumentUnits()) < buyer->getInstrumentUnits()){
        unitsTraded =  abs(seller->getInstrumentUnits());
    }
    else{
        unitsTraded = buyer->getInstrumentUnits();
    }
    Match* tradePtr = new Match(buyer->getCustomerID(), seller->getCustomerID(),
                                buyer->getInstrumentDetails(),
                                unitsTraded, aLimitPrice);

    buyer->setInstrumentUnits(buyer->getInstrumentUnits() - unitsTraded);
    seller->setInstrumentUnits(seller->getInstrumentUnits() + unitsTraded);
    return tradePtr;

}

void ForexManager::displayTradingDoneForThisSession()
{
    if(tradeList.empty())
        return;

    list<Match*>::iterator iter;
    for(iter = tradeList.begin(); iter != tradeList.end(); iter++) {
        Match* tradeToDisplay = *iter;
        tradeToDisplay->displayMatchResult();
    }
}

list<Match*> ForexManager::getTradingDoneForThisSession()
{
    return tradeList;
}

void ForexManager::displayOrdersList()
{
    if(ordersList.empty())
        return;

    list<BaseOrder*>::iterator iter;
    for(iter = ordersList.begin(); iter != ordersList.end(); iter++) {
        BaseOrder* orderToDisplay = *iter;
        orderToDisplay->displayOrderDetails();
    }
}

ForexManager::~ForexManager()
{
    while(!tradeList.empty()){
        tradeList.pop_front();
    }

    while(!ordersList.empty()){
        ordersList.pop_front();
    }
}
