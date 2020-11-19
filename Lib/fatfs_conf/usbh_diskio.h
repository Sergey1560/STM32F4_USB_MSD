#ifndef __USBH_DISKIO_H
#define __USBH_DISKIO_H

#include "diskio.h"
#include "ffconf.h"
#include "ff.h"
#include "stm32f4xx_hal.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "usbh_def.h"
#include "stdint.h"

/* Handle for USB Host */
#define hUSB_Host hUsbHostFS

DSTATUS USBH_status (BYTE);
DRESULT USBH_read (BYTE, BYTE*, DWORD, UINT);
DRESULT USBH_write (BYTE, const BYTE*, DWORD, UINT);
DRESULT USBH_ioctl (BYTE, BYTE, void*);

#endif /* __USBH_DISKIO_H */

