#include"LogFilter.h"
<<<<<<< HEAD
=======
#include<iostream>
#include<list>
>>>>>>> 00977cbd6a3f38765a0c085723ef3437b0de1d64
#include<regex>
#include<fstream>

const int kTimeLen = strlen("2019-04-15 14:43:19.705");		//string Time ����

<<<<<<< HEAD
int Tools::StringToTimeStamp(const string& time)
=======

bool Logs::AddLog(string& one_line)		//���LogInfo��Ϣ����������string�ǺϷ�Log����true�����򷵻�false
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

	log_list_.push_back(loginfo_temp);		//����ȡ����LogIngo ��ʱ���� push�� log_list_��
	
	list<LofInfo>::iterator it_loginfo = log_list_.end();
	it_loginfo--;	//��ȡ���ղ�push��LogInfo��log_list_�еĵ�����

	//����ȡ���ĵ�����������push��m_logs�ж�Ӧ���͵�list��
	mm_logs_["level"][loginfo_temp.level_].push_back(it_loginfo);
	mm_logs_["pid"][loginfo_temp.pid_].push_back(it_loginfo);
	mm_logs_["tid"][loginfo_temp.tid_].push_back(it_loginfo);
	mm_logs_["version"][loginfo_temp.version_].push_back(it_loginfo);
	mm_logs_["module"][loginfo_temp.module_].push_back(it_loginfo);
	mm_logs_["tag"][loginfo_temp.tag_].push_back(it_loginfo);

	return true;
};

int Logs::SearchByTimeL(int time_stamp, int dif)
{
	int logs_count = 0;		//����ͳ�Ʒ���������log����
	for (auto& loginfo : log_list_)		//����log_list_
	{
		if (loginfo.time_ >= time_stamp - dif && loginfo.time_ <= time_stamp + dif - 1)
		{
			logs_count++;
			cout << loginfo.one_full_log_ << endl;
		}
	}
	return logs_count;
}

list<LogInfo>& Logs::LogList()	
>>>>>>> 00977cbd6a3f38765a0c085723ef3437b0de1d64
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

	while (std::getline(log_file, one_line))	//ѭ����ȡ��ֱ�����ļ�����
	{
		bool ret = AddLogForList(one_line, loginfo_list);	//���LogInfo��Ϣ
		if (false == ret)
		{
			//���one_line���ǹ淶Log��Ϊ�˱�����Ч���ݶ�ʧ������׷�ӵ�ǰһ����ЧLogInfo��one_full_log_��
			if (loginfo_list.size() > 0)
				loginfo_list.back().one_full_log_ += "\n" + one_line;
		}
		count++;
	}

	return count;
}

<<<<<<< HEAD
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
=======
>>>>>>> 00977cbd6a3f38765a0c085723ef3437b0de1d64

	loginfo_list.push_back(loginfo_temp);		//����ȡ����LogIngo ��ʱ���� push�� log_list_��

	return true;
}

list<LofInfo> TimeFilter::filtrate(list<LogInfo>& loginfo_list)
{
<<<<<<< HEAD
	int time_stamp = Tools::StringToTimeStamp(time_);	//time(str) --> time_stamp(int)
	list<LogInfo>::iterator iter = loginfo_list.begin();

	while (iter != loginfo_list.end())
=======
	bool param_time;
	string time;
	int time_stamp, dif, logs_count = 0;
	vector<pair<string, string>> kv;

	if (input.substr(0, strlen("logfilter")) != "logfilter")
>>>>>>> 00977cbd6a3f38765a0c085723ef3437b0de1d64
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
<<<<<<< HEAD
		if ((*iter).level_ != level_)
		{
			list<LogInfo>::iterator iter_temp = iter;
			iter++;
			loginfo_list.erase(iter_temp);
		}
		else
			iter++;
=======
		time = input.substr(index + strlen("--time "), strlen("2019-04-15 14:23:34"));
		time_stamp = Tools::StringToTimeStamp(time);

		dif = input[index + strlen("--time 2019-04-15 14:23:34 ")] - '0';
		param_time = true;
		input = input.substr(0, index) + input.substr(index + strlen("--time 2019-04-15 14:23:34 1 "));
>>>>>>> 00977cbd6a3f38765a0c085723ef3437b0de1d64
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
<<<<<<< HEAD

	return loginfo_list;
}

=======
	
	return logs_count;
}
>>>>>>> 00977cbd6a3f38765a0c085723ef3437b0de1d64
