#include "lib.h"
int main(void)
{
 int a[100];
    syscall_read(0,a,100); 
    syscall_write(1,a,100);
 return 0;
}

