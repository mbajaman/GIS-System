//
// Created by Mohammed on 2022-11-30.
//

#ifndef GIS_NAMEINDEX_H
#define GIS_NAMEINDEX_H

#include <string>
#include <vector>
#include "HashTable.h"

using namespace std;

class NameIndex {
public:

    /**
     * TODO: Can likely use this struct to organize data better but didn't get around to it
     */
    struct index
    {
        string featureName;
        string stateAbv;
        int fileOffset;
    };

    /**
     * Constructor for NameIndex. Instantiates hash function, resolution function and HashTable
     */
    NameIndex();

    /**
     * Adds record to NameIndex (which uses a HashTable implementation
     * @param location GIS Record to add to NameIndex
     * @param offset File offset to store in the NameIndex to fetch full record when searched
     */
    void addToIndex(vector<string> location, string offset);

    /**
     * Searches NameIndex and retrieves full record from database file
     * @param location GIS Record to search
     * @return Returns full record from database
     * Reference: https://stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file
     */
    vector<string> searchIndex(vector<string> location);

    /**
     * @return Returns a nicely formatted string of the NameIndex for debug purposes
     */
    string str();

    HashTable<vector<string>> getHashTable() const { return hashTable; }

private:
    /// HashFunction pointer
    HashFunction<vector<string>> *hf;

    /// ResolutionFunction pointer
    ResolutionFunction *rf;

    /// HashTable initialization
    HashTable<vector<string>> hashTable = HashTable<vector<string>>(1024, hf, rf);
};


#endif //GIS_NAMEINDEX_H
