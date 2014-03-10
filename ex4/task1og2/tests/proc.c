 
  #include "tests/lib.h"
  
 -static const char prog[] = "[disk1]hw"; /* The program to start. */
 +//static const char prog[] = "[disk1]hw"; /* The program to start. */
  
  int main(void)
  {
 -  uint32_t child;
 +// pid_t abe =  syscall_exec("[disk1]hw");
 + // syscall_join(abe);
 +  int a = 4096*20;
 +  syscall_memlimit((void*)a);
 +  puts("hej\n");
 +  syscall_memlimit((void*)a);
 +  puts("hej selv");

    return 0;
  }