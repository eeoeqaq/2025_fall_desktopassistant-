#pragma once
#include<string>
#include <iostream>
#include <fstream>
using namespace std;


enum idetail_type{
        salary,
        interest,
    };

enum odetail_type{
        food,
        clothes,
        entertainment
    };
    
class recordings{
    public:
    virtual void write();
    virtual void read();
    recordings() = default;
    //recordings(string new_name,long long new_amount);
    private:
    string name;
    long long amount;
};

class irecordings:public recordings{
    public:
    void write();
    void read();
    irecordings(string new_name,long long new_amount,idetail_type type);
    private:
    idetail_type type1;
};

class orecordings:public recordings{
    public:
    void write();
    void read();   
    orecordings(string new_name,long long new_amount,odetail_type type);
    private:
    odetail_type type1;
};

