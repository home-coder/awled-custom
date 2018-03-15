/*************************************************************************
 > Filename   : sprop.c
 > Author     : oneface - one_face@sina.com
 > Company    : 一尊还酹江月
 > Time       : 2018-03-14 17:34:12
 ************************************************************************/

#include <stdio.h>
#include <cutils/properties.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

///data/property/persist

int main()
{
	fd_set rd;
	int fd = -1;
	struct timeval tv;
	int err;

	fd = open("/data/property/persist.sys.capcard", O_RDONLY);
	if (fd < 0) {
		printf("open failed\n");
	}
	FD_ZERO(&rd);
	FD_SET(fd,&rd);

	while (1) {
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		err = select(fd + 1,&rd,NULL,NULL,&tv);

		if(err == 0) //超时
		{
			printf("select time out!\n");
		}
		else if(err == -1)  //失败
		{
			printf("fail to select!\n");
		}
		else  //成功
		{
			printf("data is available!\n");
		}
	}

	return 0;
}
