#include "motion_algorithm.h"



/*=====================================================================*/

int enQueue() {
    int ret = 0;
    mgs_data raw_data;



    raw_data.data_set[0] = sys_read(MGS_X_SYS_PATH);
    raw_data.data_set[1] = sys_read(MGS_Y_SYS_PATH);
    raw_data.data_set[2] = sys_read(MGS_Z_SYS_PATH);
    raw_data.data_set[3] = sys_read(MGS_D_SYS_PATH);

//    mgs_fifo[index] = raw_data;
//    index = (index + 1) % FIFO_BUFF;



    return ret;
}
uint64_t getTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}
void print_cali() {

    ALOGI("MGSS_PRST = %d\n", MGSS_PRST);
    ALOGI("THRS_XXXX = %d\n", THRS_XXXX);
    ALOGI("THRS_YYYY = %d\n", THRS_YYYY);
    ALOGI("THRS_ZZZZ = %d\n", THRS_ZZZZ);
    ALOGI("CALB_BAND = %d\n", CALB_BAND);
    ALOGI("AVGS_AAAA = %d\n", AVGS_AAAA);
    ALOGI("AVGS_BBBB = %d\n", AVGS_BBBB);
    ALOGI("AVGS_CCCC = %d\n", AVGS_CCCC);
    ALOGI("AVGS_DDDD = %d\n", AVGS_DDDD);
    ALOGI("CALB_AAAA = %d\n", CALB_AAAA);
    ALOGI("CALB_BBBB = %d\n", CALB_BBBB);
    ALOGI("CALB_CCCC = %d\n", CALB_CCCC);
    ALOGI("CALB_DDDD = %d\n", CALB_DDDD);
    ALOGI("UP = %s\n", up);
    ALOGI("DOWN = %s\n", down);
    ALOGI("LEFT = %s\n", left);
    ALOGI("RIGHT = %s\n", right);
}
int init_cali() {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;


    fp = fopen(CALI_FILE_PATH, "r");
    if (fp == NULL)
	return -1;

    while ((read = getline(&line, &len, fp)) != -1) {

	strtok(line, " ");
	while (line != NULL)
	{
	    if(strcmp("MGSS_PRST", line) == 0) {
		line = strtok(NULL, " ");
		MGSS_PRST = atoi(line);
	    } if(strcmp("THRS_XXXX", line) == 0) {
		line = strtok(NULL, " ");
		THRS_XXXX = atoi(line);
	    } if(strcmp("THRS_YYYY", line) == 0) {
		line = strtok(NULL, " ");
		THRS_YYYY = atoi(line);
	    } if(strcmp("THRS_ZZZZ", line) == 0) {
		line = strtok(NULL, " ");
		THRS_ZZZZ = atoi(line);
	    } if(strcmp("AVGS_AAAA", line) == 0) {
		line = strtok(NULL, " ");
		AVGS_AAAA = atoi(line);
	    } if(strcmp("AVGS_BBBB", line) == 0) {
		line = strtok(NULL, " ");
		AVGS_BBBB = atoi(line);
	    } if(strcmp("AVGS_CCCC", line) == 0) {
		line = strtok(NULL, " ");
		AVGS_CCCC = atoi(line);
	    } if(strcmp("AVGS_DDDD", line) == 0) {
		line = strtok(NULL, " ");
		AVGS_DDDD = atoi(line);
	    } if(strcmp("CALB_BAND", line) == 0) {
		line = strtok(NULL, " ");
		CALB_BAND = atoi(line);
	    } if(strcmp("UP", line) == 0) {
		line = strtok(NULL, " ");
		up = line;
	    } if(strcmp("DOWN", line) == 0) {
		line = strtok(NULL, " ");
		down = line;
	    } if(strcmp("LEFT", line) == 0) {
		line = strtok(NULL, " ");
		left = line;
	    } if(strcmp("RIGHT", line) == 0) {
		line = strtok(NULL, " ");
		right = line;
	    }
	    else {
		line = strtok(NULL, " ");
	    }
	}
    }

    CALB_AAAA = AVGS_AAAA + CALB_BAND;
    CALB_BBBB = AVGS_BBBB + CALB_BAND;
    CALB_CCCC = AVGS_CCCC + CALB_BAND;
    CALB_DDDD = AVGS_DDDD + CALB_BAND;

    fclose(fp);
    if (line)
	free(line);

    return 0;


}
int sys_read(char* path) 
{
    int ret = 0;
    int fd = 0;
    char buf_r[4] = {0};

    fd = getFD(path);


    if (fd < 0) {
	ALOGE("couldn't open sysfs device");
	return -1;     
    }
    ret = read(fd, buf_r, sizeof(buf_r));
    if (ret < 0) {
	ALOGE("sysfs read failed");
	return -1;     
    }


    ret = (int)strtol(buf_r, NULL, 10);


    close(fd);
    return ret;

}
int getFD(char *path) {
    int fd = -1;
    char sysfs[PATH_MAX];
    strcpy(sysfs, path);

    fd = open(sysfs, O_RDWR);

    return fd;
}

