#pragma once
#include<string>
#include<vector>
#include<map>



typedef struct LogInfo
{
    int time_;
    char level_;
    int pid_;
    int tid_;
    string version_;
    string module_;
    string tag_;
    //string log_message_;
    string one_full_log_;
};

class Logs
{
public:
    void AddLog(string& one_line);
    void Search()；    
private:
    map<map<LogInfo*>> mm_logs_;
    vector<LogInfo> log_list_;
};


