#include"LogFilter.h"
#include<iostream>
#include<string.h>

int main(int argc, char* argv[])
{
	list<LogInfo> loginfo_list;
	
	bool ret = Tools::IsFilePathOK(argv[1]);
	if (ret == false)
	{
		std::cout << "Open Log File Error!!" << std::endl;
		return -1;
	}

	Tools::ReadFileAndAdd(argv[1], loginfo_list);
	

	for (size_t i = 2; i < argc;)
	{
		if (strcmp(argv[i], "--time") == 0 && i + 3 < argc)
		{
			string time = argv[i + 1];
			time += " ";
			time += argv[i + 2];
			int dif = atoi(string(argv[i + 3]).c_str());
			TimeFilter timefilter(time, dif);
			loginfo_list = timefilter.filtrate(loginfo_list);
			i += 4;
			continue;
		}
		if (strcmp(argv[i], "--level") == 0 && i + 1 < argc)
		{
			string level = argv[i + 1];
			LevelFilter levelfilter(level);
			loginfo_list = levelfilter.filtrate(loginfo_list);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--pid") == 0 && i + 1 < argc)
		{
			string pid = argv[i + 1];
			PidFilter pidfilter(pid);
			loginfo_list = pidfilter.filtrate(loginfo_list);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--tid") == 0 && i + 1 < argc)
		{
			string tid = argv[i + 1];
			TidFilter tidfilter(tid);
			loginfo_list = tidfilter.filtrate(loginfo_list);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--version") == 0 && i + 1 < argc)
		{
			string version = argv[i + 1];
			VersionFilter versionfilter(version);
			loginfo_list = versionfilter.filtrate(loginfo_list);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--module") == 0 && i + 1 < argc)
		{
			string module = argv[i + 1];
			ModuleFilter modulefilter(module);
			loginfo_list = modulefilter.filtrate(loginfo_list);
			i += 2;
			continue;
		}
		if (strcmp(argv[i], "--tag") == 0 && i + 1 < argc)
		{
			string tag = argv[i + 1];
			TagFilter tagfilter(tag);
			loginfo_list = tagfilter.filtrate(loginfo_list);
			i += 2;
			continue;
		}
		i++;
	}

    for(auto& loginfo : loginfo_list)
    {
        std::cout << loginfo.one_full_log_ << std::endl;
    }

	return 0;
}
