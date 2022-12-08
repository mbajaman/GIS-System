//
// Created by Mohammed on 2022-11-30.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <Logger.h>
#include <NameIndex.h>
#include "CommandProcessor.h"



CommandProcessor::CommandProcessor() {
}

CommandProcessor::~CommandProcessor() {
    CommandList.clear();
}

void CommandProcessor::addCommand(vector<string> command) {
    CommandList.push_back(command);
}

void CommandProcessor::RunCommands(Logger &logger) {
    std::map<std::string, enum_command> commandMap = {
            {"world", enum_command::world},
            {"quit", enum_command::quit},
            {"import", enum_command::import},
            {"what_is", enum_command::what_is},
            {"what_is_at", enum_command::what_is_at},
            {"what_is_in", enum_command::what_is_in},
            {"debug", enum_command::debug}
    };

    int commandsCount = 0;

    for(int i = 0; i < CommandList.size(); i++)
    {
        if(CommandList[i][0][0] == ';')
        {
            logger.addLine(CommandList[i]);
            continue;
        }

        switch(commandMap.find(CommandList[i][0])->second){
            case world:
                logger.addLine(CommandList[i]);
                WorldCommand(CommandList[i]);
                break;
            case quit:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(CommandList[i]);
                break;
            case import:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(CommandList[i]);
                ImportCommand(CommandList[i]);
                break;
            case what_is:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(CommandList[i]);
                WhatIsCommand(CommandList[i]);
                break;
            case what_is_at:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(CommandList[i]);
                cout << "Checking coordinates... " << CommandList[i][1] << " | " << CommandList[i][2] << endl;
                break;
            case what_is_in:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(CommandList[i]);
                cout << "What is in... " << CommandList[i][1] << " | " << CommandList[i][2] << endl;
                break;
            case debug:
                commandsCount++;
                logger.addText("Command " + to_string(commandsCount) +": ");
                logger.addLine(CommandList[i]);
                DebugCommand(CommandList[i]);
                break;
        }
    }
}

void CommandProcessor::WorldCommand(const vector<string>& command) {
    cout << "Running World command..." << endl;
    string longitude1 = command[1].substr(0, command[1].size() - 1);
    string longitude2 = command[2].substr(0, command[2].size() - 1);
    string latitude1 = command[3].substr(0, command[3].size() - 1);
    string latitude2 = command[4].substr(0, command[4].size() - 1);

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
    cout << "Importing dataset... " << command[1] << endl;

    ofstream database;
    database.open("database.txt");

    string data;

    vector<vector<string>> vector_collection;

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
                    count_imported++;
                }

                // Add to NameIndex
                string featureName = vector_collection[i][1];
                string stateAbv = vector_collection[i][3];
                niEntry.push_back(featureName);
                niEntry.push_back(stateAbv);
                //niEntry.push_back(to_string(i-1));
                ni.addToIndex(niEntry, to_string(i-1));

            }
            database << '\n';
        }
    }

    database.close();

    stringstream ss;
    ss << endl;
    ss << "Imported Features by name: " << count_imported << endl;
    ss << "Longest probe sequence: " << ni.hashTable.numCollisions << endl;
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
    searchResult = ni.searchIndex(location);

    if(!searchResult.empty())
    {
        ss << "\n\t" << searchResult[searchResult.size()-1] << ":  "
        << searchResult[5] << "  ("
        << searchResult[9] << ", "
        << searchResult[10] << ")";

        logger.addLine(ss.str());
        logger.addLineBreak();
    }
    else
    {
        ss << "\n\tNo records match\"" << command[1] << "\" and \"" << command[2] << "\"";
        logger.addLine(ss.str());
        logger.addLineBreak();
    }
}

void CommandProcessor::DebugCommand(const vector<string> &command) {

    if(command[1] == "hash")
    {
        logger.addLine(ni.str());
        logger.addLineBreak();
    }
}






