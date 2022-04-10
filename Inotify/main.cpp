#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <pthread.h>

#include "./include/mlog.h"

void* thread_watchInotifyDump(void* arg);

//示例
int main()
{

    LOGD("INOTIFY START");

    pthread_t ptPageMap;

    int iRet = 0;


    iRet = pthread_create(&ptPageMap, NULL, thread_watchInotifyDump, NULL);

    if (0 != iRet)
    {

        LOGE("Create,thread_watchDumpPagemap,error!\n");

        return 0;
    }

    iRet = pthread_detach(ptPageMap);

    if (0 != iRet)
    {

        LOGE("pthread_detach,thread_watchDumpPagemap,error!\n");
        return 0;
    }

    LOGD("pid:%d\n", getpid());

    sleep(1000);

    return 0;
}


void* thread_watchInotifyDump(void* arg)
{

    char dirName[30] = {0};
    //用于监控/proc/pid/XX的数据
    snprintf(dirName, 30, "/proc/%d/cmdline", getpid());

    int fd = inotify_init();
    if (fd < 0)
    {
        LOGE("inotify_init err.\n");
        return 0;
    }

    LOGD("等待Cheating读 sleep 10s");

    sleep(10);

    system("echo 8192 > /proc/sys/fs/inotify/max_user_watches");//每个真实用户 ID 创建的监控项数量的限制值

    LOGD("开始监控");

    int wd = inotify_add_watch(fd, dirName, IN_ALL_EVENTS);
    if (wd < 0)
    {

        LOGE("inotify_add_watch err.\n");
        close(fd);

        return 0;
    }

    const int buflen = sizeof(struct inotify_event) * 0x100;
    char buf[buflen] = {0};
    fd_set readfds;
    int count[5] = {0};

    while (1)
    {

        FD_ZERO(&readfds);

        FD_SET(fd, &readfds);

        int iRet = select(fd + 1, &readfds, 0, 0, 0); // 此处阻塞

        LOGD("iRet的返回值:%d\n", iRet);
        if (-1 == iRet)
            break;

        if (iRet)
        {

            memset(buf, 0, buflen);
            int len = read(fd, buf, buflen);

            int i = 0;

            while (i < len)
            {

                struct inotify_event *event = (struct inotify_event *)&buf[i];

                LOGD("event mask:%d\n", event->mask);

                if ((event->mask & IN_ACCESS))
                {

                    ++count[0];
                    LOGD("1.IN_ACCESS,第%d次.\n\n", count[0]);

                    //__asm __volatile(".int 0x8c89fa98");
                }

                if ((event->mask & IN_OPEN))
                {
                     ++count[1];
                    LOGD("2.IN_OPEN,第%d次.\n\n",  count[1]);
                }

                if ((event->mask & IN_CLOSE))
                {
                     ++count[2];
                    LOGD("3.IN_CLOSE,第%d次.\n\n", count[2]);
                }

                i += sizeof(struct inotify_event) + event->len;
            }
        }
    }

    inotify_rm_watch(fd, wd);//移出监控
    close(fd);

    return 0;
}

/********************************************************************************************
 * 
 *  在JNI中使用
 * 
 * ********************************************************************************************
 
 JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
    LOGI("jni onload called");
    JNIEnv* env = NULL; //注册时在JNIEnv中实现的，所以必须首先获取它
    jint result = -1;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) { //从JavaVM获取JNIEnv，一般使用1.4的版本
        return -1;
    }

    LOGD("START");

    pthread_t ptMem, t, ptPageMap;

    int iRet = 0;

    // 监控/proc/pid/pagemap

    iRet = pthread_create(&ptPageMap, NULL, thread_watchInotifyDump, NULL);

    if (0 != iRet)
    {

        LOGE("Create,thread_watchDumpPagemap,error!\n");

        return 0;
    }

    iRet = pthread_detach(ptPageMap);

    if (0 != iRet)
    {

        LOGE("pthread_detach,thread_watchDumpPagemap,error!\n");
        return 0;
    }

    LOGD("pid:%d\n", getpid());


    LOGI("jni onload called end...");
    return JNI_VERSION_1_4; //这里很重要，必须返回版本，否则加载会失败。
}

**/
