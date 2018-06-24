#ifndef __L_FINGPRINT_H__

void flashCache_init(void);

//int fingerTab_empty(void);
int fingerTab_isEmpty(void);
void fingprintCmd(unsigned char *cmd);
void fingerTab_fill(unsigned char Manager_ID, unsigned char value);

void FP_INIT(void);
//void FP_Register(unsigned char ID,unsigned char stime);
//void FP_Register(unsigned int ID,unsigned int stime);
void FP_Register(unsigned int ID1, unsigned int ID2, unsigned int stime);
//void FP_DeRegister(unsigned char ID1,unsigned char ID2);  
void FP_DeRegister(unsigned int ID1,unsigned int ID2);
//void FP_Query(unsigned char ID1,unsigned char ID2);	 
void FP_Query(unsigned int ID1,unsigned int ID2);
void FP_DelAll(void);
//int FP_chkResp(charBuf_queue_t *src);
int FP_GetRespData(charBuf_queue_t *src, unsigned char arr[]);
//int FP_GetRespType(unsigned char arr[]);
int FP_RespChk(unsigned char arr[]);
int FP_RespGetResult(unsigned char arr[]);
int FP_RespGetType(unsigned char arr[]);
int FP_RespGetLen(unsigned char arr[]);
int FP_RespGetId(unsigned char arr[]);
#if	0
int FP_RespInit(unsigned char arr[], unsigned char ucLen);
int FP_RespRgst(unsigned char arr[], unsigned char *ucpLen);
int FP_RespDeRgst(unsigned char arr[], unsigned char *pManagerId);
int FP_RespSearch(unsigned char arr[], unsigned char *pManagerId);
#endif
//void fingerTab_init(void);

unsigned char fingerTab_getEntry(int idx);
void fingerTab_setEntry(int idx, unsigned char value);
//void fingerTab_fill(int begin, int end, unsigned char value);
//unsigned char FP_AddByManager(unsigned char Manager_ID);
int FP_AddByManager(unsigned char Manager_ID, unsigned char stime);
unsigned char FP_DelByManager(unsigned char Manager_ID);

int ADC_getValue(void);
void ADC_setValue(int value);
int resetOP_getValue(void);
void resetOP_setValue(int value);

int psword_isFull(void);
int psword_inputConfirm(unsigned char _password[]);
int password_Query(unsigned char _password[]);
int password_add(unsigned char _password[]);
int password_del(void);
unsigned char getpassword_errno(void);
void setpassword_errno( unsigned char value);

#endif
/////////////////////////////////////////////////////

