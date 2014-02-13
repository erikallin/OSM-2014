#ifndef IO_H
#define IO_H

int syscall_read(int fhandle, void *buffer, int length);

int syscall_write(int fhandle, const void *buffer, int length);

#endif // IO_H
