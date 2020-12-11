#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "swapper.h"

int swap_files(const char* pathname1, const char* pathname2)
{
	const int step = 8;
	int i = 0;
	int fd1, fd2;
	int f1e, f2e;
	int le, be;
	fd1 = open(pathname1, O_RDWR);
	fd2 = open(pathname2, O_RDWR);
	if(fd1 == -1)
	{
		write(0, "can't open file 1\n", 18);
		close(fd1);
		close(fd2);
		return -1;
	}
	if(fd2 == -1)
	{
		write(0, "can't open file 1\n", 18);
		close(fd1);
		close(fd2);
		return -1;
	}
	f1e = lseek(fd1, 0, SEEK_END);
	f2e = lseek(fd2, 0, SEEK_END);
	le = f1e;
	be = f2e;
	if(f2e < f1e)
	{
		le = f2e;
		be = f1e;
	}
	int sub = le % step;
	for(; i <= le - step; i += step)
	{
		swap(step, &fd1, &fd2, i);
	}
	swap(sub, &fd1, &fd2, i);
	i += sub;
	if(f1e == f2e)
	{
		close(fd1);
		close(fd2);
		return 0;
	}
	for(; i <= be - step; i += step)
	{
		if(f1e > f2e)
			transfer(step, &fd1, &fd2, i);
		else
			transfer(step, &fd2, &fd1, i);
	}
	sub = be - i;
	if(sub == 0)
	{
		if(f1e > f2e)
			truncate(pathname1, le);
		else
			truncate(pathname2, le);
		close(fd1);
		close(fd2);
		return 0;
	}
	if(f1e > f2e)
	{
		transfer(sub, &fd1, &fd2, i);
		truncate(pathname1, le);
	}
	else
	{
		transfer(sub, &fd2, &fd1, i);
		truncate(pathname2, le);
	}
	close(fd1);
	close(fd2);
	return 0;
}
