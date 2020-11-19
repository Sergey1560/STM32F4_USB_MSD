#include "main.h"

uint8_t data[DATA_SIZE];

int main(void){
	//Включение тактирования на 168Mhz
	RCC_init();

	//Инициализация HAL, в основном Systick для работы HALDelay
	HAL_Init();

	//Включение порта
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	//Инициализация USB
	MX_USB_HOST_Init();

	while (1)	{
		MX_USB_HOST_Process();
		if(Appli_state==APPLICATION_READY){
			test_usb();
			Appli_state = APPLICATION_IDLE;
		};
	}
}


void test_usb(void){
  FRESULT result;
  FATFS FATFS_Obj;
  FIL file;
  uint32_t nBytes;
  volatile uint32_t data_size=0;

  result = f_mount((FATFS *)&FATFS_Obj, "0", 1);
  if(result != FR_OK){
    while(1);
  };

  result = f_open((FIL*)&file, "test_file.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if(result != FR_OK) {
    while(1);
  }
	
	data_size=0;
	/* WRITE_COUNT раз пишет в файл кусками по DATA_SIZE Кб */
	for(uint32_t i=0; i<WRITE_COUNT; i++){
		memset(data,i,DATA_SIZE);
		result=f_write((FIL*)&file, (uint8_t *)data, DATA_SIZE, (UINT *)&nBytes);	
		if(result != FR_OK) {
			while(1);
		}
		if(nBytes != DATA_SIZE){
			while(1);
		}
		data_size+=nBytes;
		result=f_sync((FIL*)&file);
		if(result != FR_OK) {
			while(1);
		}
	};

	result=f_close((FIL*)&file);
	if(result != FR_OK) {
		while(1);
	}
	 
	/* Чтение обратно, проверка записанного */
	result = f_open((FIL*)&file, "test_file.txt", FA_READ);
	if(result != FR_OK) {
		while(1);
	}

	data_size=0;
	
	for(uint32_t i=0; i<WRITE_COUNT; i++){
		memset(data,0,DATA_SIZE);
		result=f_read((FIL*)&file, (uint8_t *)data, DATA_SIZE, (UINT *)&nBytes);	
		if(result != FR_OK) {
			while(1);
		}
		if(nBytes != DATA_SIZE){
			while(1);
		}
		data_size+=nBytes;
		for(uint32_t k=0; k<DATA_SIZE; k++){
			if(data[k] != i){
			}
		}
	};
	
  result=f_close((FIL*)&file);
	if(result != FR_OK) {
		while(1);
	}

}


void SysTick_Handler(void){
    HAL_IncTick();
}

void OTG_FS_IRQHandler(void){
  HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
}
