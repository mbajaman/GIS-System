//
// Created by Mohammed on 2022-11-30.
//

#ifndef GIS_COORDINATEINDEX_H
#define GIS_COORDINATEINDEX_H

#include <string>
#include <vector>

using namespace std;
class CoordinateIndex {
    CoordinateIndex();

    void addToIndex(vector<string> location, string offset);

    vector<string> searchIndex(vector<string> location);

    string str();



};


#endif //GIS_COORDINATEINDEX_H
