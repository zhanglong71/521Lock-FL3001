
#include "stm32f10x_lib.h" 
#include "string.h"
#include "../inc/CONST.h"
#include "../inc/ctype.h" 
#include "../inc/global.h" 
#include "../inc/debug.h"

#include "l_fingprint.h"
#include "charQue.h"

/**********************************************************************
 * 指纹模板索引表。
 * 0..57对应58个指纹信息
 **********************************************************************/
void flashCache_init(void)
{
	int i;
	for(i = 0; i < CFLASH_PAGE_SIZE; i++)g_flash.arrChar[i] = CENTRYFLAG_IDLE;
}
/**********************************************************************
 * name: fingerTab_empty 
 * description: check weather the fingerprint REPO empty
 **********************************************************************/
int fingerTab_isEmpty(void)
//int fingerTab_empty(void)
{
	return	((memcmp(g_flash.arrChar, g_magic, MAGIC_SIZE) != 0) || 
				g_flash.arrChar[CFLASH_FPIndex_START] == CENTRYFLAG_IDLE);
	//return	((memcmp(g_flash.arrChar, g_magic, MAGIC_SIZE) != 0) || g_flash.arrChar[CFLASH_FPIndex_START] != CENTRYFLAG_BUSY);
}

unsigned char fingerTab_getEntry(int idx)
{
	return	g_flash.arrChar[idx + CFLASH_FPIndex_START];
}

void fingerTab_setEntry(int idx, unsigned char value)
{
	g_flash.arrChar[idx + CFLASH_FPIndex_START] = value;
}
/*******************************************************************************
 * 对索引表中的指定位置的数据赋值
 *******************************************************************************/
 #if	1
void fingerTab_fill(unsigned char Manager_ID, unsigned char value)
{
	int i;
	int begin = 0;
	int end = CTOTALFINGER - 1;
	
	if(Manager_ID == 0)                 //第一管理者删除其它所有指纹
    {
    	begin = 1;
    	end = CTOTALFINGER - 1;
    }
    else if(Manager_ID == 1)      		//第二管理者删除其所录的指纹
    {
    	begin = C2NDMANAGES;
    	end = C2NDMANAGEE;	/** 取剩下的总数的一半，加上第一管理者的管理总数就得到索引号上限 **/
    }
    else if(Manager_ID == 2)      		//第三管理者删除其所录的指纹
    {
    	begin = C3RDMANAGES;
    	end = C3RDMANAGEE;
    }
    
	for(i = begin; i <= end; i++)			//清除索引表
	{
		g_flash.arrChar[i + CFLASH_FPIndex_START] = value;
	}
	
}
#endif

/*******************************************************************************
 * 判断指定的目录项是否空闲
 *******************************************************************************/
unsigned char fingerTab_entryIsEmpty(int idx)
{
	//return	(g_flash.arrChar[idx + CFLASH_FPIndex_START] == CENTRYFLAG_IDLE);
	return	(g_flash.arrChar[idx + CFLASH_FPIndex_START] != CENTRYFLAG_BUSY);
}

/**********************************************************************
 * name: fingprintCmd
 * description:	fingerprint operation command
 * input: cmd
 * output: no
 * return: no
 * date/time: 
 **********************************************************************/
 #if	0
void fingprintCmd(unsigned char *cmd)
{
	struct charData_s charData;
	
	charData.ucVal = 0x3a;
	charQueueIn_irq(&g_com2TxQue, &charData); 
	charData.ucVal = 0xff;
	charQueueIn_irq(&g_com2TxQue, &charData); 
	charData.ucVal = 0xff;
	charQueueIn_irq(&g_com2TxQue, &charData); 
	charData.ucVal = 0xff;
	charQueueIn_irq(&g_com2TxQue, &charData); 
	charData.ucVal = cmd[0];
	charQueueIn_irq(&g_com2TxQue, &charData); 
	charData.ucVal = cmd[1];
	charQueueIn_irq(&g_com2TxQue, &charData); 
	charData.ucVal = cmd[2];
	charQueueIn_irq(&g_com2TxQue, &charData); 
	charData.ucVal = 0x3a+0xff+0xff+0xff+cmd[0]+cmd[1]+cmd[2];
	charQueueIn_irq(&g_com2TxQue, &charData); 
}
#endif

