#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "swapper.h"

void swap(int swapstep, int *fd1, int *fd2, int ind)
{
	char buf1[swapstep + 1], buf2[swapstep + 1];
	pread(*fd1, buf1, swapstep, ind);
	pread(*fd2, buf2, swapstep, ind);
	pwrite(*fd1, buf2, swapstep, ind);
	pwrite(*fd2, buf1, swapstep, ind);
}