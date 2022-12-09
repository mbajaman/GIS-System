//
// Created by Mohammed on 2022-11-30.
//

#include "BufferPool.h"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

BufferPool::BufferPool() {
    map.clear();
    dq.clear();
    cache_size = 15;
}

void BufferPool::insert(string offset, vector<string> record) {
    stringstream ss;
    for(auto & i : record){
        ss << i << "|";
    }

    // If not in cache
    if (map.find(offset) == map.end()){
        // If cache is full, remove last element
        if (cache_size == dq.size())
        {
            string last = dq.back();
            dq.pop_back();
            map.erase(last);
        }
    }
    else {
        // If its in cache, remove it from map and deque
        auto it = dq.begin();
        while (*it != offset)
        {
            it++;
        }
        dq.erase(it);
        map.erase(offset);
    }

    // Otherwise add to cache
    dq.push_front(offset);
    map[offset] = ss.str();
}

string BufferPool::str() {
    stringstream ss;

    ss << "MRU" << endl;
    for(auto it = dq.begin(); it != dq.end(); it++)
    {
        ss << "\t" <<  it->c_str() << ": " << map[it->c_str()] << endl;
    }
    ss << "LRU" << endl;
    return ss.str();
}