#if	0
void CMD_SendCmd_3d0(unsigned char Code1,unsigned int Code2,unsigned int par1,unsigned int par2,unsigned int extend_len)
{
	unsigned char CMD[12];
	unsigned char i;
	unsigned char xor = 0;
	unsigned char TxSum = 0;
	struct charData_s charData;

	CMD[0] = 0x3A;
	CMD[1] = Code1;
	CMD[2] =(unsigned char)(Code2>>8);
	CMD[3] =(unsigned char)(Code2);
	CMD[4] =(unsigned char)(extend_len>>8);
	CMD[5] = (unsigned char)(extend_len);
	for(i=0;i<6;i++)
		xor ^= CMD[i];
	CMD[6] = xor;

	CMD[7] = (unsigned char)(par1>>8);
	CMD[8] = (unsigned char)(par1);
	CMD[9] = (unsigned char)(par2>>8);
	CMD[10] = (unsigned char)(par2);
	for(TxSum=0,i=0; i<(7+extend_len); i++)
	{
		TxSum += CMD[i];
	}
	TxSum  =~TxSum;
	CMD[7+extend_len] = TxSum;

	for(i=0;i<(8+extend_len);i++)
	{
		charData.ucVal = CMD[i];
		charQueueIn_irq(&g_com2TxQue, &charData); 
	}
}
#endif

/*******************************************************************************
 * This command have no responsed
 *******************************************************************************/
void FP_INIT(void)
{
	unsigned char CMD[12];
	unsigned char i;
	unsigned char xor = 0;
	unsigned char TxSum = 0;
	struct charData_s charData;

	CMD[0] = 0x3A;
	CMD[1] = FP_INTERRUPT;		//Code1 = 0;
	CMD[2] = 0; 				//(unsigned char)(Code2>>8);
	CMD[3] = 0; 				//(unsigned char)(Code2);
	CMD[4] = 0;					//(unsigned char)(extend_len>>8);
	CMD[5] = 0;					//(unsigned char)(extend_len);
	for(i = 0; i < 6; i++)
		xor ^= CMD[i];
	CMD[6] = xor;

	for(TxSum=0,i=0; i < 7; i++)
	{
		TxSum += CMD[i];
	}
	TxSum  =~TxSum;
	CMD[7] = TxSum;

	for(i = 0; i < 8; i++)
	{
		charData.ucVal = CMD[i];
		charQueueIn_irq(&g_com2TxQue, &charData); 
		charQueueIn_irq(&g_com1TxQue, &charData); 		//?????????????????????????
	}
}
/*******************************************************************************
 * function: FPRegister
 * Description:注册指纹
 * input: 	ID - 指定的ID号
 * 			stime - 超时时间
 * output:　no
 * return:  no
 *
 * other: 串口收到...表示注册成功
 *******************************************************************************/
unsigned short int g_enroll_cmd[3];
//void FP_Register(unsigned char ID, unsigned char stime)
void FP_Register(unsigned int ID1, unsigned int ID2, unsigned int stime)
{
	unsigned char CMD[12];
	unsigned char i;
	unsigned char xor = 0;
	unsigned char TxSum = 0;
	struct charData_s charData;
	
	g_enroll_cmd[0] = MATCH_FRIST;
	g_enroll_cmd[1] = MATCH_SECOND;
	g_enroll_cmd[2] = MATCH_THREE;
	
	#define CEXTEND_LEN (4)

	CMD[0] = 0x3A;
	CMD[1] = FP_ENROLL;									//Code1;
	CMD[2] = (unsigned char)(g_enroll_cmd[stime] >> 8);	//(unsigned char)(Code2>>8);
	CMD[3] = (unsigned char)(g_enroll_cmd[stime]);		//(unsigned char)(Code2);
	CMD[4] = (unsigned char)(CEXTEND_LEN >> 8);
	CMD[5] = (unsigned char)(CEXTEND_LEN);
	for(i=0;i<6;i++)
		xor ^= CMD[i];
	CMD[6] = xor;

	CMD[7] = (unsigned char)(ID1>>8);
	CMD[8] = (unsigned char)(ID1);
	CMD[9] = (unsigned char)(ID2>>8);
	CMD[10] = (unsigned char)(ID2);
	for(TxSum = 0,i = 0; i<(7 + CEXTEND_LEN); i++)
	{
		TxSum += CMD[i];
	}
	TxSum  =~TxSum;
	CMD[7 + CEXTEND_LEN] = TxSum;

	for(i=0;i<(8 + CEXTEND_LEN);i++)
	{
		charData.ucVal = CMD[i];
		charQueueIn_irq(&g_com2TxQue, &charData); 
		charQueueIn_irq(&g_com1TxQue, &charData); 		//?????????????????????????
	}
	
	#undef CEXTEND_LEN
}
/*******************************************************************************
 * function: FPDeRegister
 * Description:注销指定范围内的指纹模板
 * input: 	ID1 - 指定范围的起始ID号
 * 		 	ID2 - 指定范围的结束ID号
 * output:　no
 * return:  no
 *
 * other: 此操作之后一般会修改索引表
 *******************************************************************************/
