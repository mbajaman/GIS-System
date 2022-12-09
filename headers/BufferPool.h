//
// Created by Mohammed on 2022-11-30.
//

#ifndef GIS_BUFFERPOOL_H
#define GIS_BUFFERPOOL_H

#include <deque>
#include <map>
#include <string>
#include <vector>

using namespace std;

/**
 * LRU Cache using two data structures
 * Reference: https://leetcode.com/problems/lru-cache/solutions/355310/lru-cache-simple-c-code-with-clear-comments/
 */
class BufferPool {
public:
    /**
     * Constructor that sets cache size and clear the map and queue
     */
    BufferPool();

    /**
     * Takes an offset and record entry and stores it in the Buffer pool
     * @param offset File offset in the database file
     * @param record Full GIS record
     */
    void insert(string offset, vector<string> record);

    /**
     * @return Returns a nicely formatted string with contents of the buffer pool
     */
    string str();

private:
    /// Buffer pool cache size
    int cache_size;

    /// Map for storing offset and GIS record
    map<string,string> map;

    /// Stores offsets in a queue to keep correct MRU and LRU order
    deque<string> dq;
};


#endif //GIS_BUFFERPOOL_H
