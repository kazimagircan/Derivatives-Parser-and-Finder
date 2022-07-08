#include<iostream>
#include<fstream>
#include <algorithm>
using namespace std;
#define SPLITLISTSIZE 7

//define reader class
class Reader{
    private:
        fstream source;
        string strings[SPLITLISTSIZE];
        string sourcePath;
        map<string, vector<tmTimestampOrder>> orderbooks;
        vector<tmTimestampOrder> orderList;
        string line;
        string substring;
        tmTimestampOrder o;
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

    public:
        Reader(string sourcePath);
        ~Reader();

        void ReadSource();
        map<string, vector<tmTimestampOrder>> GetOrderbook();

};

Reader::Reader(string sourcePath)
{
    this->sourcePath = sourcePath;
    this->seperator=';';
}

Reader::~Reader()
{
    if(this->source.is_open()){
       this->source.close();
    }
}

void Reader::SplitText()
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
    istringstream ts("20:02:2020 " + strings[0]);
    ts >> get_time(&o.timestamp, "%d:%m:%Y %H:%M:%S");
    o.symbol=strings[1];
    o.orderId=stoi(strings[2]);
    o.operation=strings[3][0];
    o.side=strings[4];
    o.volume=stoi(strings[5]);
    o.price=stod(strings[6]);

}

void Reader::ReadSource()
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

map<string, vector<tmTimestampOrder>> Reader::GetOrderbook()
{
    return this->orderbooks;
}

void Reader::ReadOrder()
{
    if(this->o.operation =='I')
        this->Insert();
    else if(this->o.operation =='C')
        this->Cancel();
    else
        this->Amend();

}

void Reader::Insert()
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

void Reader::Cancel()
{
    orderSize = orderbooks[o.symbol].size();
    for (iterator=0;iterator < orderSize ; iterator++){
        if(orderbooks[o.symbol][iterator].orderId==o.orderId){
            orderbooks[o.symbol].erase(orderbooks[o.symbol].begin()+iterator);
            return;
        }
    }
}

void Reader::Amend()
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
