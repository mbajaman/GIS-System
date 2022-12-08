//
// Created by Mohammed on 2022-12-07.
//

#ifndef GIS_HASHTABLE_H
#define GIS_HASHTABLE_H


#include <vector>
#include <string>

using namespace std;


template <typename K>
class HashFunction {
public:
    virtual unsigned int operator()(const K& key) const = 0;
    virtual ~HashFunction() = default;
};

class SimpleStringHash : public HashFunction<vector<string>> {
public:
    // Computes a simple hash value for a string
    unsigned int operator()(const vector<string>& s) const {
        unsigned int hash = 0;
        for (int i = 0; i < s.size(); i++) {
            for(int j = 0; j < s[i].size(); j++)
                hash += s[i][j];
        }
        return hash;
    }
};

/// An abstract collision resolution function
class ResolutionFunction {
public:
    virtual unsigned int operator()(int i) const = 0;
    virtual ~ResolutionFunction() = default;
};

/// The instructor did the hard part for you and implemented the two resolution functions. Phew!
class LinearProbing : public ResolutionFunction {
public:
    unsigned int operator()(int i) const {
        return i;
    }
};

class QuadraticProbing : public ResolutionFunction {
public:
    unsigned int operator()(int i) const {
        return i * i;
    }
};

template <typename K>
class HashTable {
public:
    enum BucketStatus {EMPTY, OCCUPIED, DELETED};

    HashTable(int n, HashFunction<K>* _h, ResolutionFunction* _f) : numCollisions(0), hash(_h), f(_f){
        int c = n;
        m_buckets.resize(c);
        m_statuses.resize(c);
        m_offsets.resize(c);
    };

    void insert(const K& key, string offset) {
        int tableSize = m_buckets.size();
        unsigned int h = hash->operator()(key) % tableSize;
        unsigned int firstH = h;
        while (m_statuses[h] == OCCUPIED){
            ++numCollisions;
            h = (h + hash->operator()(key)) % tableSize;
            if (h == firstH)
                return;
        }
        m_buckets[h] = key;
        m_offsets[h] = offset;
        m_statuses[h] = OCCUPIED;
    }

    int search(const K& key) {
        int tableSize = m_buckets.size();
        unsigned int h = hash->operator()(key) % tableSize;
        unsigned int firstH = h;
        while (m_statuses[h] != EMPTY){
            if (m_statuses[h] == OCCUPIED && m_buckets[h] == key)
                return stoi(m_offsets[h]);
            h = (h + hash->operator()(key)) % tableSize;
            if (h == firstH)
                return 0;
        }
        // Key wasn't found in the hash table.
        return 0;
    }

    bool erase(const K& key) {
        int tableSize = m_buckets.size();
        unsigned int h = hash->operator()(key) % tableSize;
        unsigned int firstH = h;
        while (m_statuses[h] != EMPTY){
            if (m_buckets[h] == key){
                m_statuses[h] = DELETED;
                return true;
            }
            h = (h + hash->operator()(key)) % tableSize;
            if (h == firstH)
                return false;
        }
        return false;
    }

    int getNumCollisions() const { return numCollisions; }

    /// The total number of collisions since the creation of the hashfunction.
    /// This includes the collisions encountered during insertions, searches,
    /// and deletions
    int numCollisions;

    /// All the buckets in the hashtable
    std::vector<K> m_buckets;

    /// Status of all the buckets
    std::vector<BucketStatus> m_statuses;

    /// Offset of records
    std::vector<string> m_offsets;

    /// The hash function
    HashFunction<K>* hash;

    /// The collision resolution function
    ResolutionFunction* f;
};



#endif //GIS_HASHTABLE_H
