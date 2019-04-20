#include"LogFilter.h"
#include<regex>

int Tools::StringToTimeStamp(const string& time)
{
	struct tm time_stamp;
	//2019-04-15 14:43:19

	time_stamp.tm_year = atoi(time.substr(0, 4).c_str()) - 1900;
	time_stamp.tm_mon = atoi(time.substr(5, 2).c_str()) - 1;
	time_stamp.tm_mday = atoi(time.substr(8, 2).c_str());
	time_stamp.tm_hour = atoi(time.substr(11, 2).c_str());
	time_stamp.tm_min = atoi(time.substr(14, 2).c_str());
	time_stamp.tm_sec = atoi(time.substr(17, 2).c_str());

	time_t stamp = mktime(&time_stamp);
	return stamp;
}

bool Tools::IsFilePathOK(const string& log_file_path)	
{
	//can the file be opened
	std::ifstream log_file(log_file_path);
	if (!log_file.is_open())
		return false;
	
	log_file.close();
	return true;
}

int Tools::ReadFileAndAdd(const string& log_file_path, list<LogInfo>& loginfo_list)
{
	std::ifstream log_file(log_file_path);	
	string one_line;
	int count = 0;

	if (!log_file.is_open())
	{
		return -1;
	}

	while (std::getline(log_file, one_line))	//循环读取，直到将文件读完
	{
		bool ret = AddLogForList(one_line, loginfo_list);	//添加LogInfo信息
		if (false == ret)
		{
			//如果one_line不是规范Log，为了避免有效数据丢失，将其追加到前一条有效LogInfo的one_full_log_中
			if (loginfo_list.size() > 0)
				loginfo_list.back().one_full_log_ += "\n" + one_line;
		}
		count++;
	}

	return count;
}

bool Tools::AddLogForList(const string& one_line, list<LogInfo>& loginfo_list)
{
	LogInfo loginfo_temp;
	std::string pattern = R"(\[([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}).[0-9]*\]\[([A-Z])\]\[([0-9]*):([0-9]*)\] \[([^\[]*)\]\[([^\[]*)\]\[([^\[]*)\])";
	std::regex regex_object(pattern);
	std::smatch results;

	bool found = std::regex_search(one_line, results, regex_object);
	if (found == false)
		return false;

	loginfo_temp.time_ = Tools::StringToTimeStamp(results[1]);		//time
	loginfo_temp.level_ = results[2];								//level
	loginfo_temp.pid_ = results[3];									//pid
	loginfo_temp.tid_ = results[4];									//tid
	loginfo_temp.version_ = results[5];								//version
	loginfo_temp.module_ = results[6];								//module
	loginfo_temp.tag_ = results[7];									//tid
	loginfo_temp.one_full_log_ = one_line;

	loginfo_list.push_back(loginfo_temp);		//将获取到的LogIngo 临时对象 push到 log_list_中

	return true;
}

list<LofInfo> TimeFilter::filtrate(list<LogInfo>& loginfo_list)
{
	int time_stamp = Tools::StringToTimeStamp(time_);	//time(str) --> time_stamp(int)
	list<LogInfo>::iterator iter = loginfo_list.begin();

	while (iter != loginfo_list.end())
	{
		if ((*iter).time_ > time_stamp + dif_ - 1 || (*iter).time_ < time_stamp - dif_)
		{
			list<LogInfo>::iterator iter_temp = iter;
			iter++;
			loginfo_list.erase(iter_temp);
		}
		else
			iter++;
	}

	return loginfo_list;
}

list<LofInfo> LevelFilter::filtrate(list<LogInfo>& loginfo_list)
{
	list<LogInfo>::iterator iter = loginfo_list.begin();
	while (iter != loginfo_list.end())
	{
		if ((*iter).level_ != level_)
		{
			list<LogInfo>::iterator iter_temp = iter;
			iter++;
			loginfo_list.erase(iter_temp);
		}
		else
			iter++;
	}

	return loginfo_list;
}

list<LofInfo> PidFilter::filtrate(list<LogInfo>& loginfo_list)
{
	list<LogInfo>::iterator iter = loginfo_list.begin();
	while (iter != loginfo_list.end())
	{
		if ((*iter).pid_ != pid_)
		{
			list<LogInfo>::iterator iter_temp = iter;
			iter++;
			loginfo_list.erase(iter_temp);
		}
		else
			iter++;
	}

	return loginfo_list;
}

list<LofInfo> TidFilter::filtrate(list<LogInfo>& loginfo_list)
{
	list<LogInfo>::iterator iter = loginfo_list.begin();
	while (iter != loginfo_list.end())
	{
		if ((*iter).tid_ != tid_)
		{
			list<LogInfo>::iterator iter_temp = iter;
			iter++;
			loginfo_list.erase(iter_temp);
		}
		else
			iter++;
	}

	return loginfo_list;
}

list<LofInfo> VersionFilter::filtrate(list<LogInfo>& loginfo_list)
{
	list<LogInfo>::iterator iter = loginfo_list.begin();
	while (iter != loginfo_list.end())
	{
		if ((*iter).version_ != version_)
		{
			list<LogInfo>::iterator iter_temp = iter;
			iter++;
			loginfo_list.erase(iter_temp);
		}
		else
			iter++;
	}

	return loginfo_list;
}

list<LofInfo> ModuleFilter::filtrate(list<LogInfo>& loginfo_list)
{
	list<LogInfo>::iterator iter = loginfo_list.begin();
	while (iter != loginfo_list.end())
	{
		if ((*iter).module_ != module_)
		{
			list<LogInfo>::iterator iter_temp = iter;
			iter++;
			loginfo_list.erase(iter_temp);
		}
		else
			iter++;
	}

	return loginfo_list;
}

list<LofInfo> TagFilter::filtrate(list<LogInfo>& loginfo_list)
{
	list<LogInfo>::iterator iter = loginfo_list.begin();
	while (iter != loginfo_list.end())
	{
		if ((*iter).tag_ != tag_)
		{
			list<LogInfo>::iterator iter_temp = iter;
			iter++;
			loginfo_list.erase(iter_temp);
		}
		else
			iter++;
	}

	return loginfo_list;
}

