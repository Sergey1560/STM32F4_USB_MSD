#include "usb_host.h"

USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);


void MX_USB_HOST_Init(void)
{
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
  {
    //Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS) != USBH_OK)
  {
    //Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    //Error_Handler();
  }
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  USBH_Process(&hUsbHostFS);
}

static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_DISCONNECT;
    f_mount(NULL, (TCHAR const*)"", 0); 
  break;

  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_READY;
  break;

  case HOST_USER_CONNECTION:
    Appli_state = APPLICATION_START;
  break;

  default:
  break;
  }

}

