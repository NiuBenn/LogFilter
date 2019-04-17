#pragma once
#include<string>
#include<vector>
#include<list>
#include<map>
#include<time.h>
using namespace std;

const int kTimeLen = strlen("2019-04-15 14:43:19.705");		//string Time 长度

class Tools		//工具类，里面定义部分工具函数
{
public:
	static int StringToTimeStamp(string time)	//将string Time 转换成 时间戳
	{
		struct tm time_stamp;
		//2019-04-15 14:43:19

		time_stamp.tm_year = atoi(time.substr(0, 4).c_str()) - 1900;
		time_stamp.tm_mon = atoi(time.substr(5, 2).c_str()) - 1;
		time_stamp.tm_mday = atoi(time.substr(8, 2).c_str());
		time_stamp.tm_hour = atoi(time.substr(11, 2).c_str());
		time_stamp.tm_min = atoi(time.substr(14, 2).c_str());
		time_stamp.tm_sec = atoi(time.substr(17, 2).c_str());

		time_t stamp =  mktime(&time_stamp);
		return stamp;
	}

	static bool GetIndex(string& one_line, size_t& begin_index, size_t& end_index)
	{
		if (begin_index == 0 && end_index == 0)
			begin_index = one_line.find('[');
		else 
			begin_index = one_line.find('[', end_index + 1);
		
		end_index = one_line.find(']', begin_index + 1);

		if (begin_index == string::npos || end_index == string::npos)
			return false;

		return true;
	}

	static void MakeKV(string input, vector<pair<string, string>>& kv)
	{
		//logfilter --time 2019-04-15 14:23:34 1 --level I --pid 12345
		size_t begin = 0, end = 0;
		string pair_first, pair_second;
		
		while (1)
		{
			if (end == string::npos)
				return;
			begin = input.find("--", end + 1) + 2;
			end = input.find(" ", begin + 1);
			if (begin - 2 == string::npos)
				return;

			pair_first = input.substr(begin, end - begin);
			begin = end + 1;
			end = input.find(" ", begin + 1);
			pair_second = input.substr(begin, end - begin);
			kv.push_back(make_pair(pair_first, pair_second));
		}
	}
};

typedef struct LogInfo	//LogIngo结构体，存放一条log的信信息
{
	int time_;	
	string level_;	
	string pid_;
	string tid_;
	string version_;
	string module_;
	string tag_;
	string one_full_log_;	//存放整条log，包括前面的time、level .. 以及log message; 
}LofInfo;

class Logs	//Log类，用来统计多条LogInfo的一个集合
{
public:
	bool AddLog(string& one_line);	//添加LogInfo，传入参数若是合法Log返回true,否则返回false
	
	int SearchByTimeL(int time_stamp, int dif);	//通过时间进行过滤检索
	
	list<LogInfo>& LogList();	//一个接口函数，将私有成员 log_list_ 返回

	list<list<LogInfo>::iterator>& GetLogInfoListIter(string type, string param);

	map<string, map<string, list<list<LogInfo>::iterator>>> mm_logs_;	
	//分类统计LogInfo,存放的是log_list_的迭代器
private:
	list<LogInfo> log_list_;	//存放所有的LogInfo
};

class LogFilter		//LogFilter日志过滤器类
{
public:
	LogFilter(string file_path)
		:log_file_path_(file_path),count_(0)
	{};

	bool LogFilterInit();	//使用file_path初始化，判断路径文件是否合法
	
	int UpdateLogs();		//更新log_object_的LogInfo信息
	
	int SearchLF(string input);		//检索模块

private:
	int count_;		//已经读取的文件行数
	Logs logs_object_;		//一个logs对象，统计LogInfo
	string log_file_path_;	//要进行操作的日志文件路径
};