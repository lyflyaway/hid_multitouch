#ifndef __CAN_H
#define __CAN_H

typedef enum {FAILED = 0, PASSED = !FAILED} CANTestResult;

void CAN_Config(void);
CANTestResult CAN_Polling(void);
CANTestResult CAN_Interrupt(void);

#endif


