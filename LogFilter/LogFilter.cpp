#include"LogFilter.h"
#include<iostream>
#include<list>
#include<fstream>
#include<string.h>
#include<regex>

const int kTimeLen = strlen("2019-04-15 14:43:19.705");

bool Logs::AddLog(string& one_line)		//���LogInfo��Ϣ����������string�ǺϷ�Log����true�����򷵻�false
{

    vector<string> param;
    string str(one_line);
    LogInfo loginfo_temp;   //���loginfo��
    size_t begin_index = 0, end_index = 0;
    bool ret = false;

    std::string pattern{ "\\[[^\\[]*\\]" };
    std::regex re(pattern);

    std::smatch results;
    while (std::regex_search(str, results, re))
    {
        for (auto s : results)
            param.push_back(s);
        str = results.suffix().str();
    }

    if (param.size() < 6)
        return false;

    auto str_iter = param.begin();
    int time_stamp = Tools::StringToTimeStamp((*str_iter).substr(1, kTimeLen - 4));
    loginfo_temp.time_ = time_stamp;

    str_iter++;
    loginfo_temp.level_ = (*str_iter)[1];

    str_iter++;
    size_t mid_index = (*str_iter).find(':');
    if (mid_index == string::npos)
        return false;

    loginfo_temp.pid_ = (*str_iter).substr(1, mid_index - 1);
    loginfo_temp.tid_ = (*str_iter).substr(mid_index + 1);
	
    str_iter++;
    loginfo_temp.version_ = (*str_iter).substr(1, (*str_iter).size() - 2);  //��version
    
    str_iter++;
    loginfo_temp.module_ = (*str_iter).substr(1, (*str_iter).size() - 2);   //��module

    str_iter++;
    loginfo_temp.tag_ = (*str_iter).substr(1, (*str_iter).size() - 2);  //��tag

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

//void LogFilter::SearchByTimeLF()
//{
//	system("cls");
//	string time;
//	int dif, time_stamp;
//	cout << "��������Ҫ��ѯ��logʱ��(��ʽ��:2019-04-15 14:43:19)" << endl;
//	getchar();	//�Ե����뻺�����еĻس�
//
//	getline(cin, time);		//�����û������ʱ�䣬��Ϊ�м��пո�����cin�޷�һ�ν���
//	time_stamp = Tools::StringToTimeStamp(time);	//ת��Ϊʱ���
//
//	cout << "������Ҫ��ѯ��ʱ�䷶Χ(��Ϊ��λ������ 1 ����ǰ������1��)" << endl;
//	cin >> dif;
//
//	int ret = UpdateLogs();		//����LogInfo����
//	if (-1 == ret)
//	{
//		cout << "����log����ʧ�ܣ���" << endl;
//		system("pause");
//		return;
//	}
//	cout << "�ѳɹ�����" << ret << "��log��Ϣ,������ʱ��ɸѡ" << endl;
//	system("pause");
//
//	ret = logs_object_.SearchByTimeL(time_stamp, dif);	//����ɸѡ
//	cout << "�ѳɹ�ɸѡ��" << ret << "��log" << endl;
//	system("pause");
//}

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
