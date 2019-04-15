#include"LogFilter.h"
//#include"Tools.hpp"
#include<fstream>



bool Logs::AddLog(string& one_line)
{
	LogInfo log_temp;
	size_t begin_index, end_index;
	begin_index = one_line.find('[');
	if (begin_index != 0)
	{
		return false;
	}
	end_index = one_line.find(']');
	if (end_index != kTimeLen + 1)
	{
		return false;
	}

	int time_stamp = Tools::StringToTimeStamp(one_line.substr(1, kTimeLen - 4));
	log_temp.time_ = time_stamp;

	begin_index = one_line.find('[', end_index);
	if (begin_index == string::npos || begin_index == one_line.size() - 1)
	{
		return false;
	}

	log_temp.level_ = one_line[begin_index + 1];

	begin_index = one_line.find('[', begin_index + 1);
	end_index = one_line.find(']', begin_index + 1);
	if (begin_index == string::npos || end_index == string::npos)
	{
		return false;
	}

	size_t mid_index = one_line.find(':', begin_index);
	log_temp.pid_ = atoi(one_line.substr(begin_index + 1, mid_index - begin_index - 1).c_str());
	log_temp.tid_ = atoi(one_line.substr(mid_index + 1, end_index - mid_index - 1).c_str());

	begin_index = one_line.find('[', begin_index + 1);
	end_index = one_line.find(']', begin_index + 1);
	if (begin_index == string::npos || end_index == string::npos)
	{
		return false;
	}

	log_temp.version_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);

	begin_index = one_line.find('[', begin_index + 1);
	end_index = one_line.find(']', begin_index + 1);
	if (begin_index == string::npos || end_index == string::npos)
	{
		return false;
	}

	log_temp.module_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);

	begin_index = one_line.find('[', begin_index + 1);
	end_index = one_line.find(']', begin_index + 1);
	if (begin_index == string::npos || end_index == string::npos)
	{
		return false;
	}

	log_temp.tag_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);

	log_temp.one_full_log_ = one_line;

	log_list_.push_back(log_temp);

	return true;
};



bool LogFilter::LogFilterInit()
{
	ifstream log_file(log_file_path_);
	if (!log_file.is_open())
	{
		return false;
	}

	log_file.close();
	return true;
}

bool LogFilter::UpdateLogs()
{
	ifstream log_file(log_file_path_);
	string one_line;
	if (!log_file.is_open())
	{
		return false;
	}

	while (std::getline(log_file, one_line))
	{
		logs_object_.AddLog(one_line);
	}

	log_file.close();
	return true;
}