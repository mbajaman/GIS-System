//
// Created by Mohammed on 2022-11-30.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include "Logger.h"

Logger::Logger() {
    ofstream logfile;
    logfile.open(filename);
    logfile.close();
}

Logger::Logger(const string& dbFile, const string& script, const string& logFile) {
    ofstream logfile;
    filename = logFile;
    logfile.open(filename);
    ostringstream ss;

    // Create the beginning of the log file.
    ss << "Course Project for COMP 8042\n";
    ss << "Student Name: Mohammed Bajaman, Student ID: A00991074\n";
    ss << "Begin of GIS Program log:\n";
    ss << "dbFile: " << dbFile << endl;
    ss << "script: " << script << endl;
    ss << "log: " << logFile << endl;

    // Get current time and append to starting of log file
    auto getTime = chrono::system_clock::now();
    time_t start_time = chrono::system_clock::to_time_t(getTime);
    ss << "Start Time: " << ctime(&start_time);
    logfile << ss.str();
    logfile.close();
}

void Logger::addLine(const string& line) {
    ofstream logfile;
    logfile.open(filename, ios::app);
    logfile << line << endl;
    logfile.close();
}

void Logger::addLine(vector<string> line) {
    ofstream logfile;
    logfile.open(filename, ios::app);

    stringstream ss;
    for(auto it = line.begin(); it != line.end(); it++)
    {
        if(it != line.begin())
        {
            ss << " ";
        }
        ss << *it;
    }
    logfile << ss.str() << endl;
    logfile.close();
}

void Logger::addLineBreak() {
    ofstream logfile;
    logfile.open(filename, ios::app);
    logfile << "------------------------------------------------------------------------------------------" << endl;
    logfile.close();
}

void Logger::addText(const string& text) {
    ofstream logfile;
    logfile.open(filename, ios::app);
    logfile << text;
    logfile.close();
}

Logger& Logger::operator=(const Logger &other) {
    filename = other.filename;
    return *this;
}


