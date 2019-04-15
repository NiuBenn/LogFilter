# 功能

假设一个日志文件中的日志格式为：

[time][level][pid:tid] [version][module][tag] log message

下面是具体的列子：

    [2019-04-15 14:43:19.705][I][31592:031592] [U.0.0.0][lapp][CatchAllException] call: InitConfig
    [2019-04-15 14:43:19.705][I][31592:031592] [U.0.0.0][wsrpc][NetworkAdaptor] Construct NetworkAdaptor:
    [2019-04-15 14:43:19.705][I][31592:031592] [U.0.0.0][wsrpc][AdaptiveServiceObserver][0x55775883d400] AdaptiveServiceObserver Construct
    [2019-04-15 14:43:19.705][I][31592:031592] [U.0.0.0][localsdk][ApplicationService][Init:41] initialized
    [2019-04-15 14:43:19.705][I][31592:031592] [U.0.0.0][wsrpc][AdaptiveServiceObserver] observe: ::maxhub::system::AppManager
    [2019-04-15 14:43:19.705][I][31592:031592] [U.0.0.0][localsdk][ChannelService][Init:73] initialized
    [2019-04-15 14:43:19.705][I][31592:031603] [U.0.0.0][wsrpc][AdaptiveService] Change To ReadyState


请写一个日志过滤器，用于过滤出想要的日志

# 功能要求

- 能够过滤出给定时间点附近的日志
- 能够过滤出给定level、进程、线程、版本、模块、TAG的日志
- 能够通过任意组合的方式过滤信息

## 额外工作

- 可以实时动态的过滤日志

# 注意

日志文件中可能会出现一些不符合上述规范的日志
