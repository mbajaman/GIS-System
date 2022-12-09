// Name: Mohammed Bajaman
// Date: 2022-11-30
// Student ID: A00991074
// COMP 8042 - Advanced Algorithms

#include <iostream>
#include <fstream>
#include <vector>
#include <CommandProcessor.h>
#include <sstream>
#include <Logger.h>
#include <cstring>

using namespace std;

/** Main function that initializes the program
 * Reference: https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
 * @param contains command line arguments
 */
int main(int argc, char *argv[])
{
    // String variable for storing each line of the script file
    string scripts;

    // Store World Boundaries in vector<string>
    vector<string> worldCoords;

    // Initialize the comamandProcessor
    CommandProcessor commandProcessor;

    // Pass the file names to the logger class
    Logger logger(argv[1], argv[2], argv[3]);

    ofstream LogFile(argv[3]);
    logger.addLine("*** Initializing log file ***");

    logger.addLine("\n*** Initializing database file ***");
    ifstream DBFile(argv[1]);

    logger.addLine("\n*** Reading from command script file ***\n");
    ifstream ScriptFile(argv[2]);

    // Make sure file exists
    if (ScriptFile.good())
    {
        while (getline (ScriptFile, scripts))
        {
            vector<string> command {}; // store command in this vector
            istringstream iss(scripts);
            string token;
            while(getline(iss, token, '\t'))
                command.push_back(token); // Parse each entry of the command delimited by tab and send to commandProcessor
            commandProcessor.addCommand(command);
        }
    }
    else
    {
        logger.addLine("Script file not found! Exiting...");
        EXIT_FAILURE;
    }


    // Pass logger instance and run all the commands in CommandProcessor
    commandProcessor.RunCommands(logger);

    // Make sure to close files
    DBFile.close();
    ScriptFile.close();
    LogFile.close();
}
