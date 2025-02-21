/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "KEYPAD.h"
#include "rc522.h"
#include "string.h"
#include "CLCD_I2C.h"
#include "stdio.h"
#include "FLASH.h"

#define tagsnum_address_data 0x800F800
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
CLCD_I2C_Name LCD1;
KEYPAD_Name KeyPad;
char KEYMAP[NUMROWS][NUMCOLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
char pas[]="123456"; // M?t kh?u m?u
char inputpas[7];
uint8_t pasindex = 0;
uint8_t status;
uint8_t str[MAX_LEN]; // Max_LEN = 16
uint8_t sNum[5];
char key;
uint8_t adsNum[5] = {19,95,84,45,53};
uint8_t tagsNum[100] ;
uint8_t tagindex;
uint8_t tagid ;
char idstr[3] ;
int wrongpascount=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	 FLASH_Read_Array(tagsnum_address_data,tagsNum,100);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	KEYPAD3X4_Init(&KeyPad, KEYMAP, GPIOB, GPIO_PIN_5, GPIOB, GPIO_PIN_4, GPIOB, GPIO_PIN_3, 
																	GPIOB, GPIO_PIN_9, GPIOB, GPIO_PIN_8,
																	GPIOB, GPIO_PIN_7, GPIOB, GPIO_PIN_6);
	MFRC522_Init();
	CLCD_I2C_Init(&LCD1,&hi2c2,0x4e,16,2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	CLCD_I2C_Clear(&LCD1);
	while(1)
	{
		bandau:
		CLCD_I2C_SetCursor(&LCD1,0,0);
		CLCD_I2C_WriteString(&LCD1, "Check pass");
		key = KEYPAD3X4_Readkey(&KeyPad); // L?y ký t? t? keypad
    if (key) // N?u có ký t? du?c nh?p
		{
			switch (key)
			{
				case '*': // Phím xóa (backspace)
					if (pasindex > 0)
					{
					pasindex--;
					inputpas[pasindex] = '\0';
					CLCD_I2C_SetCursor(&LCD1, pasindex, 1); // Ðua con tr? d?n v? trí ký t? cu?i
					CLCD_I2C_WriteChar(&LCD1, ' '); // Xóa ký t? trên LCD
					CLCD_I2C_SetCursor(&LCD1, pasindex, 1); // Ðua con tr? v? v? trí dúng
					}
					break;
				case '#':
					CLCD_I2C_Clear(&LCD1);
					while (1)
					{
						 CLCD_I2C_SetCursor(&LCD1,0,0);
						 CLCD_I2C_WriteString(&LCD1,"Check tag");
						 key = KEYPAD3X4_Readkey(&KeyPad);
						 status = MFRC522_Request(PICC_REQIDL, str);
						 status = MFRC522_Anticoll(str);
						 memcpy(sNum, str, 5);
						 HAL_Delay(100);
						 if((sNum[0]!=147) && (sNum[1]!=32) && (sNum[2]!=0) )
						 {	
							 if((sNum[0]==adsNum[0]) && (sNum[1]==adsNum[1]) && (sNum[2]==adsNum[2]) && (sNum[3]==adsNum[3]) && (sNum[4]==adsNum[4]) )
							 {
									for(int i=0;i<5;i++)
									{
										str[i]=0;
									}
									CLCD_I2C_Clear(&LCD1);
									while(1)
									{
										nhanlai:
										CLCD_I2C_SetCursor(&LCD1,1,0);
										CLCD_I2C_WriteString(&LCD1,"Welcome admin!");
										CLCD_I2C_SetCursor(&LCD1,2,1);
										CLCD_I2C_WriteString(&LCD1,"Choose Mode:");
										key = KEYPAD3X4_Readkey(&KeyPad);
										if(key)
										{
											switch(key)
											{
												case '1':
													CLCD_I2C_Clear(&LCD1);
													CLCD_I2C_WriteString(&LCD1,"Change pass");
													while(1)
													{
														key = KEYPAD3X4_Readkey(&KeyPad);	
														if (key) // N?u có ký t? du?c nh?p
														{
															if (key == '*') // Phím xóa (backspace)
															{
																if (pasindex > 0)
																{
																	pasindex--;
																	inputpas[pasindex] = '\0';
																	CLCD_I2C_SetCursor(&LCD1,pasindex, 1); // Ðua con tr? d?n v? trí ký t? cu?i
																	CLCD_I2C_WriteChar(&LCD1,' '); // Xóa ký t? trên LCD
																	CLCD_I2C_SetCursor(&LCD1,pasindex, 1); // Ðua con tr? v? v? trí dúng
																}
															}
															if (key >= '0' && key <= '9' && pasindex < 6) // Ch? ch?p nh?n s? và t?i da 6 ký t?
															{
																pas[pasindex++] = key;
																pas[pasindex] = '\0'; // K?t thúc chu?i
																CLCD_I2C_SetCursor(&LCD1,pasindex-1, 1);
																CLCD_I2C_WriteChar(&LCD1,key); // Hi?n th? d?u * thay cho s?
															}
															if ((key == '#')&& (pasindex==6) ) // Ðã nh?p d? 6 ký t?
															{
																CLCD_I2C_Clear(&LCD1);
																CLCD_I2C_WriteString(&LCD1,"SUCCESSFUL");
																HAL_Delay(2000);
																CLCD_I2C_Clear(&LCD1);
																pasindex = 0;
																sNum[0]= '\0';
																goto bandau;
															}							
														}
													}
													break;
												case '2':	
													for (int i=1;i<=20;i++)
													{
														tagindex = i*5;
														if ((tagsNum[tagindex-5]==255) && (tagsNum[tagindex-4]==255) && (tagsNum[tagindex-3]==255) && (tagsNum[tagindex-2]==255) && (tagsNum[tagindex-1]==255))
														{
															tagid=i;
														}
														else
														{
															tagid = 0;
															tagindex =0;
															break;
														}
														if (tagid==20)
														{
															CLCD_I2C_Clear(&LCD1);
															CLCD_I2C_SetCursor(&LCD1,0,0);
															CLCD_I2C_WriteString(&LCD1,"There is no card");
															CLCD_I2C_SetCursor(&LCD1,1,1);
															CLCD_I2C_WriteString(&LCD1,"Please add more");
															HAL_Delay(2000);
															tagid = 0;
															tagindex =0;
															CLCD_I2C_Clear(&LCD1);
															goto nhanlai;
														}
													}
													CLCD_I2C_Clear(&LCD1);
													while(1)
													{
														CLCD_I2C_SetCursor(&LCD1,0,0);
														CLCD_I2C_WriteString(&LCD1,"Delete tag");
														status = MFRC522_Request(PICC_REQIDL, str);
														status = MFRC522_Anticoll(str);
														memcpy(sNum, str, 5);
														HAL_Delay(100);
														if((sNum[0]!=147) && (sNum[1]!=32) && (sNum[2]!=0) )
														{
															for (int i=1;i<=20;i++)
															{
																tagindex = i*5;
																tagid = i;
																if((sNum[0]==tagsNum[tagindex-5]) && (sNum[1]==tagsNum[tagindex-4]) && (sNum[2]==tagsNum[tagindex-3]) && (sNum[3]==tagsNum[tagindex-2]) && (sNum[4]==tagsNum[tagindex-1]))
																{
																	for(int p=1;p<=5;p++)
																	{
																		tagsNum[tagindex-p]=255;
																	}
																	sprintf(idstr, "%d", tagid);
																	CLCD_I2C_Clear(&LCD1);
																	CLCD_I2C_SetCursor(&LCD1,1,0);
																	CLCD_I2C_WriteString(&LCD1,"Deletion complete");
																	/////////////////////
																  FLASH_Erasen(tagsnum_address_data);
	                                FLASH_Write_Array(tagsnum_address_data,tagsNum,100);
																	///////////////////////
																	CLCD_I2C_SetCursor(&LCD1,2,1);
																	CLCD_I2C_WriteString(&LCD1,"ID deleted:");
																	CLCD_I2C_WriteString(&LCD1,idstr);
																	tagid = 0;
																	tagindex =0;
																	HAL_Delay(2000);
																	CLCD_I2C_Clear(&LCD1);
																	CLCD_I2C_WriteString(&LCD1,"Delete tag");
																	for(int p=0;p<5;p++)
																	{
																		str[p]=0;
																	}	
																	goto bandau;
																}
																if(tagid==20)
																{
																	CLCD_I2C_Clear(&LCD1);
																	CLCD_I2C_SetCursor(&LCD1,3,0);
																	CLCD_I2C_WriteString(&LCD1,"Wrong card");
																	CLCD_I2C_SetCursor(&LCD1,2,1);
																	CLCD_I2C_WriteString(&LCD1,"Please Retry");
																	tagid = 0;
																	tagindex =0;
																	sNum[0] = '\0';
																	HAL_Delay(2000);
																	CLCD_I2C_Clear(&LCD1);
																	for(int p=0;p<5;p++)
																	{
																		str[p]=0;
																	}	
																}
															}					
														}
													}
													break;
												case '3':	
													CLCD_I2C_Clear(&LCD1);
													while(1)
													{
														bandau1:
														CLCD_I2C_SetCursor(&LCD1,0,0);
														CLCD_I2C_WriteString(&LCD1,"Add tag");	
														status = MFRC522_Request(PICC_REQIDL, str);
														status = MFRC522_Anticoll(str);
														memcpy(sNum, str, 5);
														if((sNum[0]!=147) && (sNum[1]!=32) && (sNum[2]!=0) )
														{
															for (int i=1;i<=20;i++)
															{
																tagindex = i*5;
																if((sNum[0]==tagsNum[tagindex-5]) && (sNum[1]==tagsNum[tagindex-4]) && (sNum[2]==tagsNum[tagindex-3]) && (sNum[3]==tagsNum[tagindex-2]) && (sNum[4]==tagsNum[tagindex-1]))
																{
																	tagid = i;
																	sprintf(idstr, "%d", tagid);
																	CLCD_I2C_Clear(&LCD1);
																	CLCD_I2C_SetCursor(&LCD1,0,0);
																	CLCD_I2C_WriteString(&LCD1,"Duplicated with");
																	CLCD_I2C_SetCursor(&LCD1,6,1);
																	CLCD_I2C_WriteString(&LCD1,"ID:");
																	CLCD_I2C_WriteString(&LCD1,idstr);
																	tagid = 0;
																	tagindex =0;
																	for(int p=0;p<5;p++)
																	{
																	str[p]=0;
																	}
																	HAL_Delay(2000);
																	CLCD_I2C_Clear(&LCD1);
																	goto bandau1;
																}
																if(i==20)
																{
																	tagid = 0;
																	tagindex =0;
																	for(int p=0;p<5;p++)
																	{
																	str[p]=0;
																	}
																	break;
																}
															}
															for (int i=1;i<21;i++)
															{
																tagindex = i*5;
																tagid = i;
																if ((tagsNum[tagindex-5]==255) && (tagsNum[tagindex-4]==255) && (tagsNum[tagindex-3]==255) && (tagsNum[tagindex-2]==255) && (tagsNum[tagindex-1]==255))
																{
																	sprintf(idstr, "%d", tagid);
																	CLCD_I2C_Clear(&LCD1);
																	CLCD_I2C_SetCursor(&LCD1,1,0);
																	CLCD_I2C_WriteString(&LCD1,"Add complete");
																  FLASH_Erasen(tagsnum_address_data);
	                                FLASH_Write_Array(tagsnum_address_data,tagsNum,100);
																	CLCD_I2C_SetCursor(&LCD1,1,1);
																	CLCD_I2C_WriteString(&LCD1,"Your ID:");
																	CLCD_I2C_WriteString(&LCD1,idstr);
																	for(int p=1;p<=5;p++)
																	{
																		tagsNum[tagindex-6+p]=sNum[p-1];
																	}
																	tagid = 0;
																	tagindex =0;
																	HAL_Delay(2000);
																	CLCD_I2C_Clear(&LCD1);
																	goto bandau;
																}
																if (i==20)
																{
																	CLCD_I2C_Clear(&LCD1);
																	CLCD_I2C_WriteString(&LCD1,"Card full");
																	HAL_Delay(2000);
																	CLCD_I2C_Clear(&LCD1);
																	goto bandau;
																	tagid = 0;
																	tagindex =0;
																}
															}
														}
													}
													break;
												case '4':
													CLCD_I2C_Clear(&LCD1);
													CLCD_I2C_WriteString(&LCD1, "Welcome admin");
													HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
													__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 750);
													HAL_Delay(500);
													HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
													HAL_Delay(4000);
													HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
													HAL_Delay(500);
													__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 250);
													HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
													CLCD_I2C_Clear(&LCD1);
													goto bandau;
													break;
												default:
													goto bandau;
											}
										}
									}	
							 }
							 for(int i=1;i<21;i++)
							 {
									tagindex = i*5;
									tagid = i;
									if((sNum[0]==tagsNum[tagindex-5]) && (sNum[1]==tagsNum[tagindex-4]) && (sNum[2]==tagsNum[tagindex-3]) && (sNum[3]==tagsNum[tagindex-2]) && (sNum[4]==tagsNum[tagindex-1]))
									{
										sprintf(idstr, "%d", tagid);
										CLCD_I2C_Clear(&LCD1);
										CLCD_I2C_SetCursor(&LCD1,1,0);
										CLCD_I2C_WriteString(&LCD1,"Welcome Bro!");
										CLCD_I2C_SetCursor(&LCD1,2,1);
										CLCD_I2C_WriteString(&LCD1,"Your ID:");
										CLCD_I2C_WriteString(&LCD1,idstr);
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
										__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 750);
										HAL_Delay(500);
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
										HAL_UART_Transmit(&huart1, (uint8_t *)idstr, strlen(idstr), 1000);
										HAL_Delay(3000);
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
										HAL_Delay(500);
										__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 250);
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
										CLCD_I2C_Clear(&LCD1);
										tagid = 0;
										tagindex =0;
										for(int p=0;p<5;p++)
										{
											str[p]=0;
										}
										goto bandau;
									}
									if(tagid==20)
									{
										CLCD_I2C_Clear(&LCD1);
										CLCD_I2C_SetCursor(&LCD1,3,0);
										CLCD_I2C_WriteString(&LCD1,"Wrong card");
										CLCD_I2C_SetCursor(&LCD1,2,1);
										CLCD_I2C_WriteString(&LCD1,"Please Retry");
										tagid = 0;
										tagindex =0;
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
										HAL_Delay(1000);
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
										HAL_Delay(1000);
										CLCD_I2C_Clear(&LCD1);
										for(int p=0;p<5;p++)
										{
											str[p]=0;
										}	
										goto bandau;
									}
							 }
						 }
						 if(key=='#')
						 {
							 CLCD_I2C_Clear(&LCD1);
							 goto bandau;
						 }
						 if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==0)
						 {
							 CLCD_I2C_Clear(&LCD1);
							 CLCD_I2C_SetCursor(&LCD1,2,0);
							 CLCD_I2C_WriteString(&LCD1,"Being opened");
							 CLCD_I2C_SetCursor(&LCD1,2,1);
							 CLCD_I2C_WriteString(&LCD1,"from inside!");
							 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
							 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 750);
							 HAL_Delay(500);
							 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
							 HAL_Delay(4000);
							 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
							 HAL_Delay(500);
							 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 250);
							 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
							 CLCD_I2C_Clear(&LCD1);
							 
						 }
					}
					break;			
				default: // X? lý các ký t? khác
					if (key >= '0' && key <= '9' && pasindex < 6) // Ch? ch?p nh?n s? và t?i da 6 ký t?
					{
						inputpas[pasindex++] = key;
						inputpas[pasindex] = '\0'; // K?t thúc chu?i
						CLCD_I2C_SetCursor(&LCD1, pasindex - 1, 1);
						CLCD_I2C_WriteChar(&LCD1, '*'); // Hi?n th? d?u * thay cho s?
					}
			}
			if (pasindex == 6) // Ðã nh?p d? 6 ký t?
			{
				CLCD_I2C_Clear(&LCD1);
				for (int i = 0; i < 6; i++)
				{
					if (inputpas[i] != pas[i])
					{
						CLCD_I2C_WriteString(&LCD1, "Wrong password!");
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
						HAL_Delay(1000);
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
						HAL_Delay(1000);
						wrongpascount++;
						goto Binh1; 
					}
				}
				CLCD_I2C_WriteString(&LCD1, "OKE Password!");
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 750);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
				HAL_Delay(4000);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
				HAL_Delay(500);
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 250);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
				wrongpascount=0;
				Binh1:
				if (wrongpascount==4)
				{
					CLCD_I2C_Clear(&LCD1);
					CLCD_I2C_SetCursor(&LCD1,3,0);
					CLCD_I2C_WriteString(&LCD1,"Warning!!!");
					CLCD_I2C_SetCursor(&LCD1,3,1);
					CLCD_I2C_WriteString(&LCD1,"Please wait");
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
					HAL_Delay(5000);
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
					wrongpascount=0;
				}
				CLCD_I2C_Clear(&LCD1);
				pasindex = 0;
				inputpas[0] = '\0';
			}
		}
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==0)
		{
			CLCD_I2C_Clear(&LCD1);
			CLCD_I2C_SetCursor(&LCD1,2,0);
			CLCD_I2C_WriteString(&LCD1,"Being opened");
			CLCD_I2C_SetCursor(&LCD1,2,1);
			CLCD_I2C_WriteString(&LCD1,"from inside!");
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 750);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
			HAL_Delay(4000);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
			HAL_Delay(500);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 250);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
			pasindex = 0;
			inputpas[0] = '\0';
			CLCD_I2C_Clear(&LCD1);
		}
	}	
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 31;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BUZZER_Pin|SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RST_Pin|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUZZER_Pin SDA_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin|SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RST_Pin PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = RST_Pin|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
