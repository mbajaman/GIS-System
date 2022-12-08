//
// Created by Mohammed on 2022-11-30.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include "Logger.h"

Logger::Logger() {
    logfile.open("log01.txt");
    logfile.close();
}

Logger::Logger(string dbFile, string script, string logFile) {
    logfile.open("log01.txt");
    ostringstream ss;

    ss << "Course Project for COMP 8042\n";
    ss << "Student Name: Mohammed Bajaman, Student ID: A00991074\n";
    ss << "Begin of GIS Program log:\n";
    ss << "dbFile: " << dbFile << endl;
    ss << "script: " << script << endl;
    ss << "log: " << logFile << endl;

    auto getTime = chrono::system_clock::now();
    time_t start_time = chrono::system_clock::to_time_t(getTime);
    ss << "Start Time: " << ctime(&start_time);

    logfile << ss.str();
}

void Logger::addLine(string line) {
    logfile.open("log01.txt", ios::app);
    logfile << line << endl;
    logfile.close();
}

void Logger::addLine(vector<string> line) {
    logfile.open("log01.txt", ios::app);

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
    logfile.open("log01.txt", ios::app);
    logfile << "------------------------------------------------------------------------------------------" << endl;
    logfile.close();
}

void Logger::addText(string text) {
    logfile.open("log01.txt", ios::app);
    logfile << text;
    logfile.close();
}


