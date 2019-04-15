#pragma once
#include<string>
#include<vector>
#include<map>
#include<time.h>
using namespace std;

const int kTimeLen = strlen("2019-04-15 14:43:19.705");

class Tools
{
public:
	static int StringToTimeStamp(string time)
	{
		struct tm time_stamp;

		//2019-04-15 14:43:19
		time_stamp.tm_year = atoi(time.substr(0, 4).c_str()) - 1900;
		time_stamp.tm_mon = atoi(time.substr(5, 2).c_str()) - 1;
		time_stamp.tm_mday = atoi(time.substr(8, 2).c_str());
		time_stamp.tm_mon = atoi(time.substr(11, 2).c_str());
		time_stamp.tm_mon = atoi(time.substr(14, 2).c_str());
		time_stamp.tm_mon = atoi(time.substr(17, 2).c_str());

		return (int)mktime(&time_stamp);
	}
};

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
}LofInfo;

class Logs
{
public:
	bool AddLog(string& one_line);
	void Search();
private:
	map<string, map<string, LogInfo*>> mm_logs_;
	vector<LogInfo> log_list_;
};

class LogFilter
{
public:
	LogFilter(string file_path)
		:log_file_path_(file_path)
	{};

	bool LogFilterInit();

	bool UpdateLogs();

private:
	Logs logs_object_;
	string log_file_path_;
};