void FP_DeRegister(unsigned int ID1,unsigned int ID2)
{
	unsigned char CMD[12];
	unsigned char i;
	unsigned char xor = 0;
	unsigned char TxSum = 0;
	struct charData_s charData;
	
	#define CEXTEND_LEN (4)

	CMD[0] = 0x3A;
	CMD[1] = FP_DELETE;									//Code1;
	CMD[2] = (unsigned char)(0xe000 >> 8);				//(unsigned char)(Code2>>8);
	CMD[3] = (unsigned char)(0xe000);					//(unsigned char)(Code2);
	CMD[4] =(unsigned char)(CEXTEND_LEN >> 8);
	CMD[5] = (unsigned char)(CEXTEND_LEN);
	for(i=0;i<6;i++)
		xor ^= CMD[i];
	CMD[6] = xor;

	CMD[7] = (unsigned char)(ID1 >> 8);
	CMD[8] = (unsigned char)(ID1);
	CMD[9] = (unsigned char)(ID2 >> 8);
	CMD[10] = (unsigned char)(ID2);
	for(TxSum = 0,i = 0; i<(7 + CEXTEND_LEN); i++)
	{
		TxSum += CMD[i];
	}
	TxSum  =~TxSum;
	CMD[7 + CEXTEND_LEN] = TxSum;

	for(i=0;i<(8 + CEXTEND_LEN);i++)
	{
		charData.ucVal = CMD[i];
		charQueueIn_irq(&g_com2TxQue, &charData); 
		charQueueIn_irq(&g_com1TxQue, &charData); 		//?????????????????????????
	}
	
	#undef CEXTEND_LEN
}
/*******************************************************************************
 * function: FPQuery
 * 查询全部指纹(0..57)
 * 
 * input: no
 * output:
 * return: no
 * other: 从串口收到的数据为0x3a+0x03+0xaa+0x01则表示此次查询成功
 *******************************************************************************/
//void FP_Query(unsigned char ID1,unsigned char ID2)								 
void FP_Query(unsigned int ID1,unsigned int ID2)
{
	unsigned char CMD[12];
	unsigned char i;
	unsigned char xor = 0;
	unsigned char TxSum = 0;
	struct charData_s charData;
	
	#define CEXTEND_LEN (4)

	CMD[0] = 0x3A;
	CMD[1] = FP_MATCH;									//Code1;
	CMD[2] = (unsigned char)(0xa6e0 >> 8);	//(unsigned char)(Code2>>8);
	CMD[3] = (unsigned char)(0xa6e0);		//(unsigned char)(Code2);
	CMD[4] =(unsigned char)(CEXTEND_LEN >> 8);
	CMD[5] = (unsigned char)(CEXTEND_LEN);
	for(i=0;i<6;i++)
		xor ^= CMD[i];
	CMD[6] = xor;

	CMD[7] = (unsigned char)(ID1 >> 8);
	CMD[8] = (unsigned char)(ID1);
	CMD[9] = (unsigned char)(ID2 >> 8);
	CMD[10] = (unsigned char)(ID2);
	for(TxSum = 0,i = 0; i<(7 + CEXTEND_LEN); i++)
	{
		TxSum += CMD[i];
	}
	TxSum  =~TxSum;
	CMD[7 + CEXTEND_LEN] = TxSum;

	for(i=0;i<(8 + CEXTEND_LEN);i++)
	{
		charData.ucVal = CMD[i];
		charQueueIn_irq(&g_com2TxQue, &charData); 
		charQueueIn_irq(&g_com1TxQue, &charData); 		//?????????????????????????
	}
	
	#undef CEXTEND_LEN
}
/*******************************************************************************
 * function: FP_AddByManager
 * Description: 管理员录入指纹(不包括指纹模板空的情景)
 * input: 管理员指纹ID
 * output: no
 * return: 	0 - 没有相应可用的空ID号
 *			
 *******************************************************************************/
