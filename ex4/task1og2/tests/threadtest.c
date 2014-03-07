
#include "tests/lib.h"
static const char prog[] = "[disk1]hw"; /* The program to start. */

int main(void)
{
  uint32_t child;
  uint32_t child2;
  uint32_t child3;
  uint32_t child4;
  int ret;
  int ret2;
  int ret3;
  int ret4;

  printf("Starting program %s\n", prog);
  child = syscall_exec(prog);
  child2 = syscall_exec(prog);
  child3 = syscall_exec(prog);
  child4 = syscall_exec(prog);
  printf("Now joining child %d\n", child);
  
  ret = (char)syscall_join(child);
  ret2 = (char)syscall_join(child2);
  ret3 = (char)syscall_join(child3);
  ret4 = (char)syscall_join(child4);
  printf("Child joined with status: %d\n", ret);
  printf("Child joined with status: %d\n", ret2);
  printf("Child joined with status: %d\n", ret3);
  printf("Child joined with status: %d\n", ret4);
  syscall_halt();
  return 0;
}
