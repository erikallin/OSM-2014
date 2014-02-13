#include "drivers/bootargs.h"
#include "drivers/device.h"
#include "drivers/gcd.h"
#include "drivers/metadev.h"
#include "drivers/polltty.h"
#include "drivers/yams.h"
#include "fs/vfs.h"
#include "kernel/assert.h"
#include "kernel/config.h"
#include "kernel/halt.h"
#include "kernel/idle.h"
#include "kernel/interrupt.h"
#include "kernel/kmalloc.h"
#include "kernel/panic.h"
#include "kernel/scheduler.h"
#include "kernel/synch.h"
#include "kernel/thread.h"
#include "lib/debug.h"
#include "lib/libc.h"
#include "net/network.h"
#include "proc/process.h"
#include "vm/vm.h"



int syscall_read(int fhandle, void *buffer, int length) {
if(fhandle == 0) {
device_t *dev;
gcd_t *gcd;

/* Skaffer device */
dev = device_get(YAMS_TYPECODE_TTY,0);
KERNEL_ASSERT(dev != NULL);
/* skaffer generisk device fra device */
gcd = (gcd_t *)dev->generic_device;
KERNEL_ASSERT(gcd != NULL);


/* iflg. drivers/gcd.h, læser read, "at most len bytes from the device to the buffer. the function returns the number of bytes read." */

return gcd->read(gcd,buffer,length);
}
return -1;

/*int tmp = fhandle;
int tmp2 =(int) buffer;
int tmp3 = length; */
}
int syscall_write(int fhandle, const void *buffer, int length) {
if(fhandle == 1) {
device_t *dev;
gcd_t *gcd;

/* Skaffer device */
dev = device_get(YAMS_TYPECODE_TTY,0);
KERNEL_ASSERT(dev != NULL);
/* skaffer generisk device fra device */
gcd = (gcd_t *)dev->generic_device;
KERNEL_ASSERT(gcd != NULL);


/* iflg. drivers/gcd.h, skriver write, "at most len bytes from the buffer to the device. the function returns the number of bytes read." */
return gcd->write(gcd,buffer,length);
}
return -1;

}

