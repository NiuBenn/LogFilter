#include"LogFilter.h"
#include<list>;
#include<fstream>


bool Logs::AddLog(string& one_line)		//添加LogInfo信息，如果传入的string是合法Log返回true，否则返回false
{
	
	LogInfo loginfo_temp;	//临时的loginfo对象
	size_t begin_index = 0, end_index = 0;
	bool ret = false;

	ret = Tools::GetIndex(one_line, begin_index, end_index);	//获取'[' ']'的下标，并且判断是否合法
	if (false == ret)
		return false;

	//将string 类型时间转换为时间戳，这里时间保留到单位：秒，如下
	//2019-04-15 14:43:19.705
	//2019-04-15 14:43:19
	int time_stamp = Tools::StringToTimeStamp(one_line.substr(1, kTimeLen - 4));
	loginfo_temp.time_ = time_stamp;

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	loginfo_temp.level_ = one_line[begin_index + 1];	//获取level

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	size_t mid_index = one_line.find(':', begin_index);		//获取pid、tid，pid与tid以 ':' 分隔
	loginfo_temp.pid_ = one_line.substr(begin_index + 1, mid_index - begin_index - 1);
	loginfo_temp.tid_ = one_line.substr(mid_index + 1, end_index - mid_index - 1);

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	loginfo_temp.version_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);	//获取version

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	loginfo_temp.module_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);	//获取module

	ret = Tools::GetIndex(one_line, begin_index, end_index);
	if (false == ret)
		return false;

	loginfo_temp.tag_ = one_line.substr(begin_index + 1, end_index - begin_index - 1);	//获取tag

	loginfo_temp.one_full_log_ = one_line;

	log_list_.push_back(loginfo_temp);		//将获取到的LogIngo 临时对象 push到 log_list_中
	
	list<LofInfo>::iterator it_loginfo = log_list_.end();
	it_loginfo--;	//获取到刚才push的LogInfo在log_list_中的迭代器

	//将获取到的迭代器按类型push到m_logs中对应类型的list中
	m_logs_["level"][loginfo_temp.level_].push_back(it_loginfo);
	m_logs_["pid"][loginfo_temp.pid_].push_back(it_loginfo);
	m_logs_["tid"][loginfo_temp.tid_].push_back(it_loginfo);
	m_logs_["version"][loginfo_temp.version_].push_back(it_loginfo);
	m_logs_["module"][loginfo_temp.module_].push_back(it_loginfo);
	m_logs_["tag"][loginfo_temp.tag_].push_back(it_loginfo);

	return true;
};

list<LogInfo>& Logs::LogList()	
{
	return log_list_;
};

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

bool LogFilter::UpdateLogs()	//更新LogInfo，更新成功返回true，失败返回false
{
	ifstream log_file(log_file_path_);
	string one_line;	//临时变量，存放从文件中读取到的一行数据
	
	if (!log_file.is_open())
	{
		return false;
	}

	while (std::getline(log_file, one_line))	//循环读取，直到将文件读完
	{
		bool ret = logs_object_.AddLog(one_line);	//添加LogInfo信息
		if (false == ret)
		{
			//如果one_line不是规范Log，为了避免有效数据丢失，将其追加到前一条有效LogInfo的one_full_log_中
			if (logs_object_.LogList().size() > 0)
				logs_object_.LogList().back().one_full_log_ += "\n" + one_line;
		}
	}

	log_file.close();
	return true;
}