#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>

#include "./include/mlog.h"

int readCmdline(const char *name, int pid);

//示例
int main(int args, char **argvs)
{

    LOGD("CHEATING START");

    system("echo 0 > /proc/sys/fs/inotify/max_user_watches");

    LOGD("Game PID: %d", readCmdline("Inotify", atoi(argvs[1])));

    return 0;
}

int readCmdline(const char *name, int pid)
{
    FILE *fp;
    DIR *dir;
    char filename[32];
    char cmdline[256];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
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
            if (strcmp(name, cmdline) == 0)
            {
                return pid;
            }
        }
    }
    closedir(dir);
    return -1;
}