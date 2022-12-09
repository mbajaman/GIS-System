//
// Created by Mohammed on 2022-11-30.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <Logger.h>
#include <stdlib.h>
#include "CommandProcessor.h"

void CommandProcessor::addCommand(const vector<string>& command) {
    CommandList.push_back(command);
}

void CommandProcessor::RunCommands(Logger &loggerInstance) {
    CommandProcessor::logger = loggerInstance;
    logger.startLogger();
    // Create a map to get enum from string
    std::map<std::string, enum_command> commandMap = {
            {"world", enum_command::world},
            {"quit", enum_command::quit},
            {"import", enum_command::import},
            {"what_is", enum_command::what_is},
            {"what_is_at", enum_command::what_is_at},
            {"what_is_in", enum_command::what_is_in},
            {"debug", enum_command::debug}
    };

    // Count number of commands - this is to display command number in log file
    int commandsCount = 0;

    // Iterate through CommandList and process commands
    for(auto & i : CommandList)
    {
        if(i[0][0] == ';')
        {
            logger.addLine(i);
            continue;
        }

        switch(commandMap.find(i[0])->second){
            case world:
                logger.addLine(i);
                WorldCommand(i);
                break;
            case quit:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(i);
                logger.addLine("Terminating execution of commands.");
                logger.addLineBreak();
                logger.endLogger();
                exit(0);
                break;
            case import:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(i);
                ImportCommand(i);
                break;
            case what_is:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(i);
                WhatIsCommand(i);
                break;
            case what_is_at:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(i);
                WhatIsAtCommand(i);
                break;
            case what_is_in:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(i);
                WhatIsInCommand(i);
                break;
            case debug:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(i);
                DebugCommand(i);
                break;
        }
    }
}

void CommandProcessor::WorldCommand(const vector<string>& command) {
    string longitude1 = command[1].substr(0, command[1].size() - 1);
    string longitude2 = command[2].substr(0, command[2].size() - 1);
    string latitude1 = command[3].substr(0, command[3].size() - 1);
    string latitude2 = command[4].substr(0, command[4].size() - 1);

    // Set world boundaries
    if(command[1][command[1].size() - 1] == 'W')
        dms.f_longitude1 = -DMStoDDConverter(longitude1);

    if(command[2][command[2].size() - 1] == 'W')
        dms.f_longitude2 = -DMStoDDConverter(longitude2);

    dms.f_latitude1 = DMStoDDConverter(latitude1);
    dms.f_latitude2 = DMStoDDConverter(latitude2);

    logger.addLineBreak();
}

float CommandProcessor::DMStoDDConverter(const string& DMS) {
    float seconds = stof(DMS.substr(DMS.size() - 2, 2));
    float minutes = stof(DMS.substr(DMS.size() - 4, 2));
    float degrees = stof(DMS.substr(0, DMS.size() - 4));

    float dd = degrees + minutes/60 + seconds/3600;

    return dd;
}

void CommandProcessor::ImportCommand(const vector<string>& command) {
    logger.addLine("\n*** Importing dataset ***");

    ofstream database;
    database.open("database.txt");

    string data;

    vector<vector<string>> vector_collection;

    // Record of successful imports into database.txt
    int count_imported = 0;

    ifstream RecordsFile(command[1]);
    if (RecordsFile.good()) {
        while (getline(RecordsFile, data)) {
            vector<string> vector_data{};
            istringstream iss(data);
            string token;
            while (getline(iss, token, '|'))
                vector_data.push_back(token);
            vector_collection.push_back(vector_data);
        }
    }

    for (int i = 1; i < vector_collection.size(); i++)
    {
        vector<string> niEntry;
        if(stof(vector_collection[i][9]) >= dms.f_latitude1 && stof(vector_collection[i][9]) <= dms.f_latitude2)
        {
            if(stof(vector_collection[i][10]) >= dms.f_longitude1 && stof(vector_collection[i][10]) <= dms.f_longitude2)
            {
                for(int j = 0; j < vector_collection[i].size(); j++)
                {
                    // Add to database
                    stringstream ss;
                    ss << vector_collection[i][j] << "|";
                    if (j == vector_collection[i].size() - 1){
                        ss << vector_collection[i][j];
                    }
                    string GISRecord = ss.str();
                    database << GISRecord;
                }

                // Add to NameIndex
                string featureName = vector_collection[i][1];
                string stateAbv = vector_collection[i][3];
                niEntry.push_back(featureName);
                niEntry.push_back(stateAbv);
                ni.addToIndex(niEntry, to_string(i-1));

                // Count imports
                count_imported++;
            }
            database << '\n';
        }
    }

    database.close();

    stringstream ss;
    ss << endl;
    ss << "Imported Features by name: " << count_imported << endl;
    ss << "Longest probe sequence: " << ni.getHashTable().getNumCollisions() << endl;
    ss << "Imported Location: Not implemented" << endl;
    ss << "Average name length: Not implemented" << endl;
    logger.addLine(ss.str());
    logger.addLineBreak();
}

void CommandProcessor::WhatIsCommand(const vector<string> &command) {
    stringstream ss;

    vector<string> location;
    location.push_back(command[1]);
    location.push_back(command[2]);

    vector<string> searchResult;
    searchResult = ni.searchIndex(location); // Store searchResult

    if(!searchResult.empty())
    {
        ss << "\n\t" << searchResult[searchResult.size()-1] << ":  "
        << searchResult[5] << "  ("
        << searchResult[9] << ", "
        << searchResult[10] << ")";

        logger.addLine(ss.str());
        logger.addLineBreak();

        // Insert into buffer pool
        bufferPool.insert(searchResult[searchResult.size()-1], searchResult);
    }
    else
    {
        ss << "\n\tNo records match \"" << command[1] << "\" and \"" << command[2] << "\"";
        logger.addLine(ss.str());
        logger.addLineBreak();
    }
}

void CommandProcessor::WhatIsAtCommand(const vector<string> &command) {
    logger.addLine("\n\t*** what_is_at command not implemented. ***");
    logger.addLineBreak();
}

void CommandProcessor::WhatIsInCommand(const vector<string> &command) {
    logger.addLine("\n\t*** what_is_in command not implemented. ***");
    logger.addLineBreak();
}

void CommandProcessor::DebugCommand(const vector<string> &command) {

    if(command[1] == "hash")
    {
        logger.addLine(ni.str());
        logger.addLineBreak();
    }
    else if(command[1] == "pool")
    {
        logger.addLine(bufferPool.str());
        logger.addLineBreak();
    }
    else if(command[1] == "quad")
    {
        logger.addLine("\n\tQuadtree not implemented");
        logger.addLineBreak();
    }
}