int FP_AddByManager(unsigned char Manager_ID, unsigned char stime) 	//根据管理权限 1，2，3添加指纹
{
#if	0
	if(Manager_ID == 0)
	{
		FP_Register(1, C1STMANAGEE, stime);
	}
	else if(Manager_ID == 1)
	{
		if(fingerTab_entryIsEmpty(2))          //找空指纹模板序号(第二管理者，可以录入第三个管理者)
    	{
    		FP_Register(2, 2, stime);
    	}
    	else
    	{
			FP_Register(C2NDMANAGES, C2NDMANAGEE, stime);
		}
	}
	else
	{
		FP_Register(C3RDMANAGES, C3RDMANAGEE, stime);
	}
	
	return	1;
#else
	int i = 0;
	
	if(Manager_ID == 0)
	{
		//for (i = 1; i <= 57; i++)
		for (i = 1; i <= C1STMANAGEE; i++)      //找空指纹模板序号(第一管理者，可以无限录入，直到录满)
    	{
    		if(fingerTab_entryIsEmpty(i)) 		//查询首个空ID号码
    		{
    			break;
    		}
    	}
    	if(i > C1STMANAGEE) 					//如果查询到20号都没有空指纹退出
    	{
    		i = 0;
    	}
	}
	else if(Manager_ID == 1)
	{
		if(fingerTab_entryIsEmpty(2))          //找空指纹模板序号(第二管理者，可以录入第三个管理者)
    	{
    		i = 2;
    	}
    	else
    	{
			for (i = C2NDMANAGES; i <= C2NDMANAGEE; i++)      //查询空指纹
    		{
    			if(fingerTab_entryIsEmpty(i)) 	//查询首个空ID号码
    			{
    				break;
    			}
    		}
   		}
    	if (i > C2NDMANAGEE) 		//如果查询到38号都没有空指纹退出
      	{
    		i = 0;
      	} 
	}
	else if(Manager_ID == 2)
	{
		for (i = C3RDMANAGES; i <= C3RDMANAGEE; i++)                 //显示空指纹
    	{
    		if(fingerTab_entryIsEmpty(i)) 	//查询首个空ID号码
    		{
    			break;
    		}
    	}
    	if (i > C3RDMANAGEE)
      	{
    		i = 0;
      	}  		//如果查询到38号都没有空指纹退出
	}
	
	//if((i >= 3) && (i <= 57))
	if((i >= 1) && (i <= CTOTALFINGER - 1))
	{
		FP_Register(i, i, stime);
		return	1;
	}
	else
	{
		return	0;
	}
#endif
}
 
/*******************************************************************************
 * function: FP_DelByManager
 * Description: 管理员录入指纹(不包括指纹模板空的情景)
 * input: 管理员指纹ID
 * output: no
 * return: no
 * 		
 *******************************************************************************/
unsigned char FP_DelByManager(unsigned char Manager_ID) //根据管理权限 1，2，3删除指纹
{
	if(Manager_ID == 0)                 //第一管理者删除其它所有指纹
    {
    	FP_DeRegister(1, CTOTALFINGER - 1);
    }
    else if(Manager_ID == 1)      		//第二管理者删除其所录的指纹
    {
    	FP_DeRegister(C2NDMANAGES, C2NDMANAGEE);
    }
    else if(Manager_ID == 2)      		//第三管理者删除其所录的指纹
    {
    	FP_DeRegister(C3RDMANAGES, C3RDMANAGEE);
    }
    
    return	0;
}
/*******************************************************************************
 * function: FP_DelAll
 * Description: 删除全部指纹
 * input: no
 * output: no
 * return: no
 * 		
 *******************************************************************************/
void FP_DelAll(void) //根据管理权限 1，
{
	FP_DeRegister(0, CTOTALFINGER - 1);
}