char motion_algo() {
    char motion = '0';
	if((ptn1_motn > 0) && (ptn2_motn == 0)) {
	    if(ptn1_motn == 1) {
		motion = up[0];//Up
	    } else if(ptn1_motn == 2) {
		motion = down[0];//Down
	    }     
	}
	if((ptn2_motn > 0) && (ptn1_motn == 0)) {
	    if(ptn2_motn == 1){
		motion = right[0];//Right
	    } else if(ptn2_motn == 2) {
		motion = left[0];//Left
	    }     
	}

    return motion;
}



//=================================================================
// MGS 演算法
//  raws_ptnL : 左方的PS值 或 下方的PS值
//  raws_ptnR : 右方的PS值 或 上方的PS值
//	data_zzzz : Z軸值（正面的PSP值）
//	algo_type : 方向選擇
//			0	: X軸
//			1	: Y軸
//=================================================================
int mgss_algo(int raws_ptnL, int raws_ptnR, int data_zzzz, unsigned char algo_type)
{
    int	raws_algo;					//經處理過後的方向data
    int	raws_high;					//演算法判定方向的high threshold
    int	raws_lows;					//演算法判定方向的low  threshold
    int	raws_thrs;					//演算法的Z軸thrshold

    unsigned char	byte_vars;
    unsigned char	zone;			//raws_algo的區間與狀態
    unsigned char	prst_thrs;		//mgsXY軸persist

    if(algo_type == 0)
    {
	raws_high = THRS_XXXX;	//I2CF_0x48設定X軸 threshold		//fake
    }
    else
    {
	raws_high = THRS_YYYY;	//I2CF_0x4a設定Y軸 threshold		//fake
    }
    //-------------------------------------------------------------
    //	RAWS_ALGO = RAWS_PTNL - RAWS_PTNR	for X axis
    //	RAWS_ALGO = RAWS_PTND - RAWS_PTNU	for Y axis
    //-------------------------------------------------------------

    raws_algo = raws_ptnL - raws_ptnR;

    //=============================================================
    // 計算 raws_algo 所在的區間
    //=============================================================
    // raws_algo location
    //
    //	zone = 2
    // ---------------- +RAWS_HIGH
    //	zone = 0 or 3
    // ----------------	-RAWS_LOWS
    //	zone = 1
    //-------------------------------------------------------------
    raws_lows = -raws_high;

    raws_thrs = THRS_ZZZZ;	//ZTH								//fake

    if(raws_algo>=255)
	raws_algo = 255;


    if(raws_algo > raws_high)
	zone = 2;					//zone = 2，raws_algo 大於RAWS_HIGH
    else
    {
	if(raws_algo >= raws_lows)
	{
	    if(eror_flag==1)
		zone = 3;				//zone = 3，raws_algo 發生飽合
	    else
	    {
		if(data_zzzz>=raws_thrs)
		    zone = 3;		//zone = 3，raws_algo 介於RAWS_HIGH與RAWS_LOWS之間，並發生飽合（大於Z軸 threshold）
		else	
		    zone = 0;		//zone = 0，raws_algo 介於RAWS_HIGH與RAWS_LOWS之間
	    }			
	}
	else	zone = 1;				//zone = 1，raws_algo 小於RAWS_HIGH
    }

    //-------------------------------------------------------------
    // 對raws_pipe進行判斷，並修正zone的一些特殊狀態
    //	(n-2)	(n-1)   (n)
    //   01      11      10     ->  16進制為 0x2D = 30
    //	LOWS	EROR	HIGH	->  zone = 4 ( 判斷為 上 或 右 , + )
    //   10      11      01     ->  16進制為0x1E = 45
    //	HIGH	EROR	LOWS	->  zone = 5 ( 判斷為 下 或 左 , - )
    //-------------------------------------------------------------
    if(raws_stat < 4)
    {
	raws_motn = 0;
	if(raws_pipe==30)
    	    zone = 4;		//Up	/	Right
	else if(raws_pipe==45)
    	    zone = 5;		//Down 	/ 	Left
    }
    else
    {
	if(raws_stat== 4)
    	    raws_motn = 1;	//Up	/	Right
	else			
	    raws_motn = 2;	//Down 	/ 	Left
	zone = 0;
    }
    //-------------------------------------------------------------
    // persist
    //-------------------------------------------------------------
    if(raws_stat >= 4)
    {	
	prst_thrs = 0;//0x1D;	//HOLD									//fake

    }
    else
    {
	prst_thrs = 1;//PRST									//fake
    }	
    //-------------------------------------------------------------
    // raws_pipe
    // [7:6] : Force Zero
    // [5:4] : stat(n-2)
    // [3:2] : stat(n-1)
    // [1:0] : stat(n)
    //
    // zone , raws_stat , stat( n ) compare table
    //	  zone 	     raws_stat       stat( n )
    //     5         Output Motion       0
    //     4         Output Motion       0
    //     3           EROR              3 (EROR)
    //     2           HIGH              2 (HIGH)
    //     0           STOP              0 (STOP)
    //     1           LOWS              1 (LOWS)
    //------------------------------------------------------------- 
    if(zone!=raws_stat)
    {			
	if (raws_cont >= prst_thrs)
	{
	    raws_cont = 0;
	    raws_stat = zone;

	    if(zone>=4)
		byte_vars = 0;
	    else	
		byte_vars = zone;

	    raws_pipe = 4 * raws_pipe;

	    raws_pipe = raws_pipe | byte_vars;
	    raws_pipe = raws_pipe & 0x3f;

	}else	
	    raws_cont = raws_cont + 1;
    }else		
	raws_cont = 0;
    //=============================================================
    return(raws_algo);
}
//=================================================================


