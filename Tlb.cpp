/* We can use stl container list as a double
   ended queue to store the cache keys, with
   the descending time of reference from front
   to back and a set container to check presence
   of a key. But to fetch the address of the key
   in the list using find(), it takes O(N) time.
    This can be optimized by storing a reference
    (iterator) to each key in a hash map. */
#include "Tlb.h"

 
Tlb::Tlb(int n)
{
    csize = n;
}
 
int Tlb::lookForFrame(int pageNum) 
{
    auto it = ma.find(pageNum);
    if(it != ma.end()){
        //we found the reference
        return (*it).second->frameNubmer;
    }else{
        return -1; //did not find the frame
    }



    // // not present in cache
    // if (ma.find(pageNum) == ma.end())
    // {
    //     // cache is full
    //     if (tlbList.size() == csize)
    //     {
    //         //delete least recently used element
    //         tlbTableRow last = tlbList.back();
    //         tlbList.pop_back();
    //         ma.erase(last.pageNumber);
    //     }
    // }
 
    // // present in cache
    // else
    //     tlbList.erase(ma[pageNum]); //erase the reference 
 
    // // update reference
    // tlbList.push_front(pageNum);
    // ma[x] = tlbList.begin();
}
 
void Tlb::add(tlbTableRow* row){
    //deleting first element (FIFO)
    if(tlbList.size() == csize){ //if tlb is full
        tlbTableRow first = tlbList.front();
        tlbList.pop_front();
        ma.erase(first.pageNumber);
        //updating reference
        tlbList.push_back(*row); 
        ma[row->pageNumber] = --tlbList.end(); //maps the page Number to the last itterator in the list
    }else{
        tlbList.push_back(*row); 
        ma[row->pageNumber] = --tlbList.end(); //maps the page Number to the last itterator in the list
    }
}

// display contents of cache
void Tlb::printTLB()
{
    for (auto it = tlbList.begin(); it != tlbList.end(); ++it){
        cout << "Page Number: " << (*it).pageNumber << ", Frame Number: " << (*it).frameNubmer << " ";
    }
    cout << endl;
}
 
// Driver program to test above functions
// int main()
// {
//     LRUCache ca(4);
 
//     ca.refer(1);
//     ca.refer(2);
//     ca.refer(3);
//     ca.refer(1);
//     ca.refer(4);
//     ca.refer(5);
//     ca.display();
 
//     return 0;
// }
// This code is contributed by Satish Srinivas