#if	0
 unsigned char FPrespTab[][4] = {
 	{0, 0, 0, 0},
 	{0x3a, 0x00, 0xaa, 0x00},	/** 初始化指纹模板成功(返回5Bytes) **/
 	{0x3a, 0x02, 0xaa, 0x00},	/** 注册指纹模板成功(返回5Bytes) **/
 	{0x3a, 0x03, 0xaa, 0x01},	/** 查找指纹模板成功(返回6Bytes) **/
 	{0x3a, 0x04, 0xaa, 0x00},	/** 删除指纹模板成功(返回5Bytes, !!!注消指纹模板只需要改索引表即可) **/
 	{0, 0, 0, 0},
 };
 #endif
 
 /******************************************************************************
  * function: FP_GetRespData
  * Description: get received data from FIFO
  * input: src - pointer of FIFO
  * output: arr - the data get from FIFO
  * return: <= 12 the length of data get from FIFO
  * Get all of the data in the FIFO
  *******************************************************************************/
int FP_GetRespData(charBuf_queue_t *src, unsigned char arr[]) 
{
    int i = 255;
	charData_t charData;
    
    /** 找第一个数据，后续数据一定是本组数据 **/
	while(charQueueOut_irq(src, &charData) == TRUE)
    {
        if((charData.ucVal == 0x3a) && (i == 255))		/** 找到第一个0x3a **/
        {
    		i = 0;
    		arr[i] = charData.ucVal;
            break;
        }
    }
    /** 数据移到找到comTmpBuf中。结束符号，其之前数据一定是本组数据 **/
    if(i == 0)
    {
    	while(charQueueOut_irq(src, &charData) == TRUE)
    	{
    		i++;
    		if(i < 12)
    		{
    			arr[i] = charData.ucVal;
    		}
    	}
    	
    	return	i + 1;		/** 收到正常数据而结束 **/
	}
	
    return	0;		/** 没有收到正常数据 **/
}

/** 查返回值类型,  **/
int FP_RespGetType(unsigned char arr[]) 
{
	if(arr[0] == 0x3a)
	{
    	return	arr[1];
	}
	return	0xff;	/** 不可识别的数据类型 **/
}

/** 查返回值长度 **/
int FP_RespGetLen(unsigned char arr[])
{
	if(arr[0] == 0x3a)
	{
    	return	((((int)arr[4]) << 8) | arr[5]);
	}
	return	0xffff;	/** 不可识别的Id类型 **/
}

/** 查返回值类型,  **/
int FP_RespGetId(unsigned char arr[])
{
	if(arr[0] == 0x3a)
	{
    	return	((((int)arr[7]) << 8) | arr[8]);
	}
	return	CINVALIDID;	/** 不可识别的Id类型 **/
}

/** 查处理结果 **/
int FP_RespGetResult(unsigned char arr[])
{
	if(arr[0] == 0x3a)
	{
    	return	((((int)arr[2]) << 8) | arr[3]);
	}
	return	0xffff;	/** 不可识别的Id类型 **/
}

#if	0
/***********************************************************
 * function: FP_RespInit
 * Description: check responsed of initial command
 * input: arr - the responsed data
 *        ucLen - the data length
 * output: no
 * return: ResultCode, same as FP-ResultCode
 * other:
 * 		check responsed: initial
 * 		0x3a+0x00+0xab+0x00+0xe5
 * 		0x3a+0x00+0xaa+0x00+0xe4
 ***********************************************************/
int FP_RespInit(unsigned char arr[], unsigned char ucLen)
{	
	if((arr[0] == 0x3a)
		&& (arr[1] == 0x00)
		&& (arr[2] == 0x00)
		&& (arr[3] == 0x00)
		&& (arr[4] == 0x00)
		&& (arr[5] == 0x00)
		&& (arr[6] == 0x3a)
		&& (arr[7] == 0x8b))
	{
		return	CMD_ACK_OK;		/** success **/
	}

	return	CMD_ACK_SUM_ERR;	/** failed **/
}
#endif

/***********************************************************
 * function: FP_RespOk
 * check responsed: 
 ***********************************************************/
int FP_RespChk(unsigned char arr[])
{
	unsigned char xor_check = 0;
  	unsigned char rx_sum=0;
	int iRevDatLen = 0;
	int i;
	
	for(i = 0; i < 6; i++)
	{
		xor_check ^= arr[i];
	}
	if(xor_check != arr[i])
	{
		return CMD_ACK_XOR_ERR;			/** xor error **/
	}
	iRevDatLen = (((unsigned int)arr[4])<<8)|arr[5];
	
	for(i = 0; i < (7 + iRevDatLen); i++)
	{
		rx_sum += arr[i];
	}
	rx_sum = ~rx_sum;
	if(rx_sum!=arr[i])
	{
		return CMD_ACK_SUM_ERR;			/** sum error **/
	}
	return	CMD_ACK_OK;		/** success **/
}

