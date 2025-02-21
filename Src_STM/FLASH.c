#include <FLASH.h>
#include <stm32f1xx.h>


void FLASH_Erasen(uint32_t address)
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.Banks=1;
	EraseInit.NbPages=1;
	EraseInit.PageAddress = address;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	uint32_t pagee;
	HAL_FLASHEx_Erase(&EraseInit,&pagee);
	HAL_FLASH_Lock();
}



void FLASH_Write_Array(uint32_t address,uint8_t *arr, uint16_t len)
{
	HAL_FLASH_Unlock();
	uint16_t *pt= (uint16_t*)arr;
	for(uint16_t i=0;i<(len+1)/2;i++)
	{
	HAL_FLASH_Program( FLASH_TYPEPROGRAM_HALFWORD,address +2*i ,*pt);
	pt++;
	}
	HAL_FLASH_Lock();
}

void FLASH_Read_Array(uint32_t address,uint8_t *arr, uint16_t len)
{
	uint16_t *pt= (uint16_t*)arr;
	for(uint16_t i=0;i<(len+1)/2;i++)
	{
	*pt= *(__IO uint16_t *)(address +2*i);
	pt++;
	}
}
void FLASH_Write_Int(uint32_t address,int value)
{
		HAL_FLASH_Unlock();
	HAL_FLASH_Program( FLASH_TYPEPROGRAM_HALFWORD,address,value);
	
	HAL_FLASH_Lock();
}
	
int FLASH_Read_Int(uint32_t address)
	{
		return *(__IO uint16_t *)(address);
			
		
}