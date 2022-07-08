#include<iostream>
#include <vector> 
using namespace std;


//define orderbook class
template<typename T>
class GenericOrderbook{
    private:
        map<string, vector<T>> orderbooks;
        map<string, vector<T>> biggestBuyOrders;
        map<string,int> orderCounts;
        vector<T> orderList;

    public:
        
        GenericOrderbook(map<string, vector<T>> orderbooks);
        map<string,int> OrderCounts();
        vector<T> BiggestBuyOrders(string symbol);
        T BestSellAtTime(string symbol, string _timestamp);

};

template<typename T>
GenericOrderbook<T>::GenericOrderbook(map<string, vector<T>> orderbooks)
{
    this->orderbooks = orderbooks;
}

template<typename T>
map<string,int> GenericOrderbook<T>::OrderCounts()
{
    orderCounts.clear();
    for (pair<string, vector<T>> order : orderbooks)
        orderCounts[order.first]=order.second.size();
    return orderCounts;
}

template<typename T>
vector<T> GenericOrderbook<T>::BiggestBuyOrders(string symbol)
{
    int orderSize = orderbooks[symbol].size();
    vector<T> orderList(3);

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

template<typename T>
T GenericOrderbook<T>::BestSellAtTime(string symbol, string _timestamp)
{
    int orderSize = orderbooks[symbol].size();
    T order;
    
    int timestamp =stoi(_timestamp.substr(0,2) + _timestamp.substr(3,2) + _timestamp.substr(6,2));
    
    for (int i=0;i < orderSize ; i++){
        if(orderbooks[symbol][i].side=="SELL"){
            int orderTimeStamp=stoi(orderbooks[symbol][i].timestamp.substr(0,2) + orderbooks[symbol][i].timestamp.substr(3,2) + orderbooks[symbol][i].timestamp.substr(6,2));
            if(orderTimeStamp<=timestamp){ 
                if(order.price<orderbooks[symbol][i].price){
                    order=orderbooks[symbol][i];
                }
            }
        }
    }

    return order;

}