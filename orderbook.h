#include<iostream>
#include <vector> 
using namespace std;


//declare orderbook class
class Orderbook{
    private:
        map<string, vector<tmTimestampOrder>> orderbooks;
        map<string, vector<tmTimestampOrder>> biggestBuyOrders;
        map<string,int> orderCounts;
        vector<tmTimestampOrder> orderList;

    public:
        
        Orderbook(map<string, vector<tmTimestampOrder>> orderbooks);
        map<string,int> OrderCounts();
        vector<tmTimestampOrder> BiggestBuyOrders(string symbol);
        tmTimestampOrder BestSellAtTime(string symbol, string _timestamp);

};

Orderbook::Orderbook(map<string, vector<tmTimestampOrder>> orderbooks)
{
    this->orderbooks = orderbooks;
}

map<string,int> Orderbook::OrderCounts()
{
    orderCounts.clear();
    for (pair<string, vector<tmTimestampOrder>> order : orderbooks)
        orderCounts[order.first]=order.second.size();
    return orderCounts;
}

vector<tmTimestampOrder> Orderbook::BiggestBuyOrders(string symbol)
{
    int orderSize = orderbooks[symbol].size();
    vector<tmTimestampOrder> orderList(3);

    for (int i=0;i < orderSize ; i++){
        if(orderbooks[symbol][i].side=="BUY"){
            if( orderbooks[symbol][i].price > orderList[0].price){
                orderList[2] = orderList[1];
                orderList[1] = orderList[0];
                orderList[0] =orderbooks[symbol][i];
                continue;   
            }
            if(  orderbooks[symbol][i].price > orderList[1].price){
                orderList[2] = orderList[1];
                orderList[1] =orderbooks[symbol][i]; 
                continue;   
            }
            if(  orderbooks[symbol][i].price > orderList[2].price){
                orderList[2] =orderbooks[symbol][i];  
                continue;   
            }
        }
    }

    return orderList;

}

tmTimestampOrder Orderbook::BestSellAtTime(string symbol, string _timestamp)
{
    int orderSize = orderbooks[symbol].size();
    tmTimestampOrder order;
    
    tm requestTime, responseTime;
    time_t resTime, reqTime, rawtime;
    double d;
    _timestamp ="20:02:2020 "+_timestamp;
    istringstream reqt(_timestamp);
    reqt >> get_time(&requestTime, "%d:%m:%Y %H:%M:%S");
    reqTime = mktime(&requestTime);
        
    for (int i=0;i < orderSize ; i++){
        if(orderbooks[symbol][i].side=="SELL"){

            responseTime= orderbooks[symbol][i].timestamp;
            get_time(&responseTime, "%d:%m:%Y %H:%M:%S");
            resTime = mktime(&responseTime);
            d = difftime(reqTime, resTime);
            if(0<=d){ 
                if(order.price < orderbooks[symbol][i].price){
                    order=orderbooks[symbol][i];
                }
            }
        }
    }

    return order;

}