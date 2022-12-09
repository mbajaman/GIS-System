//
// Created by Mohammed on 2022-11-30.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <utility>
#include "NameIndex.h"

NameIndex::NameIndex() {
    hf = new ElfStringHash();
    rf = new QuadraticProbing();
    hashTable = HashTable<vector<string>>(1024, hf, rf);
}

void NameIndex::addToIndex(const vector<string>& location, string offset) {
    hashTable.insert(location, std::move(offset));
}

vector<string> NameIndex::searchIndex(const vector<string>& location) {
    int result = hashTable.search(location);
    vector<string> vector_data{};
    if(result) {
        string line;
        fstream database;
        database.open("database.txt");
        database.seekg(std::ios::beg);

        // Iterate through file till you get to the line with the full GIS record
        for (int i = 0; i < result - 1; i++) {
            database.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        while (getline(database, line)) {
            istringstream iss(line);
            string token;
            while (getline(iss, token, '|'))
                vector_data.push_back(token);
            break;
        }
        vector_data.push_back(to_string(result));
    }
    return vector_data;
}

string NameIndex::str() const {
    stringstream ss;
    ss << "\nFormat of display is\n"
          "Slot Number: data record\n"
          "Current table size is " << hashTable.getBucketSize() <<
       "\nNumber of elements in table is " << hashTable.getNumOccupied() << "\n\n";

    // Create containers for output
    vector<vector<string>> myHashTable = hashTable.getBuckets();
    vector<string> myOffsets = hashTable.getOffsets();

    for(int i = 0; i < hashTable.getBucketSize(); i++)
    {
        // If occupied...
        if(hashTable.getBucketStatus()[i] == 1)
        {
            ss << "\t" << to_string(i) << ": [" << myHashTable[i][0] << ":" << myHashTable[i][1]
               << ", [" << myOffsets[i] << "]]" << endl;
        }
    }

    // Cleanup temporary containers
    myHashTable.clear();
    myOffsets.clear();
    return ss.str();
}



