#ifndef __MAIN_H
#define __MAIN_H


#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "usb_host.h"

#include "ff.h"
#include "rcc.h"

#define DATA_SIZE 70*1024
#define WRITE_COUNT 255

extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
extern ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUsbHostFS;

void test_usb(void);

#endif /* __MAIN_H */