#if	0
/***********************************************************
 * function: FP_RespRgst
 * Description: check responsed of register command
 * input: arr - the responsed data
 *        ucLen - the data length
 * output: no
 * return: ResultCode, same as FP-ResultCode
 *
 * check responsed: Register
 *		0x3a+0x02+0xab+0x00+0xe7
 *		0x3a+0x02+0xaa+0x01+0xID+CKSUM
 * 3种可以能性:	case 1: 0x3a+0x02+0xab+0x00+0xe7	(0xab)
 *				case 2: 0x3a+0x02+0xaa+0x01+0xID+CKSUM	(0xaa)
 *				case 3: 0x3a+0x02+0xab+0x00+0xe7+0x3a+0x02+0xaa+0x01+0xID+CKSUM	(0xab+0xaa)	- 一定要先处理这个
 ***********************************************************/
int FP_RespRgst(unsigned char arr[], unsigned char *ucpLen)
{
	int i;	  
//	charData_t charData;
	unsigned char ucTmp;


/** case 3 **/
	ucTmp = 0;
	for(i = 5; i < 10; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[5] == 0x3a) && (arr[6] == 0x02) && (arr[7] == 0xaa) && (arr[8] == 0x01) && (arr[10] == ucTmp))
	{
		*ucpLen = arr[9];
		return	arr[2];				/** 0xaa **/
	}

	/** case 2 **/
	ucTmp = 0;
	for(i = 0; i < 5; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[0] == 0x3a) && (arr[1] == 0x02) && (arr[2] == 0xaa) && (arr[3] == 0x01) && (arr[5] == ucTmp)) 
	//if((arr[0] == 0x3a) && (arr[1] == 0x02) && (arr[2] == 0xaa) && (arr[3] == 0x01))
	{
		*ucpLen = arr[4];
		return	arr[2];				/** 0xaa **/
	}

	/** case 1 **/
	ucTmp = 0;
	for(i = 0; i < 4; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[0] == 0x3a) && (arr[1] == 0x02) && (arr[2] == 0xab) && (arr[3] == 0x00) && (arr[4] == ucTmp))
	//if((arr[0] == 0x3a) && (arr[1] == 0x02) && (arr[2] == 0xab) && (arr[3] == 0x00))
	{
		return	arr[2];				/** 0xaa or oxab **/
	}
		
	return	0;	/** 无关数据, 返回0 **/
}
#endif
 
 #if	0
/***********************************************************
 * FP register done
 * 3种可以能性:	0x3a+0x02+0xab+0x00+0xe7	(0xab)
 *				0x3a+0x02+0xaa+0x01+0xID+CKSUM	(0xaa)
 *				0x3a+0x02+0xab+0x00+0xe7+0x3a+0x02+0xaa+0x01+0xID+CKSUM	(0xab+0xaa)
 *				0	 1	  2	   3	4	 5	  6	   7	8	 9	  10
 ***********************************************************/


 /***********************************************************
 * FP Deregister doing/done
 * 3种可以能性:	0x3a+0x04+0xab+0x00+sum	(0xab)
 *				0x3a+0x04+0xaa+0x00+sum	(0xaa)
 *				0x3a+0x04+0xab+0x00+0xe7+0x3a+0x02+0xaa+0x00+CKSUM	(0xab+0xaa)
 *				0	 1	  2	   3	4	 5	  6	   7	8	 9	  
 ***********************************************************/

