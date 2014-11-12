#ifndef _MOTION_ALGORITHM_H_
#define _MOTION_ALGORITHM_H_
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
#include <cutils/log.h>


//====================================================================================
//I2C設定值
//====================================================================================
int LEDS_WDTH = 55;//I2CF_0x15設定mgs LED width

int MGSS_PRST = 1;//I2CF_0x17設定mgsXY軸persist		

int THRS_XXXX = 15;//I2CF_0x48設定X軸 threshold
int THRS_YYYY = 15;//I2CF_0x4a設定Y軸 threshold	  
int THRS_ZZZZ = 185;//I2CF_0x4b設定Z軸 threshold

int CALB_BAND = 5;//取 5 Counter 做 calibration 設定值的BUFF
int AVGS_AAAA = 29;//A channel data平均值，用來當作A  calibration設定值
int AVGS_BBBB = 30;//B channel data平均值，用來當作B  calibration設定值
int AVGS_CCCC = 31;//C channel data平均值，用來當作C  calibration設定值
int AVGS_DDDD = 30;//D channel data平均值，用來當作D  calibration設定值

int CALB_ZZZZ = 18;//I2CF_0x3f設定Z軸calibration
int CALB_AAAA = 0;// = AVGS_AAAA + CALB_BAND;//I2CF_0x3a設定A  calibration
int CALB_BBBB = 0;// = AVGS_BBBB + CALB_BAND;//I2CF_0x3b設定B  calibration
int CALB_CCCC = 0;// = AVGS_CCCC + CALB_BAND;//I2CF_0x4e設定C  calibration
int CALB_DDDD = 0;// = AVGS_DDDD + CALB_BAND;//I2CF_0x4f設定D  calibration
char* up = "1";
char* down = "2";
char* right = "3";
char* left = "4";

//====================================================================================
//Kalm filter
//====================================================================================

#define		QQQQ_COEF	1			//kalm_fltr's Q factor
#define		RRRR_COEF	4			//kalm_fltr's R factor (the bigger R is the slower the convergence，but also the smoother output)
#define		XXXX_INIT	0			//kalm_fltr initial value of X
#define		PPPP_INIT	QQQQ_COEF	//kalm_fltr initial value of P

//Globe valuables
int			kalm_xxxx;				//kalm X
int			kalm_pppp;				//kalm P

int			aaaa_xxxx;				//A channel kalm X
int			aaaa_pppp;				//A channel kalm P
int			bbbb_xxxx;				//B channel kalm X
int			bbbb_pppp;				//B channel kalm P
int			cccc_xxxx;				//C channel kalm X
int			cccc_pppp;				//C channel kalm P
int			dddd_xxxx;				//D channel kalm X
int			dddd_pppp;				//D channel kalm P
//====================================================================================
//MGS Algorithm
//====================================================================================
//global valuable
unsigned char	raws_cont;			//mgsXY axis persist counter
unsigned char	raws_stat;			//mgsXY axis status
unsigned char 	raws_pipe;			//mgsXY axis pipeline，buffering the state of three times before
unsigned char 	raws_motn;			//mgsXY direction

unsigned char	ptn1_cont;
unsigned char	ptn1_stat;
unsigned char 	ptn1_pipe;
unsigned char 	ptn1_motn;

unsigned char	ptn2_cont;
unsigned char	ptn2_stat;
unsigned char 	ptn2_pipe;
unsigned char 	ptn2_motn;

unsigned int	eror_flag;
//====================================================================================





/*=====================================================================*/
#define MOTION_SYS_PATH "/sys/class/sensors/di_sensors/motion"
#define MGS_X_SYS_PATH "/sys/class/sensors/di_sensors/mgs_x"
#define MGS_Y_SYS_PATH "/sys/class/sensors/di_sensors/mgs_y"
#define MGS_Z_SYS_PATH "/sys/class/sensors/di_sensors/mgs_z"
#define MGS_D_SYS_PATH "/sys/class/sensors/di_sensors/mgs_d"
#define MGS_ERR_FLAG_PATH "/sys/class/sensors/di_sensors/err_flag"
#define MGS_INT_PATH "/sys/class/sensors/di_sensors/motion_int"
#define MGS_SYS_PATH "/sys/class/sensors/di_sensors/mode"
#define CALI_FILE_PATH "/sdcard/cali.txt"
#define PATH_MAX 200
#define MAX_BUFF 20
#define UP    '1'
#define DOWN  '2'
#define RIGHT '3'
#define LEFT  '4'
#define LOG_TAG "MGS" 
#define FIFO_BUFF 1000


typedef struct {
    int data_set[4];
} mgs_data;

mgs_data mgs_fifo[FIFO_BUFF] = {NULL};
//int index = 0;

int motion_fd, interrupt_fd, mgs_x_fd, mgs_y_fd, mgs_z_fd, mgs_d_fd, err_flag_fd;

char motion_algo();
int getFD(char *path);
short long_math(short word_var1, short word_var2, short word_var3);
int kalm_fltr(int adcs_cont);
void mgss_flow(int mgs_data_set[4]);
int mgss_algo(int raws_ptnL, int raws_ptnR, int data_zzzz, unsigned char algo_type);
int sys_read(char* path);
int enQueue();
int deQueue();
uint64_t getTimeStamp();
int init_cali();
void print_cali();
#endif
