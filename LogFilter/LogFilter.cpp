#include"LogFilter.h"

bool Logs::AddLog(string&  one_line);
{
    LogInfo temp;
    size_t begin_index, end_index;
    begin_index = one_line.find('[');
    if(index != 0)
    {
        return false;
    }
    end_index = one_line.find(']');
    if(end_index != kTimeLen + 1)
    {
        return false;
    }

    int time_stamp = StringToTimeStamp(one_line.substr(1, kTimeLen - 4));
    temp.time_ = time_stamp;
    
    begin_index = one_line.find('[', end_index);
    if(begin_index == string::npos || begin_index == one_line.size() - 1)
    {
        return false;
    }

    temp.level = one_line[begin_index + 1];

    begin_index = one_line.find('[', begin_index + 1);
    end_index = one_line.find(']', begin_index + 1);
    if(begin_index == string::npos || end_index == string::npos)
    {
        return false;
    }

    size_t mid_index = one_line.find(':', begin_index);
    temp.pid_ = atoi(one_line.substr(begin_index + 1, mid_index - begin_index - 1));
    temp.tid_ = atoi(one_line.substr(mid_index + 1, end_index - mid_index - 1));

    begin_index = one_line.find('[', begin_index + 1);
    end_index = one_line.find(']',begin_index + 1);
};