int FP_RespDeRgst(unsigned char arr[], unsigned char *pManagerId)
{	
	int i;
	unsigned char ucTmp;

	/** case 3 **/
	ucTmp = 0;
	for(i = 5; i < 8; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[5] == 0x3a) && (arr[6] == 0x04) && (arr[7] == 0xaa) && (arr[8] == 0x00) && (arr[9] == ucTmp))
	{
		return	arr[7];				/** 0xaa **/
	}
	
	/** case 2 **/
	ucTmp = 0;
	for(i = 0; i < 4; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[0] == 0x3a) && (arr[1] == 0x04) && (arr[2] == 0xaa) && (arr[3] == 0x00) && (arr[4] == ucTmp)) 
	//if((arr[0] == 0x3a) && (arr[1] == 0x02) && (arr[2] == 0xaa) && (arr[3] == 0x01))
	{
		return	arr[2];				/** 0xaa **/
	}
	
	/** case 1 **/
	ucTmp = 0;
	for(i = 0; i < 4; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[0] == 0x3a) && (arr[1] == 0x04) && (arr[2] == 0xab) && (arr[3] == 0x00) && (arr[4] == ucTmp))
	//if((arr[0] == 0x3a) && (arr[1] == 0x02) && (arr[2] == 0xab) && (arr[3] == 0x00))
	{
		return	arr[2];				/** 0xaa or oxab **/
	}
	
	return	0;	/** 无关数据, 返回0 **/
}
#endif

#if	0
 /***********************************************************
 * check responsed: search
 *
 * check responsed: Register
 *		0x3a+0x03+0xab+0x00+0xe7
 *		0x3a+0x03+0xaa+0x01+0xID+CKSUM
 * 3种可以能性:	case 1: 0x3a+0x03+0xab+0x00+CKSM	(0xab)
 *				case 2: 0x3a+0x03+0xaa+0x01+0xID+CKSM	(0xaa)
 *				case 3: 0x3a+0x03+0xab+0x00+0xe7+0x3a+0x02+0xaa+0x01+0xID+CKSM	(0xab+0xaa)
 ***********************************************************/
int FP_RespSearch(unsigned char arr[], unsigned char *pManagerId)
{
	int i;	  
//	charData_t charData;
	unsigned char ucTmp;

	/** case 1 **/
	ucTmp = 0;
	for(i = 0; i < 4; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[0] == 0x3a) && (arr[1] == 0x03) && (arr[2] == 0xab) && (arr[3] == 0x00) && (arr[4] == ucTmp))
	//if((arr[0] == 0x3a) && (arr[1] == 0x03) && (arr[2] == 0xab) && (arr[3] == 0x00))
	{
		return	arr[2];				/** 0xaa or oxab **/
	}
	
	/** case 2 **/
	ucTmp = 0;
	for(i = 0; i < 5; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[0] == 0x3a) && (arr[1] == 0x03) && (arr[2] == 0xaa) && (arr[3] == 0x01) && (arr[5] == ucTmp)) 
	//if((arr[0] == 0x3a) && (arr[1] == 0x03) && (arr[2] == 0xaa) && (arr[3] == 0x01))
	{
		*pManagerId = arr[4];
		return	arr[2];				/** 0xaa **/
	}

	/** case 3 **/
	ucTmp = 0;
	for(i = 5; i < 10; i++)
	{
		ucTmp += arr[i];
	}
	if((arr[5] == 0x3a) && (arr[6] == 0x03) && (arr[7] == 0xaa) && (arr[8] == 0x01) && (arr[10] == ucTmp))
	{
		*pManagerId = arr[9];
		return	arr[7];				/** 0xaa **/
	}
	
	return	0;	/** 无关数据, 返回0 **/
}
#endif

/////////////////////////////////////////////////////
/**********************************************************************
 * name: ADC_getValue 
 * description: get ADCref value(default value 0)
 **********************************************************************/
int ADC_getValue(void)
{
	return	(g_flash.arrChar[CFLASH_ADC_VALUE] | (g_flash.arrChar[CFLASH_ADC_VALUE + 1] << 8));
}

/**********************************************************************
 * name: ADC_getValue 
 * description: get ADCref value(default value 0)
 **********************************************************************/
void ADC_setValue(int value)
{
	g_flash.arrChar[CFLASH_ADC_VALUE] = (value & 0xff);
	g_flash.arrChar[CFLASH_ADC_VALUE + 1] = ((value >> 8) & 0xff);
	//return	(g_flash.arrChar[CFLASH_ADC_VALUE] | (g_flash.arrChar[CFLASH_ADC_VALUE + 1] << 8));
}

/**********************************************************************
 * name: resetOP_setValue/resetOP_getValue
 * description: get reset operation control value(default value 0xff)
 **********************************************************************/
int resetOP_getValue(void)
{
	return	g_flash.arrChar[CFLASH_RESET_VALUE];
}

