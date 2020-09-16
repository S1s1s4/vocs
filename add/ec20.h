#ifndef __ec20_h
#define __ec20_h


void AT_START(void);
unsigned char AT_CGSN(char* sn);
int AT_CSQ(void);
void  AT_CIMI(void);
void  AT_QICSGP(void);
void  AT_QIACT(void);
void  AT_QIOPEN(int port,char* ip);
void  AT_CLOSE(void);
void connect(char* m_sn,char* ip,int port,char *rssi);
void AT_QICLOSE(void);
int FindString(char * pSrc, int srcSize, char * pDest, int dstSize);
unsigned char FtpDownload( char* cDirectory, char* cFilename);
unsigned char  FtpLogin(char* cIp, unsigned short int usPort, char* cUser, char* cPass);
void FtpLogout(void);
unsigned char model_gettime(unsigned char isGeted);
#endif
