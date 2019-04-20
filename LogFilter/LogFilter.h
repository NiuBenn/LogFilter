#pragma once
#include<fstream>
#include<list>
#include<string>
#include<time.h>

using std::string;
using std::list;

typedef struct LogInfo	//struct log info
{
	int time_;
	string level_;
	string pid_;
	string tid_;
	string version_;
	string module_;
	string tag_;
	string one_full_log_;	//one full log  include time level pid ...
}LofInfo;

class Tools
{
public:
	static int StringToTimeStamp(const string& time);	//time(string) --> timestamp
	
	static bool IsFilePathOK(const string& log_file_path);	//OK --> true  

	static int ReadFileAndAdd(const string& log_file_path, list<LogInfo>& loginfo_list); 

	static bool AddLogForList(const string& one_line, list<LogInfo>& loginfo_list);	
};

class Filter	//Filter»ùÀà
{
public:
	virtual list<LogInfo> filtrate(list<LogInfo>& loginfo_list)
	{
		list<LogInfo> result;
		return result;
	}
};

class TimeFilter : public Filter
{
private:
	string time_;	
	int dif_;
public:
	TimeFilter(string time, int dif)
		:time_(time), dif_(dif)
	{};

	list<LofInfo> filtrate(list<LogInfo>& loginfo_list);
};

class LevelFilter : public Filter
{
private:
	string level_;

public:
	LevelFilter(string level)
		:level_(level)
	{};

	list<LofInfo> filtrate(list<LogInfo>& loginfo_list);
};

class PidFilter : public Filter
{
private:
	string pid_;

public:
	PidFilter(string pid)
		:pid_(pid)
	{};

	list<LofInfo> filtrate(list<LogInfo>& loginfo_list);
};

class TidFilter : public Filter
{
private:
	string tid_;

public:
	TidFilter(string tid)
		:tid_(tid)
	{};

	list<LofInfo> filtrate(list<LogInfo>& loginfo_list);
};

class VersionFilter : public Filter
{
private:
	string version_;

public:
	VersionFilter(string version)
		:version_(version)
	{};

	list<LofInfo> filtrate(list<LogInfo>& loginfo_list);
};

class ModuleFilter : public Filter
{
private:
	string module_;

public:
	ModuleFilter(string module)
		:module_(module)
	{};

	list<LofInfo> filtrate(list<LogInfo>& loginfo_list);
};

class TagFilter : public Filter
{
private:
	string tag_;

public:
	TagFilter(string tag)
		:tag_(tag)
	{};

	list<LofInfo> filtrate(list<LogInfo>& loginfo_list);
};