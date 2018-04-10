#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string.h>
#include <string>
#include <algorithm>
#include "Tlb.h"
#include "PageTable.h"
using namespace std;
const int FRAME_SIZE = 256; 
const int PAGE_TABLE_SIZE = 256;
const int PHYSICAL_MEM_SIZE = 256;
const int TLB_SIZE = 16;

int main() { 
    string line;
    ifstream myfile ("addresses.txt");
    ofstream outputFile;
    outputFile.open("output.txt");
    FILE * pFile;
    pFile = fopen ( "BACKING_STORE.bin" , "r" );
    ifstream backingStoreBin("BACKING_STORE.bin", std::ios::binary);
    Tlb tlb(TLB_SIZE);
    PageTable pageTable;
    vector<char*> physicalMem;
    vector<char> backingStore((std::istreambuf_iterator<char>(backingStoreBin)), //same as fread fscan combo (for testing)
         (std::istreambuf_iterator<char>()));
    vector<int> inputTable;
    int firstUsedIndex= 0;

    if (myfile.is_open())
    {
        for (int i= 0; getline (myfile,line); i++)
        {
            char * cstr = new char [line.length()+1];
            strcpy(cstr, line.c_str());

            // cstr now contains a c-string copy of str
            char * p = strtok (cstr," ");
            for (int j = 0; p!=0; j++)
            {
                inputTable.push_back(stoi(string(p)));
                //cout << p << '\n';
                p = strtok(NULL," ");
            }
        }
        cout << "Input Table size:" << inputTable.size()<< endl;
        myfile.close();
    }
    else{
        cout << "Unable to open file"; 
    }
    // cout<< "Page Table size: " << backingStore.size() << endl;
    //cout<< "Last value in page table: " << int(backingStore.back()) << endl; //65535
    //  cout<< "Number of pages in page table: " << pageTable.size() << endl;


    pageTable.frameNumber.fill(-1);
    //parse inputTable and get page number and offset from the number
    for(int i = 0; i< inputTable.size(); i++){
        int pageNum = inputTable.at(i)>>8; //bit shift
        unsigned offset = inputTable.at(i) & 0xFF; //bit masking
        //check TLB for that frame number
        //...
        int corFrame = -1;
        corFrame = tlb.lookForFrame(pageNum);
        if(corFrame == -1){ //means that it was not found in the TLB, check Page Table now
            if(pageTable.frameNumber[pageNum] != -1){
                corFrame = pageTable.frameNumber[pageNum];
                cout<< "Found frame number in Page Table."<< endl;
            }
        }else{
            cout<< "Found frame number in TLB." << endl;
        }
        if(corFrame == -1){ //means that the frame number was not found in TLB and Page Table
            // go to bin and load the frame into physical mem and update TLB and Page Table
            if (!ferror(pFile)) {
                fseek ( pFile , FRAME_SIZE * pageNum, SEEK_SET ); //FRAME_SIZE * pageNum
                char* str = new char[FRAME_SIZE];
                fread(str, sizeof(char), FRAME_SIZE, pFile);
                // const char temp[FRAME_SIZE] = str;
                if(physicalMem.size() != PHYSICAL_MEM_SIZE){
                    // cout<< int(str[offset]) << endl; //testing if the numbers are working properly
                    // cout<< int(backingStore[FRAME_SIZE* pageNum+offset])<< endl;
                    cout<< "Found frame number in Hard Drive." <<endl;
                    //push into physical mem
                    physicalMem.push_back(str);
                    corFrame = int(physicalMem.size()-1);
                    //update page table
                    pageTable.frameNumber[pageNum] = corFrame;
                    //update TLB
                    tlbTableRow* row = new tlbTableRow;
                    row->pageNumber = pageNum;
                    row->frameNubmer = corFrame;
                    tlb.add(row);
                }else{
                    //if physical mem is full
                    cout<< "Found frame number in Hard Drive." <<endl;
                    //push into physical mem
                    physicalMem.at(firstUsedIndex) = str;
                    corFrame = firstUsedIndex;
                    firstUsedIndex++;
                    //update page table
                    pageTable.frameNumber[pageNum] = corFrame;
                    //update TLB
                    tlbTableRow* row = new tlbTableRow;
                    row->pageNumber = pageNum;
                    row->frameNubmer = corFrame;
                    tlb.add(row);
                }
            }else{
                //error
                cout<< "There was an error reading the bin file"<< endl;
            }
        }

        //read physical mem
        int memOutput = int(physicalMem.at(corFrame)[offset]);
        
        cout<< "Virtual Address: " << inputTable.at(i) << endl;
        cout<< "Page Number: " << pageNum <<endl;
        cout<< "Offset: "<< offset <<endl;
        cout<< "Physical Memory Address: " << FRAME_SIZE* corFrame + offset << endl;
        cout<< "Physical Memory Output: " << memOutput<< endl <<endl;

        outputFile << "Virtual Address: " << inputTable.at(i) << "\t";
        outputFile<< "Physical Memory Address: " << FRAME_SIZE* corFrame + offset << "\t";
        outputFile<< "Physical Memory Output: " << memOutput<< endl;
    }
    
    pclose(pFile);
    outputFile.close();
    getchar();
    return 0;
}
