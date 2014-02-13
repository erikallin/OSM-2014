#include "lib.h"

int main(void)
{
 int i;
 int a[100];
 for(i = 0; i < 100; i++) {
    a[i] = 'a';
 }
    syscall_write(1,a,100);
 return 0;
}

