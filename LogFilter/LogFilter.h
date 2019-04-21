#pragma once
#include<fstream>
#include<iostream>
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
}LogInfo;

class Filter	//Filter»ùÀà
{
public:
	virtual bool filtrate(const LogInfo& loginfo)
	{
		return false;
	}

	virtual ~Filter()
	{};
};

class Tools
{
public:
	static int StringToTimeStamp(const string& time);	//time(string) --> timestamp
	
	static bool IsFilePathOK(const string& log_file_path);	//OK --> true  

	static void ReadLogAndParse(std::istream& input, list<Filter*>& filter_list);

	static bool ParseLogLine(const string& one_line, LogInfo& loginfo);	

	static list<Filter*> CreatFilterList(const int argc, const char* argv[]);
};


class TimeFilter : public Filter
{
private:
	int time_stamp_;	
	int dif_;

public:
	TimeFilter(string time, int dif)
		:dif_(dif)
	{
		time_stamp_ = Tools::StringToTimeStamp(time);
	};

	bool filtrate(const LogInfo& loginfo);
};

class LevelFilter : public Filter
{
private:
	string level_;

public:
	LevelFilter(string level)
		:level_(level)
	{};

	bool filtrate(const LogInfo& loginfo);
};

class PidFilter : public Filter
{
private:
	string pid_;

public:
	PidFilter(string pid)
		:pid_(pid)
	{};

	bool filtrate(const LogInfo& loginfo);
};

class TidFilter : public Filter
{
private:
	string tid_;

public:
	TidFilter(string tid)
		:tid_(tid)
	{};

	bool filtrate(const LogInfo& loginfo);
};

class VersionFilter : public Filter
{
private:
	string version_;

public:
	VersionFilter(string version)
		:version_(version)
	{};

	bool filtrate(const LogInfo& loginfo);
};

class ModuleFilter : public Filter
{
private:
	string module_;

public:
	ModuleFilter(string module)
		:module_(module)
	{};

	bool filtrate(const LogInfo& loginfo);
};

class TagFilter : public Filter
{
private:
	string tag_;

public:
	TagFilter(string tag)
		:tag_(tag)
	{};

	bool filtrate(const LogInfo& loginfo);
};