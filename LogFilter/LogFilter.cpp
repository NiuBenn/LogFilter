#include"LogFilter.h"
#include<iostream>
#include<list>;
#include<fstream>


bool Logs::AddLog(string& one_line)		//���LogInfo��Ϣ����������string�ǺϷ�Log����true�����򷵻�false
{
	
	LogInfo loginfo_temp;	//��ʱ��loginfo����
	size_t begin_index = 0, end_index = 0;
	bool ret = false;

	ret = Tools::GetIndex(one_line, begin_index, end_index);	//��ȡ'[' ']'���±꣬�����ж��Ƿ�Ϸ�
	if (false == ret)
		return false;

	//��string ����ʱ��ת��Ϊʱ���������ʱ�䱣������λ���룬����
	//2019-04-15 14:43:19.705
	//2019-04-15 14:43:19
	int time_stamp = Tools::StringToTimeStamp(one_line.substr(1, kTimeLen - 4));
	loginfo_temp.time_ = time_stamp;

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	loginfo_temp.level_ = one_line[begin_index + 1];	//��ȡlevel

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	size_t mid_index = one_line.find(':', begin_index);		//��ȡpid��tid��pid��tid�� ':' �ָ�
	loginfo_temp.pid_ = one_line.substr(begin_index + 1, mid_index - begin_index - 1);
	loginfo_temp.tid_ = one_line.substr(mid_index + 1, end_index - mid_index - 1);

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	loginfo_temp.version_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);	//��ȡversion

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	loginfo_temp.module_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);	//��ȡmodule

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	loginfo_temp.tag_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);	//��ȡtag

	loginfo_temp.one_full_log_ = one_line;

	log_list_.push_back(loginfo_temp);		//����ȡ����LogIngo ��ʱ���� push�� log_list_��
	
	list<LofInfo>::iterator it_loginfo = log_list_.end();
	it_loginfo--;	//��ȡ���ղ�push��LogInfo��log_list_�еĵ�����

	//����ȡ���ĵ�����������push��m_logs�ж�Ӧ���͵�list��
	m_logs_["level"][loginfo_temp.level_].push_back(it_loginfo);
	m_logs_["pid"][loginfo_temp.pid_].push_back(it_loginfo);
	m_logs_["tid"][loginfo_temp.tid_].push_back(it_loginfo);
	m_logs_["version"][loginfo_temp.version_].push_back(it_loginfo);
	m_logs_["module"][loginfo_temp.module_].push_back(it_loginfo);
	m_logs_["tag"][loginfo_temp.tag_].push_back(it_loginfo);

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

int Logs::SearchByOtherL()
{
	return 0;
}

list<LogInfo>& Logs::LogList()	
{
	return log_list_;
};

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

void LogFilter::SearchByTimeLF()
{
	system("cls");
	string time;
	int dif, time_stamp;
	cout << "��������Ҫ��ѯ��logʱ��(��ʽ��:2019-04-15 14:43:19)" << endl;
	getchar();	//�Ե����뻺�����еĻس�

	getline(cin, time);		//�����û������ʱ�䣬��Ϊ�м��пո�����cin�޷�һ�ν���
	time_stamp = Tools::StringToTimeStamp(time);	//ת��Ϊʱ���

	cout << "������Ҫ��ѯ��ʱ�䷶Χ(��Ϊ��λ������ 1 ����ǰ������1��)" << endl;
	cin >> dif;

	int ret = UpdateLogs();		//����LogInfo����
	if (-1 == ret)
	{
		cout << "����log����ʧ�ܣ���" << endl;
		system("pause");
		return;
	}
	cout << "�ѳɹ�����" << ret << "��log��Ϣ,������ʱ��ɸѡ" << endl;
	system("pause");

	ret = logs_object_.SearchByTimeL(time_stamp, dif);	//����ɸѡ
	cout << "�ѳɹ�ɸѡ��" << ret << "��log" << endl;
	system("pause");
}

void LogFilter::SearchLF(string input)
{
	bool param_time;
	vector<pair<string, string>> kv;
	if (input.substr(0, strlen("logfilter")) != "logfilter")
	{
		cout << "�Ƿ������" << endl;
		return;
	}

	if (input.find("--time") != string::npos)
	{
		param_time = false;
		//input = input.substr();
	}
	else
		param_time = true;

	bool ret = Tools::MakeKV(input, kv);
	

}