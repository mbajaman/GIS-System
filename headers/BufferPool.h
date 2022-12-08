//
// Created by Mohammed on 2022-11-30.
//

#ifndef GIS_BUFFERPOOL_H
#define GIS_BUFFERPOOL_H

#include <deque>
#include <map>
#include "../headers/GISRecord.h"

using namespace std;

class BufferPool {
public:

    BufferPool(int size);

    GISRecord get(int key);

    int cache_size;
    map<int,GISRecord> map;
    deque<int> dq;
};


#endif //GIS_BUFFERPOOL_H
