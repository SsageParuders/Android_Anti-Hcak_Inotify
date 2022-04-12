#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>

#include <mlog.h>

int cmpCmdline(const char *shell, int pid);//如果命令相同,返回PID | 否则-1

//示例
int main(int args, char **argvs)
{

    LOGD("CHEATING START");

    system("echo 0 > /proc/sys/fs/inotify/max_user_watches");//每个真实用户 ID 创建的监控项数量的限制值

    LOGD("Game PID: %d", cmpCmdline("./Inotify", atoi(argvs[1])));

    return 0;
}

int cmpCmdline(const char *shell, int pid)
{
    FILE *fp;
    DIR *dir;
    char filename[32];
    char cmdline[256];
    struct dirent *entry;
    dir = opendir("/proc");
    if ((entry = readdir(dir)) != NULL)
    {
        sprintf(filename, "/proc/%d/cmdline", pid);
        fp = fopen(filename, "r");
        LOGD("等待inotify sleep 10s");
        sleep(10);
        LOGD("开始读取");
        if (fp)
        {
            fgets(cmdline, sizeof(cmdline), fp);
            fclose(fp);
            LOGD("cmdline:%s", cmdline);
            if (0 == strcmp(shell, cmdline))
            {
                return pid;
            }
        }
    }
    closedir(dir);
    return -1;
}