//
// Created by Mohammed on 2022-11-30.
//

#ifndef GIS_LOGGER_H
#define GIS_LOGGER_H

#include <string>
#include <vector>

using namespace std;

class Logger {
public:
    Logger();

    Logger(string dbFile, string script, string logFile);

    void addLine(string line);

    void addText(string text);

    void addLine(vector<string> line);

    void addLineBreak();

    ofstream logfile;
    string logs;
};


#endif //GIS_LOGGER_H
