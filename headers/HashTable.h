//
// Created by Mohammed on 2022-12-07.
// Used hashtable-solution.cc from lab code from course COMP 8042 to implement this HashTable
//

#ifndef GIS_HASHTABLE_H
#define GIS_HASHTABLE_H


#include <vector>
#include <string>

using namespace std;

template <typename K>
/**
 * Parent class for HashFunction
 */
class HashFunction {
public:
    virtual unsigned int operator()(const K& key) const = 0;
    virtual ~HashFunction() = default;
};

/**
 * Computes hash for entry into hash table (uses elfhash)
 * Reference: https://www.programmingalgorithms.com/algorithm/elf-hash/cpp/
 */
class ElfStringHash : public HashFunction<vector<string>> {
public:
    // Computes an elfhash value for a string
    unsigned int operator()(const vector<string>& s) const {
        unsigned int hash = 0;
        unsigned int x;
        for (const auto & i : s) {
            unsigned int len = i.length();
            for(int j = 0; j < len; j++)
            {
                hash = (hash << 4) + (i[j]);
                if ((x = hash & 0xF0000000) != 0)
                {
                    hash ^= (x >> 24);
                }
                hash &= ~x;
            }
        }
        return hash;
    }
};

/**
 * Parent class ResolutionFunction
 */
class ResolutionFunction {
public:
    virtual unsigned int operator()(int i) const = 0;
    virtual ~ResolutionFunction() = default;
};

/**
 * Probing sequence. Follows [(n^2) + n / 2]
 */
class QuadraticProbing : public ResolutionFunction {
public:
    unsigned int operator()(int i) const {
        return (i * i) + i / 2;
    }
};

/**
 *
 * @tparam K Specify template type for HashTable, in this case it's vector<string>
 */
template <typename K>
class HashTable {
public:
    // Enum for storing statuses of buckets
    enum BucketStatus {EMPTY, OCCUPIED, DELETED};

    /**
     *
     * @param n Number of collisions
     * @param _h The hash function
     * @param _f The resolution function if a collision occurs
     */
    HashTable(int n, HashFunction<K>* _h, ResolutionFunction* _f) : numCollisions(0), hash(_h), f(_f), occupied(0){
        int c = n;
        m_buckets.resize(c);
        m_statuses.resize(c);
        m_offsets.resize(c);
    };

    /**
     * Inserts key into hashtable and offset in a separate vector
     * @param key Key/Record to be stored into HashTable
     * @param offset File offset fo record
     */
    void insert(const K& key, string offset) {

        // Expand and Rehash if HashTable load factor is more than 70%
        if(occupied >= m_buckets.size() * 0.7){
            occupied = 0;
            expandAndRehash();
        }

        int tableSize = m_buckets.size();
        unsigned int h = hash->operator()(key) % tableSize;
        unsigned int firstH = h;
        while (m_statuses[h] == OCCUPIED){
            ++numCollisions; // Record how many collisions occur
            h = (h + hash->operator()(key)) % tableSize; // Compute next hash
            if (h == firstH)
                return;
        }

        // Populate HashTable bucket, statuses and offset information
        m_buckets[h] = key;
        m_offsets[h] = offset;
        m_statuses[h] = OCCUPIED;

        // Track how many buckets have been occupied
        occupied++;
    }

    /**
     * Searches and returns file offset of record in HashTable
     * @param key Enter key/record to search
     * @return Returns file offset to fetch full record file from database
     */
    int search(const K& key) {
        int tableSize = m_buckets.size();
        unsigned int h = hash->operator()(key) % tableSize;
        unsigned int firstH = h;
        while (m_statuses[h] != EMPTY){
            if (m_statuses[h] == OCCUPIED && m_buckets[h] == key)
                return stoi(m_offsets[h]); // Convert offset (string) to int
            h = (h + hash->operator()(key)) % tableSize;
            if (h == firstH)
                return 0;
        }
        // Key wasn't found in the hash table.
        return 0;
    }

    /**
     * Expands and Rehashes the HashTable if more than 70% is occupied
     */
    void expandAndRehash(){
        int i ;
        for(i = 0; i < sizeof(goodSquareNumbers); i++)
        {
            if(m_buckets.size() == goodSquareNumbers[i]) {
                i++;
                break;
            }
        }

        int expandSize = goodSquareNumbers[i];

        // Create temporary storage
        vector<K> tempBucket;
        vector<BucketStatus> tempStatus;
        vector<string> tempOffsets;

        // Create variable to store old HashTable content
        vector<K> oldBucket;
        vector<BucketStatus> oldStatus;
        vector<string> oldOffsets;

        // Store old HashTable
        oldBucket = m_buckets;
        oldStatus = m_statuses;
        oldOffsets = m_offsets;

        // Create new hashtable with new size using the temporary containers
        tempBucket.resize(expandSize);
        tempStatus.resize(expandSize);
        tempOffsets.resize(expandSize);

        // Reinsert newly created hashtable back into member variables
        m_buckets = tempBucket;
        m_statuses = tempStatus;
        m_offsets = tempOffsets;

        // Rehash and re-insert into new hashtable iterating through old HashTable
        for(i = 0; i < oldBucket.size(); i++)
        {
            if(oldStatus[i] == OCCUPIED)
            {
                insert(oldBucket[i], oldOffsets[i]);
            }
        }

        // Clean up temporary containers
        tempBucket.clear();
        tempStatus.clear();
        tempOffsets.clear();
    }

    /**
     * @return Returns total number of collisions that occured in the HashTable
     */
    int getNumCollisions() const { return numCollisions; }

    /**
     * @return Returns total number of occupied buckets in HashTable
     */
    int getNumOccupied() const { return occupied; }

    /**
     * @return Returns HashTable size
     */
    int getBucketSize() const { return m_buckets.size(); }

    /**
     * @return Returns HashTable bucket
     */
    vector<K> getBuckets() const { return m_buckets; }

    /**
     * @return Returns statuses vector which states if a bucket is occupied or not
     */
    vector<BucketStatus> getBucketStatus() const { return m_statuses; }

    /**
     * @return Returns offsets vector which states where the record is in the database file
     */
    vector<string> getOffsets() const { return m_offsets; }

private:
    /// The total number of collisions since the creation of the hashfunction.
    /// This includes the collisions encountered during insertions, searches, and deletions
    int numCollisions;

    /// Count of occupied buckets in the HashTable
    int occupied;

    /// All the buckets in the hashtable
    std::vector<K> m_buckets;

    /// Status of all the buckets
    std::vector<BucketStatus> m_statuses;

    /// Offset of records that are in the HashTable
    std::vector<string> m_offsets;

    /// The hash function
    HashFunction<K>* hash;

    /// The collision resolution function
    ResolutionFunction* f;

    /// Good prime numbers to use as Hashtable sizes
    int goodSquareNumbers[26] = {1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144,
                               24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469,
                               524288, 1048576, 2097152, 4194304, 8388608, 16777216,
                               33554432, 67108864
    };
};



#endif //GIS_HASHTABLE_H
