#ifndef PAGETABLE_H
#define PAGETABLE_H
#include <array>
#include <algorithm>

class PageTable{
    public:
        std::array<int, 256> frameNumber;
};

#endif