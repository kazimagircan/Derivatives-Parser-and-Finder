#include<iostream>
#include<fstream>
#include <algorithm>
using namespace std;
#define SPLITLISTSIZE 7

//define GenericReader class
template<typename T>
class GenericReader{
    private:
        fstream source;
        string strings[SPLITLISTSIZE];
        string sourcePath;
        map<string, vector<T>> orderbooks;
        vector<T> orderList;
        string line;
        string substring;
        T o;
        int currIndex;
        int iterator;
        int startIndex;
        int endIndex;
        int orderSize;
        int lineSize;
        char seperator;
        void SplitText();
        void ReadOrder();
        void Insert();
        void Cancel();
        void Amend();
        void ReadSource();


    public:
        GenericReader(string sourcePath);
        ~GenericReader();

        map<string, vector<T>> GetOrderbook();

};

template<typename T>
GenericReader<T>::GenericReader(string sourcePath)
{
    this->sourcePath = sourcePath;
    this->seperator=';';
}

template<typename T>
GenericReader<T>::~GenericReader()
{
    if(this->source.is_open()){
       this->source.close();
    }
}

template<typename T>
void GenericReader<T>::SplitText()
{

    currIndex = iterator = startIndex = endIndex = 0;  
    lineSize = line.size();
             
    while (iterator <= lineSize)  
    {  
        if (line[iterator] == seperator || iterator == lineSize)  
        {  
            endIndex = iterator;  
            substring = "";  
            substring.append(line, startIndex, endIndex - startIndex);  
            strings[currIndex] = substring;
            currIndex += 1;  
            startIndex = endIndex + 1;  
        }  
        iterator++;  
    }

    o.timestamp=strings[0];
    o.symbol=strings[1];
    o.orderId=stoi(strings[2]);
    o.operation=strings[3][0];
    o.side=strings[4];
    o.volume=stoi(strings[5]);
    o.price=stod(strings[6]);

}

template<typename T>
void GenericReader<T>::ReadSource()
{
    if(this->sourcePath.empty()) {
        cout << "problem occurs when open a file !" << endl;
        exit(1);
    }
    this->source.open(this->sourcePath,ios::in);

    if(this->source.is_open()){
        while ( getline(this->source, this->line) ){
            this->SplitText();
            this->ReadOrder();
        }
    }
}

template<typename T>
map<string, vector<T>> GenericReader<T>::GetOrderbook()
{
    this->ReadSource();
    return this->orderbooks;
}

template<typename T>
void GenericReader<T>::ReadOrder()
{
    if(this->o.operation =='I')
        this->Insert();
    else if(this->o.operation =='C')
        this->Cancel();
    else
        this->Amend();

}

template<typename T>
void GenericReader<T>::Insert()
{
    orderSize = orderbooks[o.symbol].size();
    if(orderSize>0){
        for (iterator=0;iterator < orderSize ; iterator++){
            if(orderbooks[o.symbol][iterator].price==o.price){
                if(orderbooks[o.symbol][iterator].side == o.side){
                    orderbooks[o.symbol].push_back(o);
                }else{
                    orderbooks[o.symbol][iterator].volume - o.volume;
                    if(orderbooks[o.symbol][iterator].volume == 0){
                        orderbooks[o.symbol].erase(orderbooks[o.symbol].begin()+iterator);
                    }
                }
                return;
            }else{
                orderbooks[o.symbol].push_back(o);
                return;
            }
        }
    }else{
        orderbooks[o.symbol].push_back(o);
    }
}

template<typename T>
void GenericReader<T>::Cancel()
{
    orderSize = orderbooks[o.symbol].size();
    for (iterator=0;iterator < orderSize ; iterator++){
        if(orderbooks[o.symbol][iterator].orderId==o.orderId){
            orderbooks[o.symbol].erase(orderbooks[o.symbol].begin()+iterator);
            return;
        }
    }
}

template<typename T>
void GenericReader<T>::Amend()
{
    orderSize = orderbooks[o.symbol].size();
    for (iterator=0;iterator < orderSize ; iterator++){
        if(orderbooks[o.symbol][iterator].orderId==o.orderId){
            orderbooks[o.symbol][iterator].price = o.price;
            orderbooks[o.symbol][iterator].volume = o.volume;
            return ;
        }
    }
}
