//
// Created by Mohammed on 2022-11-30.
//

#ifndef GIS_COMMANDPROCESSOR_H
#define GIS_COMMANDPROCESSOR_H

#include <vector>
#include <string>
#include <Logger.h>
#include <NameIndex.h>

using namespace std;

enum enum_command{
    world,
    quit,
    import,
    what_is,
    what_is_at,
    what_is_in,
    debug,
};

struct DMS{
    float f_longitude1;
    float f_longitude2;
    float f_latitude1;
    float f_latitude2;
};

class CommandProcessor {
public:

    // Constructor and Destructor
    CommandProcessor();
    ~CommandProcessor();

    // Add command to CommandProcessor vector
    void addCommand(vector<string> command);

    // Processes the world command, converts DMS to DD format and stores world coordinates in application
    void WorldCommand(const vector<string>& command);

    void ImportCommand(const vector<string>& command);

    void WhatIsCommand(const vector<string>& command);

    void DebugCommand(const vector<string>& command);

    float DMStoDDConverter(const string& DMS);

    // List all commands in commandList
    void RunCommands(Logger &logger);

    vector<vector<string>> CommandList;
    DMS dms{};
    NameIndex ni;
    Logger logger;
};


#endif //GIS_COMMANDPROCESSOR_H
