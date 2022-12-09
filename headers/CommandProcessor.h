//
// Created by Mohammed on 2022-11-30.
//

#ifndef GIS_COMMANDPROCESSOR_H
#define GIS_COMMANDPROCESSOR_H

#include <vector>
#include <string>
#include <Logger.h>
#include <NameIndex.h>
#include <BufferPool.h>

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

    // Add command to CommandProcessor vector
    /**
     * Adds a command to a Command list to later process in RunCommands
     * @param commandCommand to add to command list
     */
    void addCommand(const vector<string>& command);

    // Processes the world command, converts DMS to DD format and stores world coordinates in application
    /**
     * Processes the world command from the script file and sets world boundaries
     * @param command The world command to process
     */
    void WorldCommand(const vector<string>& command);

    /**
     * Processes an import command. Populates database with valid records based on import file
     * @param command The import command to process
     */
    void ImportCommand(const vector<string>& command);

    /**
     * Processes a what_is command and searches the NameIndex
     * @param command The what_is command to process
     */
    void WhatIsCommand(const vector<string>& command);

    /**
     * Not Implemented
     * @param command The what_is_at command to process
     */
    void WhatIsAtCommand(const vector<string>& command);

    /**
     * Not Implemented
     * @param command The what_is_at command to process
     */
    void WhatIsInCommand(const vector<string>& command);

    /**
     * Processs the debug based on command argument (pool, hash, quad)
     * @param command The debug command to process
     */
    void DebugCommand(const vector<string>& command);

    /**
     * Converts DMS to Degree Decimals
     * @param DMS Container for Degrees, Minutes and Seconds data which are to be converted
     * @return Converted degree decimal value
     */
    static float DMStoDDConverter(const string& DMS);

    /**
     * Executes all commands from script file.
     * @param logger Pass logger instance from GIS/main file
     */
    void RunCommands(Logger &logger);

private:
    vector<vector<string>> CommandList;
    DMS dms{};
    NameIndex ni;
    Logger logger;
    BufferPool bufferPool;
};


#endif //GIS_COMMANDPROCESSOR_H
