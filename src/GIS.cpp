//
// Created by Mohammed on 2022-11-30.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <CommandProcessor.h>
#include <sstream>
#include <Logger.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
    string scripts;
    vector<string> worldCoords;
    CommandProcessor commandProcessor;
    Logger logger(argv[1], argv[2], argv[3]);

    cout << "Initializing database file..." << endl;
    ifstream DBFile(argv[1]);

    cout << "Initializing command script file..." << endl;
    ifstream ScriptFile(argv[2]);
    if (ScriptFile.good())
    {
        while (getline (ScriptFile, scripts))
        {
            //logger.addLine(scripts);
            vector<string> command {};
            istringstream iss(scripts);
            string token;
            while(getline(iss, token, '\t'))
                command.push_back(token);
            commandProcessor.addCommand(command);
        }
    }
    else
    {
        cout << "Script file not found!" << endl;
        EXIT_FAILURE;
    }

    commandProcessor.RunCommands(logger);

    cout << "Initializing log file..." << endl;
    ofstream LogFile(argv[3]);

    DBFile.close();
    ScriptFile.close();
    LogFile.close();
}
