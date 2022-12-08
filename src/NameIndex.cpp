//
// Created by Mohammed on 2022-11-30.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include "NameIndex.h"

NameIndex::NameIndex() {
    hf = new SimpleStringHash();
    rf = new QuadraticProbing();
    hashTable = HashTable<vector<string>>(1024, hf, rf);
}

void NameIndex::addToIndex(vector<string> location, string offset) {
    hashTable.insert(location, offset);
}

string NameIndex::str() {
    stringstream ss;
    ss << "\nFormat of display is\n"
          "Slot Number: data record\n"
          "Current table size is " << hashTable.m_buckets.size() <<
          "\nNumber of elements in table is XXXX\n\n";
    for(int i = 0; i < hashTable.m_buckets.size(); i++)
    {
        if(hashTable.m_statuses[i] == 1)
        {
            ss << "\t" << to_string(i) << ": [" << hashTable.m_buckets[i][0] << ":" << hashTable.m_buckets[i][1]
            << ", [" << hashTable.m_offsets[i] << "]]" << endl;
        }
    }
    return ss.str();
}

vector<string> NameIndex::searchIndex(vector<string> location) {
    int result = hashTable.search(location);
    vector<string> vector_data{};
    if(result) {
        string line;
        fstream database;
        database.open("database.txt");
        database.seekg(std::ios::beg);

        // https://stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file
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


