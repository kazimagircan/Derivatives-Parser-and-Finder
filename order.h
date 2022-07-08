#include<iostream>
using namespace std;


//define order struct type variable with timestamp as string
struct stringTimestampOrder{
    string timestamp;
    string symbol;
    int orderId;
    char operation;
    string side;
    int volume;
    double price;
    };

//define order struct type variable with timestamp as tm struct
struct tmTimestampOrder{
    tm timestamp;
    string symbol;
    int orderId;
    char operation;
    string side;
    int volume;
    double price;
    };
