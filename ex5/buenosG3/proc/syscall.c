/*
* System calls.
*
* Copyright (C) 2003 Juha Aatrokoski, Timo Lilja,
*   Leena Salmela, Teemu Takanen, Aleksi Virtanen.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above
*    copyright notice, this list of conditions and the following
*    disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 3. The name of the author may not be used to endorse or promote
*    products derived from this software without specific prior
*    written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* $Id: syscall.c,v 1.3 2004/01/13 11:10:05 ttakanen Exp $
*
*/
#include "kernel/cswitch.h"
#include "proc/syscall.h"
#include "kernel/halt.h"
#include "kernel/panic.h"
#include "lib/libc.h"
#include "kernel/assert.h"
#include "proc/process.h"
#include "drivers/device.h"
#include "drivers/gcd.h"
#include "fs/vfs.h"
#include "kernel/thread.h"
#include "kernel/semaphore.h"

#define MAX_SEM 10

typedef struct {
  char const* name;
  semaphore_t* sem;
} usr_sem_t;

usr_sem_t semlist[MAX_SEM];

int find_free_block() {
  for(int i = 0; i < MAX_SEM; i++) {
    if (semlist[i].name == NULL)
      return i;
  }
  return -1;
}

void* find_existing_block(char const* name) {
  for(int i = 0; i < MAX_SEM; i++) {
    if(semlist[i].name == name)
    return &semlist[i];
  }
  return (int*)-1;
}

usr_sem_t* syscall_sem_open(char const* name, int value) {
  if (value >= 0) {
    if(!((int*)find_existing_block(name) == (int*)-1))
    return NULL;
    
    int i = find_free_block();
    semlist[i].sem = (semaphore_t*) semaphore_create(value);
    semlist[i].name = name;
    return &semlist[i];
  }
  return find_existing_block(name);
}

int syscall_sem_p(usr_sem_t* handle) {
  semaphore_P(handle->sem);
  return 0;
}

int syscall_sem_v(usr_sem_t* handle) {
  semaphore_V(handle->sem);
  return 0;
}

int syscall_sem_destroy(usr_sem_t* handle) {
  if(handle->sem->value <= 0)
  return -1;
  semaphore_destroy(handle->sem);
  handle->name = NULL;
  return 0;
}


int syscall_open(char const *pathname) {
  return (int) (vfs_open((char*)pathname)+3);
}

int syscall_close(int filehandle) {
  return (int) vfs_close(filehandle-3);
}

int syscall_write(int filehandle, void const *buffer, int length) {
  gcd_t *gcd;
  device_t *dev;
  if (filehandle == FILEHANDLE_STDOUT || filehandle == FILEHANDLE_STDERR) {
    dev = device_get(YAMS_TYPECODE_TTY, 0);
    gcd = (gcd_t *)dev->generic_device;
    return gcd->write(gcd, buffer, length);
  } else {
  return (int) vfs_write(filehandle-3,(void*) buffer, length);
  }
}

int syscall_read(int filehandle, void *buffer, int length) {
  gcd_t *gcd;
  device_t *dev;
  if (filehandle == FILEHANDLE_STDIN) {
    dev = device_get(YAMS_TYPECODE_TTY, 0);
    gcd = (gcd_t *)dev->generic_device;
    return gcd->read(gcd, buffer, length);
  } else {
  return (int) vfs_read(filehandle-3, buffer, length);
  }
}

int syscall_seek(int filehandle, int offset) {
  return (int) vfs_seek(filehandle-3, offset);
}

int syscall_create(char const *pathname, int size) {
  return (int) vfs_create((char*)pathname, size);
}

int syscall_delete(char const *pathname) {
  return (int) vfs_remove((char*)pathname);
}


/* Task 2 */
int syscall_filecount(char const *name) {
   return (int) vfs_filecount((char*) name); 
} 

//int syscall_file(char const* name, int index, char* buffer) {


 // return 0;
//}

/**
* Handle system calls. Interrupts are enabled when this function is
* called.
*
* @param user_context The userland context (CPU registers as they
* where when system call instruction was called in userland)
*/
void syscall_handle(context_t *user_context)
{
  int A1 = user_context->cpu_regs[MIPS_REGISTER_A1];
  int A2 = user_context->cpu_regs[MIPS_REGISTER_A2];
  int A3 = user_context->cpu_regs[MIPS_REGISTER_A3];

  #define V0 (user_context->cpu_regs[MIPS_REGISTER_V0])

  /* When a syscall is executed in userland, register a0 contains
  * the number of the syscall. Registers a1, a2 and a3 contain the
  * arguments of the syscall. The userland code expects that after
  * returning from the syscall instruction the return value of the
  * syscall is found in register v0. Before entering this function
  * the userland context has been saved to user_context and after
  * returning from this function the userland context will be
  * restored from user_context.*/
  switch(user_context->cpu_regs[MIPS_REGISTER_A0])
  {
    case SYSCALL_HALT:
    halt_kernel();
    break;
    case SYSCALL_EXEC:
    V0 = process_spawn((char *)A1);
    break;
    case SYSCALL_EXIT:
    process_finish(A1);
    break;
    case SYSCALL_JOIN:
    V0 = process_join(A1);
    break;
    case SYSCALL_READ:
    V0 = syscall_read(A1, (char *)A2, A3);
    break;
    case SYSCALL_WRITE:
    V0 = syscall_write(A1, (char *)A2, A3);
    break;
    case SYSCALL_SEM_OPEN:
    V0 = (int)syscall_sem_open((char const*) A1,(int)A2);
    break;
    case SYSCALL_SEM_PROCURE:
    V0 = syscall_sem_p((usr_sem_t*) A1);
    break;
    case SYSCALL_SEM_VACATE:
    V0 = syscall_sem_v((usr_sem_t*) A1);
    break;
    case SYSCALL_SEM_DESTROY:
    V0 = syscall_sem_destroy((usr_sem_t*) A1);
    break;
    case SYSCALL_CREATE:
    V0 = syscall_create((char const*) A1, (int) A2);
    break;
    case SYSCALL_DELETE:
    V0 = syscall_delete((char const*) A1);
    break;
    case SYSCALL_SEEK:
    V0 = syscall_seek((int) A1, (int) A2);
    break;
    case SYSCALL_OPEN:
    V0 = syscall_open((char const*) A1);
    break;
    case SYSCALL_CLOSE:
    V0 = syscall_close((int) A1);
    break;
    case SYSCALL_FILECOUNT: 
    V0 = syscall_filecount((char const*) A1);
    break;
    case SYSCALL_FILE:
  //  V0 = syscall_file((char const*) A1, (int) A2, (char*) A3);
    break;
    default:
    KERNEL_PANIC("Unhandled system call\n");
  }

  /* Move to next instruction after system call */
  user_context->pc += 4;

  #undef V0

}
