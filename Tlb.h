#ifndef H_TLB
#define H_TLB
#include <bits/stdc++.h>
using namespace std;


struct tlbTableRow{
    int pageNumber;
    int frameNubmer;
};

class Tlb
{
    // store keys of cache
    list<tlbTableRow> tlbList;
 
    // store references of key in cache
    unordered_map<int, list<tlbTableRow>::iterator> ma; //page Number and list itterator
    int csize; //maximum capacity of cache
 
public:
    Tlb(int);
    int lookForFrame(int);
    void add(tlbTableRow*);
    void printTLB();
};

#endif


