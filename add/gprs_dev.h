#ifndef _GPRS_DEV_H_
#define _GPRS_DEV_H_



extern void gprs_dev_init(void);
extern void gprs_dev_pwrup(void);
extern void gprs_dev_restart(void);
extern void gprs_dev_pwrdown(void);

extern void gprs_dev_gotosleep(void);
extern void gprs_dev_wakeup(void);


#endif



