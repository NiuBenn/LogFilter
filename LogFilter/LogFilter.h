#pragma once
#include<string>
#include<vector>
#include<list>
#include<map>
#include<time.h>
using namespace std;

const int kTimeLen = strlen("2019-04-15 14:43:19.705");		//string Time ����

class Tools		//�����࣬���涨�岿�ֹ��ߺ���
{
public:
	static int StringToTimeStamp(string time)	//��string Time ת���� ʱ���
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

typedef struct LogInfo	//LogIngo�ṹ�壬���һ��log������Ϣ
{
	int time_;	
	string level_;	
	string pid_;
	string tid_;
	string version_;
	string module_;
	string tag_;
	string one_full_log_;	//�������log������ǰ���time��level .. �Լ�log message; 
}LofInfo;

class Logs	//Log�࣬����ͳ�ƶ���LogInfo��һ������
{
public:
	bool AddLog(string& one_line);	//���LogInfo������������ǺϷ�Log����true,���򷵻�false
	
	int SearchByTimeL(int time_stamp, int dif);	//ͨ��ʱ����й��˼���
	
	list<LogInfo>& LogList();	//һ���ӿں�������˽�г�Ա log_list_ ����

	list<list<LogInfo>::iterator>& GetLogInfoListIter(string type, string param);

	map<string, map<string, list<list<LogInfo>::iterator>>> mm_logs_;	
	//����ͳ��LogInfo,��ŵ���log_list_�ĵ�����
private:
	list<LogInfo> log_list_;	//������е�LogInfo
};

class LogFilter		//LogFilter��־��������
{
public:
	LogFilter(string file_path)
		:log_file_path_(file_path),count_(0)
	{};

	bool LogFilterInit();	//ʹ��file_path��ʼ�����ж�·���ļ��Ƿ�Ϸ�
	
	int UpdateLogs();		//����log_object_��LogInfo��Ϣ
	
	int SearchLF(string input);		//����ģ��

private:
	int count_;		//�Ѿ���ȡ���ļ�����
	Logs logs_object_;		//һ��logs����ͳ��LogInfo
	string log_file_path_;	//Ҫ���в�������־�ļ�·��
};