void resetOP_setValue(int value)
{
	g_flash.arrChar[CFLASH_RESET_VALUE] = (value & 0xff);
}

/*********************************************************************
 *
 *
 *********************************************************************/
#define CPSWORDLEN	(8 + 2)
#define	CPSWORDCNT	(8)
//#define	CENTRYFLAG_BUSY	0xa5

/**********************************************************************
 * name: psword_isFull 
 * description: check weather the password is full
 **********************************************************************/
int psword_isFull(void)
{
	int	i;
	for(i = 0; i < CPSWORDCNT; i++)
	{
		if(g_flash.arrChar[i * 10 + CFLASH_PASSWD_START] != CENTRYFLAG_BUSY)
		{
			return	0;		/** find a idle entry **/
		}
	}
	
	return	1;
}

/**********************************************************************
 * name: psword_isEmpty 
 * description: check weather the password is  empty
 **********************************************************************/
int psword_isEmpty(void)
{
	int	i;
	for(i = 0; i < CPSWORDCNT; i++)
	{
		if(g_flash.arrChar[i * 10 + CFLASH_PASSWD_START] == CENTRYFLAG_BUSY)
		{
			return	0;
		}
	}
	
	return	1;
}

/**********************************************************************
 * name: psword_inputConfirm 
 * description: check weather the password is same as before
 **********************************************************************/
int psword_inputConfirm(unsigned char _password[])
{
	int	i;
	for(i = 0; i < 8; i++)
	{
		if(_password[i] != _password[i + 8])
		{
			return	0;
		}
	}
	
	return	1;
}

/**********************************************************************
 * name: password_Query 
 * description: Query weather the password is registered
 **********************************************************************/
int password_Query(unsigned char _password[])
{
	int i, j;

	for(i = 0; i < CPSWORDCNT; i++)
	{
		if(g_flash.arrChar[i * 10 + CFLASH_PASSWD_START] == CENTRYFLAG_BUSY)	/** 此组有密码, 可以进行比较 **/
		{
			for(j = 0; j < CPSWORDLEN - 2; j++)
			{
				if(g_flash.arrChar[i * 10 + CFLASH_PASSWD_START + j + 1] != _password[j])
				{
					break;			/** 不相等，当前一组密码比对失败，退出并比较下一组 **/
				}
			}
			if(j == CPSWORDLEN - 2)	/** 没有中途退出，也就是比较通过 **/
			{
				return	1;			/** 查找成功 **/
			}
		}
	}
	return	0;	/** 查找失败 **/
}

 /******************************************************************************
  *	function: 
  * Description: 添加一组密码 
  * input: 	_password
  * output: 
  * return:  成功返回1, 失败返回0
  *
  * other: 注意密码的存贮顺序
  ******************************************************************************/
int password_add(unsigned char _password[])
{
	int i, j;

	for(i = 0; i < CPSWORDCNT; i++)
	{
		if(g_flash.arrChar[i * 10 + CFLASH_PASSWD_START] != CENTRYFLAG_BUSY)	/** 此组没有密码, 可以存入 **/
		{
			g_flash.arrChar[i * 10 + CFLASH_PASSWD_START] = CENTRYFLAG_BUSY;
			
			for(j = 0; j < CPSWORDLEN - 2; j++)
			{
				g_flash.arrChar[i * 10 + CFLASH_PASSWD_START + j + 1] =  _password[j];
			}
			
			return	1;		/** 添加成功 **/
		}
	}
	return	0;	/** 添加失败 **/
}

/*******************************************************************************
 * function:
 * Description: 删除全部密码(对应索引的位置清零即可)
 * input: no
 * output: no
 * return: no
 *
 *******************************************************************************/
int password_del(void)
{
	int i;
	
	for(i = 0; i < CPSWORDCNT; i++)
	{
		g_flash.arrChar[i * 10 + CFLASH_PASSWD_START] = CENTRYFLAG_IDLE;		/** 此组没有密码 **/
	}
	
	return	1;
}

/*******************************************************************************
 * get/set password_errno
 *******************************************************************************/
unsigned char getpassword_errno(void)
{
	return	g_flash.arrChar[CFLASH_PASSWD_ERR];
}

void setpassword_errno( unsigned char value)
{
	g_flash.arrChar[CFLASH_PASSWD_ERR] = value;
}
