//
// Created by ziyht on 17-2-8.
//

#include "test_main.h"



void ejson_sub_test()
{
    cstr  s = 0;
    ejson e = ejson_new(EOBJ, 0);

    ejson e0, e1, e2, e3;

    printf("--------- ejson_sub_test ------------\n"); fflush(stdout);

    e0 = ejson_addS(e, "0", "abcdasd");
    e1 = ejson_addS(e, "1", "abcd${PATH}asdasdf");
    e2 = ejson_addS(e, "2", "abcd${PATH}asdasdf${PATH}");
    e3 = ejson_addS(e, "3", "abcd${PATH}asdasdf${PATH}.../sdf///${PATH}fd%asd");
    printf("src: \n%s\n", ejson_toS(e, &s, PRETTY)); fflush(stdout);

//    ejso_subS(e0, "${PATH}", "${}");
//    ejso_subS(e1, "${PATH}", "${}");
//    ejso_subS(e2, "${PATH}", "${}");
//    ejso_subS(e3, "${PATH}", "${}");
    printf("\"${PATH}\" -> \"${}\":\n%s\n", ejson_toS(e, &s, PRETTY)); fflush(stdout);

//    ejso_subS(e0, "${}", "${PATH}");
//    ejso_subS(e1, "${}", "${PATH}");
//    ejso_subS(e2, "${}", "${PATH}");
//    ejso_subS(e3, "${}", "${PATH}");
    printf(":\"${}\" -> \"${PATH}\":\n%s\n", ejson_toS(e, &s, PRETTY)); fflush(stdout);

//    ejso_subS(e0, "${PATH}", "${abcd}");
//    ejso_subS(e1, "${PATH}", "${abcd}");
//    ejso_subS(e2, "${PATH}", "${abcd}");
//    ejso_subS(e3, "${PATH}", "${abcd}");
    printf(":\"${PATH}\" -> \"${abcd}\":\n%s\n", ejson_toS(e, &s, PRETTY)); fflush(stdout);

//    ejsk_subS(e, "0", "${abcd}", 0);
//    ejsk_subS(e, "1", "${abcd}", 0);
//    ejsk_subS(e, "2", "${abcd}", 0);
//    ejsk_subS(e, "3", "${abcd}", 0);
    printf("\"${abcd}\" -> \"\":\n%s\n", ejson_toS(e, &s, PRETTY)); fflush(stdout);

    ejson_free(e);
    estr_free(s);
}

int test_obj_sub(int argc, char* argv[])
{
    ejson_sub_test();

    return ETEST_OK;
}
