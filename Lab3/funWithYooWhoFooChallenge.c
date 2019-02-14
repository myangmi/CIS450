//
// funWithYooWhoFoo.c - fun with function calls
//
#include <stdio.h>
#include <stdlib.h>

void foo()
{
    static int foo_cnt = 0;
    foo_cnt++;
    printf("Now inside foo() - count = %d !!\n", foo_cnt);
}

void who()
{
    static int who_cnt = 0;
    who_cnt++;
    printf("Now inside who() - count = %d !\n", who_cnt);
}

void yoo()
{
    void *addr[5];
    printf("Now inside yoo() !\n");
    // you can only modify this section

	addr[6] = who;
    addr[7] = foo;

    return;
}

int main (int argc, char *argv[])
{
    void *space[234];
    yoo();
    printf("Back in main\n");
    return 0;
}
