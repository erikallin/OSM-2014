#include "lib/lib.c"

#include "io.h"

int syscall_read(int fhandle, void *buffer, int length) {
  kread("hej",4);
  return -1;
}

int syscall_write(int fhandle, const void *buffer, int length) {
  kwrite("top lelz");
  return -1;
}
