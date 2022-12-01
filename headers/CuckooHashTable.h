#ifndef CUCKOO_HASH_TABLE_H
#define CUCKOO_HASH_TABLE_H

#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <sstream>

using namespace std;


/**
 * Uniform Random Number generator class, using C++ Mersenne Twister and using a 48-bit linear congruential generator.
 */
class UniformRandom {
public:
    explicit UniformRandom(long long initialValue = currentTimeMicroSeconds()) {
        state = initialValue & MASK;
    }

    long long nextLong() {
        return ((static_cast<long long>( next(32))) << 32) + next(32);
    }

    int nextInt() {
        return next(32);
    }

    int nextInt(int high) {
        return static_cast<int>( abs(nextLong() % high));
    }

private:
    long long state;

    static const long long A = 25214903917LL;
    static const long long C = 11;
    static const long long M = (1LL << 48);
    static const long long MASK = M - 1;

    static long long currentTimeMicroSeconds() {
        auto now = chrono::high_resolution_clock::now().time_since_epoch();
        return (chrono::duration_cast<chrono::microseconds>(now)).count();
    }

    /**
     * Return specified number of random bits
     */
    int next(int bits) {
        state = (A * state + C) & MASK;
        return state >> (48 - bits);
    }
};

/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
bool isPrime(int n) {
    if (n == 2 || n == 3)
        return true;

    if (n == 1 || n % 2 == 0)
        return false;

    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;

    return true;
}

/**
 * Internal method to return a prime number at least as large as n.
 * Assumes n > 0.
 */
int nextPrime(int n) {
    if (n % 2 == 0)
        ++n;

    for (; !isPrime(n); n += 2);

    return n;
}

/**
 * A hash routine for string objects.
 */
int hashCode(const string &key) {
    int hashVal = 0;

    for (auto ch: key)
        hashVal = 37 * hashVal + ch;

    return hashVal;
}

/**
 * A hash routine for ints.
 */
int hashCode(int key) {
    return key;
}

template<typename T>
class CuckooHashFamily {
public:
    size_t hash(const T &x, int which) const;

    int getNumberOfFunctions();

    void generateNewFunctions();
};

template<int count>
class StringHashFamily {
public:
    StringHashFamily() : MULTIPLIERS(count) {
        generateNewFunctions();
    }

    int getNumberOfFunctions() const {
        return count;
    }

    void generateNewFunctions() {
        for (auto &mult: MULTIPLIERS)
            mult = r.nextInt();
    }

    size_t hash(const string &x, int which) const {
        // TODO number 1
        // Implement this function to take the "which"th value from MULTIPLIERS and multiply each character
        //  in string x to that multiplier value adding it up with hashVal

        size_t hashVal = 0;

        // EDIT: Iterate through each character and multiply each character with "which"th value
        for (char c : x)
        {
            hashVal += c * which;
        }

        return hashVal;
    }

private:
    vector<int> MULTIPLIERS;
    UniformRandom r;
};


#define MAX_LOAD 0.40  /* g++ 4.6 does not support static const double */

// CuckooHashing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hashCode( string str ) --> Global method to hash strings

template<typename T, typename HashFamily>
class HashTable {
public:
    explicit HashTable(int size = 101) : array(nextPrime(size)) {
        numHashFunctions = hashFunctions.getNumberOfFunctions();
        rehashes = 0;
        currentSize = 0;
        makeEmpty();
    }

    bool contains(const T &x) const {
        return findPos(x) != -1;
    }

    void makeEmpty() {
        currentSize = 0;
        for (auto &entry: array)
            entry.isActive = false;
    }

    bool insert(T &&x) {
        // TODO number 2
        //Make sure you dont insert stuff that are already in the table and make sure the table is not too crowded.
        if (contains(x)) return false; // EDIT: If already in table, don't insert
        if (currentSize >= array.size() * MAX_LOAD) expand(); // EDIT: If table too crowded, expand it.

        const int COUNT_LIMIT = 100;
        // in a while true loop for numHashFunctions times try to find the hash value of the item x
        // and place it into the array object. If you went over all the numHashFunctions different hash values,
        // evict one of the numHashFunctions possible items and place in x, now try to place in the evicted item.
        // you can allow this eviction process to reoccur for COUNT_LIMIT number of times, if you didn't get out by then
        // you know what to do ...
        while (true)
        {
            int lastPos = -1;
            int pos;
            for (int count = 0; count < COUNT_LIMIT; count++)
            {
                for (int i = 0; i < numHashFunctions; i++)
                {
                    pos = myhash(x, i);

                    if (!isActive(pos))
                    {
                        array[pos] = std::move(HashEntry{ std::move(x), true});
                        currentSize++;
                        return true;
                    }
                }

                // If no spots available, evict a random one
                int i = 0;
                do {
                    pos = myhash(x, r.nextInt(numHashFunctions));
                } while (pos == lastPos && i++ < 5);

                lastPos = pos;
                std::swap(x, array[pos].element);
            }

            if(rehashes++ > ALLOWED_REHASHES)
            {
                expand();
                rehashes = 0;
            }
            else
            {
                rehash();
            }
        }
        // return false; Base code
    }

    int size() const {
        return currentSize;
    }

    int capacity() const {
        return array.size();
    }

    bool remove(const T &x) {
        int currentPos = findPos(x);
        if (!isActive(currentPos))
            return false;

        array[currentPos].isActive = false;
        return true;
    }

    string print() {
        std::ostringstream os;
        os << "Contents of Cuckoo hash table:" << endl;
        for (auto & content : array) {
            if (content.isActive)
                os << " | " << content.element;
            else
                os << " | " << "[]";
        }
        os << endl;
        return os.str();
    }

private:

    struct HashEntry {
        T element;
        bool isActive;
        HashEntry(const T &e = T(), bool a = false): element{e}, isActive{a} {}
        HashEntry(T &&e, bool a = false): element{std::move(e)}, isActive{a} {}
    };

    vector<HashEntry> array;
    int currentSize;
    int numHashFunctions;
    int rehashes;
    HashFamily hashFunctions;
    UniformRandom r;

    //  static const double MAX_LOAD = 0.40;  // Not supported in g++ 4.6
    static const int ALLOWED_REHASHES = 5;

    bool isActive(int currentPos) const { return currentPos != -1 && array[currentPos].isActive; }

    // Method that search all hash function places
    int findPos(const T &x) const {
        for (int i = 0; i < numHashFunctions; ++i) {
            int pos = myhash(x, i);

            if (isActive(pos) && array[pos].element == x)
                return pos;
        }

        return -1;
    }

    void expand() {
        rehash(static_cast<int>( array.size() / MAX_LOAD ));
    }

    void rehash() {
        hashFunctions.generateNewFunctions();
        rehash(array.size());
    }

    void rehash(int newSize) {
        vector<HashEntry> oldArray = array;

        // Create new double-sized, empty table
        array.resize(nextPrime(newSize));
        for (auto &entry: array)
            entry.isActive = false;

        // Copy table over
        currentSize = 0;
        for (auto &entry: oldArray)
            if (entry.isActive)
                insert(std::move(entry.element));
    }


    size_t myhash(const T &x, int which) const {
        return hashFunctions.hash(x, which) % array.size();
    }
};


#endif
