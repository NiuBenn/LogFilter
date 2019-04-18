#include"LogFilter.h"
#include<iostream>
#include<list>
#include<fstream>
#include<string.h>
#include<regex>

const int kTimeLen = strlen("2019-04-15 14:43:19.705");

bool Logs::AddLog(string& one_line)		//添加LogInfo信息，如果传入的string是合法Log返回true，否则返回false
{

    vector<string> param;
    string str(one_line);
    LogInfo loginfo_temp;   //靠縧oginfo靠
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
    loginfo_temp.version_ = (*str_iter).substr(1, (*str_iter).size() - 2);  //靠version
    
    str_iter++;
    loginfo_temp.module_ = (*str_iter).substr(1, (*str_iter).size() - 2);   //靠module

    str_iter++;
    loginfo_temp.tag_ = (*str_iter).substr(1, (*str_iter).size() - 2);  //靠tag

    loginfo_temp.one_full_log_ = one_line;

    log_list_.push_back(loginfo_temp);		//将获取到的LogIngo 临时对象 push到 log_list_中
	
	list<LofInfo>::iterator it_loginfo = log_list_.end();
	it_loginfo--;	//获取到刚才push的LogInfo在log_list_中的迭代器

	//将获取到的迭代器按类型push到m_logs中对应类型的list中
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
	int logs_count = 0;		//用来统计符合条件的log数量
	for (auto& loginfo : log_list_)		//遍历log_list_
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
	//判断文件路径是否合法？能否打开？？
	ifstream log_file(log_file_path_);
	if (!log_file.is_open())
	{
		return false;
	}

	log_file.close();
	return true;
}

int LogFilter::UpdateLogs()	//更新LogInfo，更新成功返回更新成功的Log条数，失败返回-1;
{
	ifstream log_file(log_file_path_);
	string one_line;	//临时变量，存放从文件中读取到的一行数据
	
	int new_count = 0;

	if (!log_file.is_open())
	{
		return -1;
	}

	while (std::getline(log_file, one_line))	//循环读取，直到将文件读完
	{
		if (new_count < count_)
		{
			new_count++;
			continue;
		}

		bool ret = logs_object_.AddLog(one_line);	//添加LogInfo信息
		if (false == ret)
		{
			//如果one_line不是规范Log，为了避免有效数据丢失，将其追加到前一条有效LogInfo的one_full_log_中
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
//	cout << "请输入您要查询的log时间(格式如:2019-04-15 14:43:19)" << endl;
//	getchar();	//吃掉输入缓冲区中的回车
//
//	getline(cin, time);		//接收用户输入的时间，因为中间有空格，所以cin无法一次接收
//	time_stamp = Tools::StringToTimeStamp(time);	//转换为时间戳
//
//	cout << "请输入要查询的时间范围(秒为单位，例如 1 代表前后误差各1秒)" << endl;
//	cin >> dif;
//
//	int ret = UpdateLogs();		//更新LogInfo数据
//	if (-1 == ret)
//	{
//		cout << "更新log数据失败！！" << endl;
//		system("pause");
//		return;
//	}
//	cout << "已成功更新" << ret << "条log信息,即将按时间筛选" << endl;
//	system("pause");
//
//	ret = logs_object_.SearchByTimeL(time_stamp, dif);	//进行筛选
//	cout << "已成功筛选出" << ret << "条log" << endl;
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
