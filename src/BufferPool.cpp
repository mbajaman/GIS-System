//
// Created by Mohammed on 2022-11-30.
//

#include "BufferPool.h"

BufferPool::BufferPool(int size) {
    map.clear();
    dq.clear();
    cache_size = size;
}
