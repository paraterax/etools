#include "test_main.h"

static constr _llog_basename(constr path){static constr slash; if (slash) {return slash + 1;}else{slash = strrchr(path, '/');}if (slash) {return slash + 1;}return 0;}
#define log(fmt, ...)   fprintf(stdout, "%s(%d):" fmt "%s", _llog_basename(__FILE__), __LINE__, __VA_ARGS__)
#define llog(...)       log(__VA_ARGS__, "\n");fflush(stdout)

#define MAX_CONN 2048
#define MAX_TEST 200

static void closedCB      (enats t, void* closure __unused)
{
    //printf("%s:%s connection closed\n", enats_name(t), enats_lasturl(t)); fflush(stdout);
}

void server_max_collect_test()
{
    cstr url = "nats://localhost:4242";

    int i, j;

    enats* ntSet = calloc(MAX_CONN, sizeof(*ntSet));

    for (i = 0; (ntSet[i] = enats_new(url)) && i < MAX_TEST - 1;)
    {
        enats_setClosedCB(ntSet[i], closedCB, 0);

        i++;

        if(i % 100 == 0 )
            llog("connected: %d", i);
    }
    llog("connected: %d\n", i + 1);

    for(j = 0; j <= i ; j++)
    {
        enats_destroy(ntSet[j]);

        if(j % 100 == 0 )
            llog("destroyed: %d", j);

    }
    llog("destroyed: %d", j);

    free(ntSet);

    sleep(1);
}