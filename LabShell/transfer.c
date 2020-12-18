#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "include/swapper.h"

void transfer(int swapstep, int *fd1, int *fd2, int ind)
{
	char buf[swapstep + 1];
	pread(*fd1, buf, swapstep, ind);
	pwrite(*fd2, buf, swapstep, ind);
}