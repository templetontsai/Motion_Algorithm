#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#include <linux/input.h>

#include <utils/Atomic.h>
#include <utils/Log.h>
#include "motion_algorithm.h"


/*=====================================================================*/

int main() {

    int ret;
    int motion_fd = 0;
    int mode_fd = 0;
    char buf_w[2];
    struct timeval start, end;
    int data[4] ={0};


    mode_fd = getFD(MGS_SYS_PATH);

    if (mode_fd < 0) {
	ALOGE("couldn't open mode_fd sysfs device");
	return -1;     
    }

    buf_w[0] = '8';
    buf_w[1] = '\0';
    write(mode_fd, buf_w, sizeof(buf_w));

    motion_fd = getFD(MOTION_SYS_PATH);

    if (motion_fd < 0) {
	ALOGE("couldn't open motion_fd sysfs device");
	return -1;     
    }

    if(init_cali() < 0)
	ALOGI("No cali file has been read, use the default values");
    print_cali();

    while(1) {

	if(sys_read(MGS_INT_PATH)) {
	    //enQueue();
	    data[0] = sys_read(MGS_X_SYS_PATH);
	    data[1] = sys_read(MGS_Y_SYS_PATH);
	    data[2] = sys_read(MGS_Z_SYS_PATH);
	    data[3] = sys_read(MGS_D_SYS_PATH);
	    gettimeofday(&start, NULL);
	    ALOGI("time: start = %ld\n", start.tv_sec * 1000000 + start.tv_usec);
	    mgss_flow(data);
	    gettimeofday(&end, NULL);
	    ALOGI("time: end = %ld\n", end.tv_sec * 1000000 + end.tv_usec);
	    ALOGI("time: diff = %ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
					  - (start.tv_sec * 1000000 + start.tv_usec)));
	    buf_w[0] = motion_algo();
	    buf_w[1] = '\0';
	    write(motion_fd, buf_w, sizeof(buf_w));
	}

    }

    return 0;
}
