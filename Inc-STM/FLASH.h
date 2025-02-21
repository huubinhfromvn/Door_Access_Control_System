#ifndef  __FLASH_H
#define  __FLASH_H
#include <stdint.h>
#include <string.h>


void FLASH_Erasen(uint32_t address);

void FLASH_Write_Array(uint32_t address,uint8_t *arr, uint16_t len);
void FLASH_Write_Int(uint32_t address,int value);
int FLASH_Read_Int(uint32_t address);
void FLASH_Read_Array(uint32_t address,uint8_t *arr, uint16_t len);


#endif