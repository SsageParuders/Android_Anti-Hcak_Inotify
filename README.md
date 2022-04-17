## 常用监控项
|行为|含义|掩码|对应|
|-----|-----|-----|-----|
|IN_ACCESS|访问|0x00000001|fgets(buffer, sizeof(buffer), fp)|
|IN_OPEN|文件被打开|0x00000020|fopen(filename, "r")|
|IN_ATTRIB|数据变化|0x00000004|  |
|IN_CLOSE|文件关闭|IN_CLOSE_WRITE \| IN_CLOSE_NOWRITE|close(fd) 未监控到?|

## 检测范围

| 调用层 | 读取层 | 结果   |
| ------ | ------ | ------ |
| root   | root   | 可检测 |
| 用户   | root   | 可检测 |

`ps top pidof` 都会触发 IN_OPEN | IN_ACCESS

| 监控位置 | 系统行为是否触发 | ls /proc是否触发 |
| ------ | ------ | ------ |
| /proc|是|是|
| /proc/pid|是|否 |
|/proc/other process|inotify_add_watch err.|null|

系统行为指, 软件前后台切换等

## 绕过方案

> 修改/proc/sys/fs/inotify/max_user_watches为0

| 修改时间   | 结果                  |
| ---------- | --------------------- |
| 启动应用前 | inotify_add_watch err |
| 启动应用后 | IN_OPEN \| IN_ACCESS  |
| 获取文件句柄后 | IN_ACCESS|