//====================================================================================
// 16 bit 乘除運算
// 輸入： 16 bit有號數
// 實現： outs(16bit) = word_var1(16bit) * word_var2(16bit) / word_var3(16bit)
//====================================================================================
short long_math(short word_var1, short word_var2, short word_var3)
{
    long			long_outs;		//使用32 bit long變數，防止乘積over flow
    int				word_outs;

    long_outs = (long)word_var1 * (long)word_var2;
    long_outs = 	  long_outs / (long)word_var3;

    word_outs =(short)long_outs;

    return word_outs;
}

//====================================================================================
// Kalm Filter
//====================================================================================
int kalm_fltr(int adcs_cont)
{
    int		raws_data;
    int		kalm_vars;
    int		kalm_coef;
    int		kalm_data;
    //---------------------------------------
    // KALMAN FILTER
    //---------------------------------------		

    raws_data = 1*adcs_cont;
    // Measurement update
    kalm_vars = kalm_pppp + RRRR_COEF;							//------4
    kalm_coef = long_math(         1 ,     16384 , kalm_vars );	//------4
    kalm_coef = long_math( kalm_coef , kalm_pppp ,         1 );	//------4

    kalm_vars = raws_data - kalm_xxxx;							//------3
    kalm_vars = long_math( kalm_coef , kalm_vars ,     16384 );	//------3
    kalm_xxxx = kalm_xxxx + kalm_vars;							//------3

    kalm_vars = long_math( kalm_coef , kalm_pppp ,     16384 );	//------5
    kalm_pppp = kalm_pppp - kalm_vars;							//------5

    // Compute output
    kalm_data = kalm_xxxx;										//------1

    // Time update
    kalm_xxxx = kalm_xxxx * 1;									//------2
    kalm_pppp = kalm_pppp + QQQQ_COEF;							//------2

    return (kalm_data);
    //---------------------------------------
}
//====================================================================================
// mgs flow function
//====================================================================================
void mgss_flow(int mgs_data_set[4])
{
    int		word_vars;
    int		raws_aaaa;
    int		raws_bbbb;
    int		raws_cccc;
    int		raws_dddd;
    int		raws_zzzz;

    int		raws_ptnL;
    int		raws_ptnR;
    int		raws_ptnD;
    int		raws_ptnU;
    int		raws_algx;
    int		raws_algy;
    //---------------------------------------
    // Accquiring ABCD's Data
    //---------------------------------------		
    eror_flag = sys_read(MGS_ERR_FLAG_PATH);//i2cm_read( ICID , 0x1E	);	//ERROR			//real
    raws_aaaa = mgs_data_set[0];//sys_read(MGS_X_SYS_PATH);//i2cm_read( ICID , 0x20	);	//A    			//real
    raws_bbbb = mgs_data_set[1];//sys_read(MGS_Y_SYS_PATH);//i2cm_read( ICID , 0x21	);	//B    			//real
    raws_cccc = mgs_data_set[2];//sys_read(MGS_Z_SYS_PATH);//i2cm_read( ICID , 0x22	);	//C    			//real
    raws_dddd = mgs_data_set[3];//sys_read(MGS_D_SYS_PATH);//i2cm_read( ICID , 0x23	);	//D    			//real

    //print the ABCD raw data

    ALOGI("r. %d, %d, %d, %d", raws_aaaa, raws_bbbb, raws_cccc, raws_dddd);



    //---------------------------------------
    // 取得Z的資料，並計算Z軸calibration
    //---------------------------------------
    raws_zzzz = raws_aaaa + raws_bbbb + raws_cccc + raws_dddd;
    raws_zzzz = raws_zzzz/2;

    word_vars = CALB_ZZZZ;			//fake
    if(raws_zzzz>word_vars)									//raws_zzzz = Z軸資料 - Z軸calibration
	raws_zzzz = raws_zzzz - word_vars;
    else	raws_zzzz = 0;	
    //---------------------------------------
    // 將ABCD資料經卡爾曼濾波器優化
    //---------------------------------------
    kalm_xxxx = aaaa_xxxx;
    kalm_pppp = aaaa_pppp;
    raws_aaaa = kalm_fltr( raws_aaaa );			//將A資料輸入到卡爾曼濾波器
    aaaa_xxxx = kalm_xxxx;
    aaaa_pppp = kalm_pppp;

    kalm_xxxx = bbbb_xxxx;
    kalm_pppp = bbbb_pppp;
    raws_bbbb = kalm_fltr( raws_bbbb );			//將B資料輸入到卡爾曼濾波器
    bbbb_xxxx = kalm_xxxx;
    bbbb_pppp = kalm_pppp;

    kalm_xxxx = cccc_xxxx;
    kalm_pppp = cccc_pppp;
    raws_cccc = kalm_fltr( raws_cccc );			//將C資料輸入到卡爾曼濾波器
    cccc_xxxx = kalm_xxxx;
    cccc_pppp = kalm_pppp;

    kalm_xxxx = dddd_xxxx;
    kalm_pppp = dddd_pppp;
    raws_dddd = kalm_fltr( raws_dddd );			//將D資料輸入到卡爾曼濾波器
    dddd_xxxx = kalm_xxxx;
    dddd_pppp = kalm_pppp;
    ALOGI("k. %d, %d, %d, %d", raws_aaaa, raws_bbbb, raws_cccc, raws_dddd);
    //---------------------------------------
    // 計算ABCD calibration（使用9960的方法）
    //---------------------------------------
    word_vars = CALB_AAAA;//取得A calibration						//fake
    if(raws_aaaa>word_vars)
	raws_aaaa = raws_aaaa - word_vars;	//raws_aaaa = A資料 - A calibration
    else	raws_aaaa = 0;

    word_vars = CALB_BBBB;//取得B calibration						//fake
    if(raws_bbbb>word_vars)
	raws_bbbb = raws_bbbb - word_vars;	//raws_bbbb = B資料 - B calibration
    else	raws_bbbb = 0;

    word_vars = CALB_CCCC;//取得C calibration						//fake
    if(raws_cccc>word_vars)
	raws_cccc = raws_cccc - word_vars;	//raws_cccc = C資料 - C calibration
    else	raws_cccc = 0;

    word_vars = CALB_DDDD;//取得D calibration						//fake
    if(raws_dddd>word_vars)
	raws_dddd = raws_dddd - word_vars;	//raws_dddd = D資料 - D calibration
    else	raws_dddd = 0;

    if(eror_flag>0)	raws_aaaa = 255;			//小於0，強制為0
    if(eror_flag>0)	raws_bbbb = 255;
    if(eror_flag>0)	raws_cccc = 255;
    if(eror_flag>0)	raws_dddd = 255;
    ALOGI("n. %d, %d, %d, %d", raws_aaaa, raws_bbbb, raws_cccc, raws_dddd);
    //---------------------------------------
    //X軸mgs演算法
    //	C  B
    //	D  A
    //	RAWS_PTNL = C + D
    //	RAWS_PTNR = B + A
    //	RAWS_PTNS = RAWS_PTNL - RAWS_PTNR
    //---------------------------------------
    raws_ptnL = raws_cccc + raws_dddd;
    raws_ptnR = raws_bbbb + raws_aaaa;

    raws_cont = ptn1_cont;
    raws_stat = ptn1_stat;
    raws_pipe = ptn1_pipe;
    raws_motn = ptn1_motn;
    raws_algx = mgss_algo( raws_ptnL , raws_ptnR , raws_zzzz , 0 );		//mgs演算法
    ptn1_cont = raws_cont;
    ptn1_pipe = raws_pipe;
    ptn1_stat = raws_stat;
    ptn1_motn = raws_motn;
    //---------------------------------------
    //Y軸mgs演算法
    //	C  B
    //	D  A
    //	RAWS_PTNU = C + B
    //	RAWS_PTND = D + A
    //	RAWS_PTNS = RAWS_PTND - RAWS_PTNU
    //---------------------------------------
    raws_ptnD = raws_cccc + raws_bbbb;
    raws_ptnU = raws_dddd + raws_aaaa; 

    raws_cont = ptn2_cont;
    raws_stat = ptn2_stat;
    raws_pipe = ptn2_pipe;
    raws_motn = ptn2_motn;
    raws_algy = mgss_algo( raws_ptnD , raws_ptnU , raws_zzzz , 1 );		//mgs演算法
    ALOGI(" raws_algx=  %d, raws_algy = %d, raws_zzzz = %d", raws_algx, raws_algy, raws_zzzz);
    ptn2_cont = raws_cont;
    ptn2_pipe = raws_pipe;
    ptn2_stat = raws_stat;
    ptn2_motn = raws_motn;
    //---------------------------------------
}
//====================================================================================
