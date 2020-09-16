#ifndef __mymodbus_h
#define __mymodbus_h
#include <stdint.h>
typedef enum FLAGS
{
    TP_FLAG,
    PM_FLAG,
    CO_FLAG,
    NO2_FLAG,
    O3_FLAG,
    SO2_FLAG
} q_flags;
void GetData(void);
#endif
