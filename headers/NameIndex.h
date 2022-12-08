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

    struct index
    {
        string featureName;
        string stateAbv;
        int fileOffset;
    };

    NameIndex();

    void addToIndex(vector<string> location, string offset);

    vector<string> searchIndex(vector<string> location);

    string str();

    HashFunction<vector<string>> *hf;

    ResolutionFunction *rf;

    HashTable<vector<string>> hashTable = HashTable<vector<string>>(1024, hf, rf);
};


#endif //GIS_NAMEINDEX_H
