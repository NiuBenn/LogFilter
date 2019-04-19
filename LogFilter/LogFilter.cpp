#include"LogFilter.h"
#include<iostream>
#include<list>
#include<regex>
#include<fstream>

const int kTimeLen = strlen("2019-04-15 14:43:19.705");		//string Time ����


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
{
	return log_list_;
};

list<list<LogInfo>::iterator>& Logs::GetLogInfoListIter(string type, string param)
{
	return this->mm_logs_[type][param];
}

bool LogFilter::LogFilterInit()
{
	//�ж��ļ�·���Ƿ�Ϸ����ܷ�򿪣���
	ifstream log_file(log_file_path_);
	if (!log_file.is_open())
	{
		return false;
	}

	log_file.close();
	return true;
}

int LogFilter::UpdateLogs()	//����LogInfo�����³ɹ����ظ��³ɹ���Log������ʧ�ܷ���-1;
{
	ifstream log_file(log_file_path_);
	string one_line;	//��ʱ��������Ŵ��ļ��ж�ȡ����һ������
	
	int new_count = 0;

	if (!log_file.is_open())
	{
		return -1;
	}

	while (std::getline(log_file, one_line))	//ѭ����ȡ��ֱ�����ļ�����
	{
		if (new_count < count_)
		{
			new_count++;
			continue;
		}

		bool ret = logs_object_.AddLog(one_line);	//���LogInfo��Ϣ
		if (false == ret)
		{
			//���one_line���ǹ淶Log��Ϊ�˱�����Ч���ݶ�ʧ������׷�ӵ�ǰһ����ЧLogInfo��one_full_log_��
			if (logs_object_.LogList().size() > 0)
				logs_object_.LogList().back().one_full_log_ += "\n" + one_line;
		}
		new_count++;
	}

	log_file.close();
	swap(count_, new_count);
	return count_ - new_count;
}


int LogFilter::SearchLF(string input)
{
	bool param_time;
	string time;
	int time_stamp, dif, logs_count = 0;
	vector<pair<string, string>> kv;

	if (input.substr(0, strlen("logfilter")) != "logfilter")
	{
		return -1;
	}

	//logfilter --time 2019-04-15 14:23:34 1 --level E --pid 12345
	size_t index = input.find("--time");
	if (index != string::npos)
	{
		time = input.substr(index + strlen("--time "), strlen("2019-04-15 14:23:34"));
		time_stamp = Tools::StringToTimeStamp(time);

		dif = input[index + strlen("--time 2019-04-15 14:23:34 ")] - '0';
		param_time = true;
		input = input.substr(0, index) + input.substr(index + strlen("--time 2019-04-15 14:23:34 1 "));
	}
	else
		param_time = false;

	Tools::MakeKV(input, kv);
	if (kv.size() == 0 && false == param_time)
		return -1;

	if (kv.size() == 0)
	{
		logs_count = logs_object_.SearchByTimeL(time_stamp, dif);
		return logs_count;
	}

	if (kv[0].first != "level" && kv[0].first != "pid" &&
		kv[0].first != "tid" && kv[0].first != "version" &&
		kv[0].first != "module" && kv[0].first != "tag")
		return -1;

	list<list<LogInfo>::iterator>& loginfo_iter_list = logs_object_.GetLogInfoListIter(kv[0].first, kv[0].second);

	for (auto& iter : loginfo_iter_list)
	{
		bool typeok = true;
		//[level] [pid:tid] [version] [module] [tag]
		for (size_t i = 1; i < kv.size(); ++i)
		{
			if (kv[i].first == "level")
			{
				if (iter->level_ != kv[i].second)
				{
					typeok = false;
					break;
				}
			}
			else if (kv[i].first == "pid")
			{
				if (iter->pid_ != kv[i].second)
				{
					typeok = false;
					break;
				}
			}
			else if (kv[i].first == "tid")
			{
				if (iter->tid_ != kv[i].second)
				{
					typeok = false;
					break;
				}
			}
			else if (kv[i].first == "version")
			{
				if (iter->version_ != kv[i].second)
				{
					typeok = false;
					break;
				}
			}
			else if (kv[i].first == "module")
			{
				if (iter->module_ != kv[i].second)
				{
					typeok = false;
					break;
				}
			}
			else if (kv[i].first == "tag")
			{
				if (iter->tag_ != kv[i].second)
				{
					typeok = false;
					break;
				}
			}
			else
				return -1;
			
		}

		if (true == param_time)
		{
			if (iter->time_ < time_stamp - dif || iter->time_ > time_stamp + dif - 1)
				break;
		}
		if (true == typeok)
		{
			logs_count++;
			cout << iter->one_full_log_ << endl;
		}
	}
	
	return logs_count;
}