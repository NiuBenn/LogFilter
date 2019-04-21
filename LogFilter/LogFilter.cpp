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

void Tools::ReadLogAndParse(std::istream& input, list<Filter*>& filter_list)
{
	LogInfo temp_loginfo;
	string one_line;
	bool last_log = false;

	while (std::getline(input, one_line))
	{
		bool ret = Tools::ParseLogLine(one_line, temp_loginfo);
		
        if (ret == false)
		{
			if (last_log == true)
			{
				std::cout << one_line << std::endl;
			}
		}
		else
		{
			last_log = true;
			for (auto& filter : filter_list)
			{
				if (filter->filtrate(temp_loginfo) == false)
				{
					last_log = false;
					break;
				}
			}
			if (last_log == true)
				std::cout << one_line << std::endl;
		}
	}

}

bool Tools::ParseLogLine(const string& one_line, LogInfo& loginfo)
{
	std::string pattern = R"(\[([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}).[0-9]*\]\[([A-Z])\]\[([0-9]*):([0-9]*)\] \[([^\[]*)\]\[([^\[]*)\]\[([^\[]*)\])";
	std::regex regex_object(pattern);
	std::smatch results;

	bool found = std::regex_search(one_line, results, regex_object);
	if (found == false)
		return false;

	loginfo.time_ = Tools::StringToTimeStamp(results[1]);		//time
	loginfo.level_ = results[2];								//level
	loginfo.pid_ = results[3];									//pid
	loginfo.tid_ = results[4];									//tid
	loginfo.version_ = results[5];								//version
	loginfo.module_ = results[6];								//module
	loginfo.tag_ = results[7];									//tid
	loginfo.one_full_log_ = one_line;

	return true;
}

list<Filter*> Tools::CreatFilterList(const int argc,const char* argv[])
{
	list<Filter*> filter_list;

	for (size_t i = 0; i < argc;)
	{
		if (strcmp(argv[i], "--time") == 0 && i + 3 < argc)
		{
			string time = argv[i + 1];
			time += " ";
			time += argv[i + 2];
			int dif = atoi(string(argv[i + 3]).c_str());
			TimeFilter * timefilter = new TimeFilter(time, dif);
			filter_list.push_back(timefilter);
			i += 4;
			continue;
		}
		if (strcmp(argv[i], "--level") == 0 && i + 1 < argc)
		{
			string level = argv[i + 1];
			LevelFilter* levelfilter = new LevelFilter(level);
			filter_list.push_back(levelfilter);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--pid") == 0 && i + 1 < argc)
		{
			string pid = argv[i + 1];
			PidFilter* pidfilter = new PidFilter(pid);
			filter_list.push_back(pidfilter);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--tid") == 0 && i + 1 < argc)
		{
			string tid = argv[i + 1];
			TidFilter* tidfilter = new TidFilter(tid);
			filter_list.push_back(tidfilter);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--version") == 0 && i + 1 < argc)
		{
			string version = argv[i + 1];
			VersionFilter* versionfilter = new VersionFilter(version);
			filter_list.push_back(versionfilter);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--module") == 0 && i + 1 < argc)
		{
			string module = argv[i + 1];
			ModuleFilter* modulefilter = new ModuleFilter(module);
			filter_list.push_back(modulefilter);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--tag") == 0 && i + 1 < argc)
		{
			string tag = argv[i + 1];
			TagFilter* tagfilter = new TagFilter(tag);
			filter_list.push_back(tagfilter);
			i += 2;
			continue;
		}
		i++;
	}

	return filter_list;
}


bool TimeFilter::filtrate(const LogInfo& loginfo)
{
	if (loginfo.time_ > time_stamp_ + dif_ - 1 || loginfo.time_ < time_stamp_ - dif_)
		return false;
	return true;
}

bool LevelFilter::filtrate(const LogInfo& loginfo)
{
	return level_ == loginfo.level_;
}

bool PidFilter::filtrate(const LogInfo& loginfo)
{
	return pid_ == loginfo.pid_;
}

bool TidFilter::filtrate(const LogInfo& loginfo)
{
	return tid_ == loginfo.tid_;
}

bool VersionFilter::filtrate(const LogInfo& loginfo)
{
	return version_ == loginfo.version_;
}

bool ModuleFilter::filtrate(const LogInfo& loginfo)
{
	return module_ == loginfo.module_;
}

bool TagFilter::filtrate(const LogInfo& loginfo)
{
	return tag_ == loginfo.tag_;
}
