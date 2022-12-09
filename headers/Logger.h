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

    /**
     * Logger constructor - helps with creating the beginning of the log file
     * @param dbFile name of database file
     * @param script name of script file
     * @param logFile name of log file
     */
    Logger(const string& dbFile, const string& script, const string& logFile);

    Logger& operator=(const Logger& other);

    /**
     * Adds a string and inserts a newline after
     * @param line string that gets appended to the log file
     */
    void addLine(const string& line);

    /**
     * Adds a string with no newline
     * @param text string that gets appended to the log file
     */
    void addText(const string& text);

    /**
     * Adds a string and inserts a newline after
     * @param line takes a vector<string> and prints it with proper spacing to the log file
     */
    void addLine(vector<string> line);

    /**
     * Adds a line break to the logfile
     */
    void addLineBreak();

    /**
     * Adds the closing time stamp to the log file
     */
    void endLogger();

    void startLogger();

private:
    /// Log file name
    string filename;
    string dbFile;
    string scriptFile;
    string logFile;
};


#endif //GIS_LOGGER_H
