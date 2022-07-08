#include<iostream>
#include<map>
#include<vector>
#include"order.h"
#include <bits/stdc++.h>
#include"reader.h"
#include"orderbook.h"
#include"genericReader.h"
#include"genericOrderbook.h"
#include <sys/time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(){
   
    //***************************************************************Basic class type programming***************************************************//

    struct timeval start, end;
    gettimeofday(&start, NULL);
    ios_base::sync_with_stdio(false);

    //timestamp was declared as tm struct type, and declared basic class and programming. Slower than string type timestamp below.
    Reader rdr("source.dat");
    rdr.ReadSource();
    map<string, vector<tmTimestampOrder>> orderbookResponse =rdr.GetOrderbook();
    Orderbook orderbook(orderbookResponse);
    vector<tmTimestampOrder> biggestBuys = orderbook.BiggestBuyOrders("DVAM1");
    tmTimestampOrder bestSellAtTime = orderbook.BestSellAtTime("TEST8","15:38:03");
    map<string,int> oCounts = orderbook.OrderCounts();


    gettimeofday(&end, NULL);
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
  
    cout << "Time taken by program is : " << fixed << time_taken << setprecision(6);
    cout << " sec" << endl;
   


    //***************************************************************Generic class type programming***************************************************//
    auto genericStart = high_resolution_clock::now();
    ios_base::sync_with_stdio(false);

    //faster than tm sruct type timestamp while stored data
    GenericReader<stringTimestampOrder> genericReader("source.dat");
    map<string, vector<stringTimestampOrder>> genericResponse =genericReader.GetOrderbook();
    GenericOrderbook<stringTimestampOrder> genericOrderbook(genericResponse);

    vector<stringTimestampOrder> biggestBuyOrders = genericOrderbook.BiggestBuyOrders("DVAM1");
    stringTimestampOrder bestSellOrderAtTime = genericOrderbook.BestSellAtTime("DVAM1","15:30:00");
    map<string,int> orderCounts = genericOrderbook.OrderCounts();


    auto genericStop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(genericStop - genericStart);
 
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    system("pause");


}

