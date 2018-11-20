#ifndef FOREXMANAGER_H
#define FOREXMANAGER_H
#include "order.h"
#include "match.h"
#include <list>


class ForexManager
{
public:
    ForexManager();
    ~ForexManager();
    void addOrder(string orderDetails, unsigned int orderID);
    void executeTradingSession();
    void displayTradingDoneForThisSession();
    void displayOrdersList();
    list<Match*> getTradingDoneForThisSession();
private:
    BaseOrder* findBestSeller(BaseOrder* buyer);
    BaseOrder* findBestBuyer(BaseOrder* seller);
    Match* executeTrade(BaseOrder* buyer, BaseOrder* seller, float aLimitPrice);

    list<BaseOrder*> ordersList;
    list<Match*> tradeList;
};

#endif // FOREXMANAGER_H
