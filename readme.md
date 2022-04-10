## 检测范围

| 调用层 | 读取层 | 结果   |
| ------ | ------ | ------ |
| root   | root   | 检测到 |
| 用户   | root   | 检测到 |

## 修改/proc/sys/fs/inotify/max_user_watches

| 修改时间   | 结果                  |
| ---------- | --------------------- |
| 启动应用前 | inotify_add_watch err |
| 启动应用后 | 检测到                |
|            |                       |
