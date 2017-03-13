#include "test_main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char* _llog_basename(const char* path){static const char* slash; if (slash) {return slash + 1;}else{slash = strrchr(path, '/');}if (slash) {return slash + 1;}return 0;}
#define log(fmt, ...)   fprintf(stdout, "%s(%d):" fmt "%s", _llog_basename(__FILE__), __LINE__, __VA_ARGS__)
#define llog(...)       log(__VA_ARGS__, "\n");fflush(stdout)


void cb1(void* _d)
{
    char* d = (char*) _d;

    llog("this is in cb1, data: \"%s\"", d);
    return ;
}

void after_cb1(void* _d)
{
    char* d = (char*) _d;

    llog("this is in after_cb1, data: \"%s\"", d);
    return ;
}

char d1[] = "this is d1 data";

void cb2(void* _d)
{
    char* d = (char*) _d;

    llog("this is in cb2, data: \"%s\"", d);
    sleep(1);
    return ;
}

void after_cb2(void* _d)
{
    char* d = (char*) _d;

    llog("this is in after_cb2, data: \"%s\"", d);
    return ;
}

char d2[] = "this is d2 data";


void ev_threadpool_test1()
{
    ert tp = ert_new(16);

    for(int i = 0; i < 100; i++)
    {
        ert_run(tp, 0, cb1, after_cb1, d1);
        ert_run(tp, 0, cb2, after_cb2, d2);
        ert_run(tp, "tag1", cb1, after_cb1, d1);
        ert_run(tp, "tag1", cb1, after_cb1, d1);
        ert_run(tp, "tag2", cb2, after_cb2, d2);
        ert_run(tp, "tag2", cb2, after_cb2, d2);

        usleep(1000);
    }

    sleep(1);

    ert_release(tp, ERT_WAITING_TASKS);
    ert_join(tp);

    sleep(2);
}


void cb_all(void* _d)       {llog("this is in cb   , data: \"%s\"", (cstr)_d);return ;}
void after_cb_all(void* _d) {llog("this is in after, data: \"%s\"", (cstr)_d);return ;}
void ev_threadpool_test2()
{
    ert tp = ert_new(16);

    for(int i = 0; i < 100; i++)
    {
        ert_run(tp, "cpu",          cb_all, after_cb_all, "cpu");
        ert_run(tp, "df",           cb_all, after_cb_all, "df");
        ert_run(tp, "dskio",        cb_all, after_cb_all, "dskio");
        ert_run(tp, "ipmi",         cb_all, after_cb_all, "ipmi");
        ert_run(tp, "meminfo",      cb_all, after_cb_all, "meminfo");
        ert_run(tp, "net",          cb_all, after_cb_all, "net");
        ert_run(tp, "nfs",          cb_all, after_cb_all, "nfs");
        ert_run(tp, "proc",         cb_all, after_cb_all, "proc");
        ert_run(tp, "microarch2",   cb_all, after_cb_all, "microarch2");
        ert_run(tp, "sysinfo",      cb_all, after_cb_all, "sysinfo");

        usleep(1000);

        puts("");
    }

    ert_release(tp, 0);
    ert_join(tp);

    sleep(2);
}

void ert_runtask_test()
{
    ev_threadpool_test1();

    ev_threadpool_test2();
}
