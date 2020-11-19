#include "diskio.h"
#include "usbh_diskio.h"

/* Definitions of physical drive number for each drive */
#define DEV_USB		0	


DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
  DSTATUS stat;

	if(pdrv == DEV_USB){
    stat = USBH_status(0);
    return stat;
	};

	return STA_NODISK;
}

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	if(pdrv == DEV_USB){
		return(RES_OK);
	};

	return STA_NODISK;
}

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
  DRESULT res=RES_PARERR;

	if(pdrv == DEV_USB){
		res=USBH_read(0, buff, sector, count);
  };

	return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_write (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count        	/* Number of sectors to write */
)
{
  DRESULT res=RES_PARERR;

	if(pdrv == DEV_USB){
		res = USBH_write(0, buff, sector, count);
	};

  return res;
}

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DRESULT res=RES_PARERR;


  if(pdrv == DEV_USB){
    res = USBH_ioctl(0, cmd, buff);
  }

  return res;
}


/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
/*
DWORD get_fattime (void) {
	TDateTime DateTime;
	uint32_t  d,ret;
	
	rtc_Get(&DateTime);
	
	d=((((uint32_t)DateTime.Year)+2000) > 2017) ? DateTime.Year: 2017;
	d=(d+2000)-1980;
	ret=(d << 25);
	
	d=(uint32_t)DateTime.Month;
	d=((d > 0) && (d < 13)) ? d : 1;
	ret |= (d << 21);
	
	d=(uint32_t)DateTime.Day;
	d=((d > 0) && (d < 32)) ? d : 1;
	ret |= (d << 16);
	
	d=(uint32_t)DateTime.Hours;
	d=((d > 0) && (d < 25)) ? d : 1;
	ret |= (d << 11);
	
	d=(uint32_t)DateTime.Minutes;
	d=((d > 0) && (d < 60)) ? d : 1;
	ret |= (d << 5);
	
	d=(uint32_t)DateTime.Seconds / 2;
	d=((d > 0) && (d < 30)) ? d : 1;
	ret |= d ;
	
	return  ret;
};

*/
