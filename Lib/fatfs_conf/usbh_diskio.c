#include "usbh_diskio.h"

#define USB_DEFAULT_BLOCK_SIZE 512

extern USBH_HandleTypeDef  hUSB_Host;

DSTATUS USBH_status(BYTE lun){
  DRESULT res = RES_ERROR;

  if(USBH_MSC_UnitIsReady(&hUSB_Host, lun))
  {
    res = RES_OK;
  }else{
    res = RES_ERROR;
  }
  return res;
}

DRESULT USBH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;

  if(USBH_MSC_Read(&hUSB_Host, lun, sector, buff, count) == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUSB_Host, lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog ("USB Disk is not ready!");
      res = RES_NOTRDY;
      break; 

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}


DRESULT USBH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;

  if(USBH_MSC_Write(&hUSB_Host, lun, sector, (BYTE *)buff, count) == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUSB_Host, lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_WRITE_PROTECTED:
      USBH_ErrLog("USB Disk is Write protected!");
      res = RES_WRPRT;
      break;

    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}



DRESULT USBH_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC:
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    if(USBH_MSC_GetLUNInfo(&hUSB_Host, lun, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_nbr;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    if(USBH_MSC_GetLUNInfo(&hUSB_Host, lun, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_size;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

    /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :

    if(USBH_MSC_GetLUNInfo(&hUSB_Host, lun, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_size / USB_DEFAULT_BLOCK_SIZE;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
