#include "sys.h"

void Cache_Enable(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();
	  SCB->CACR|=1<<2;
}


void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
	
    __HAL_RCC_PWR_CLK_ENABLE();
 
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
      
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;
    RCC_OscInitStructure.PLL.PLLM=pllm;
    RCC_OscInitStructure.PLL.PLLN=plln;
    RCC_OscInitStructure.PLL.PLLP=pllp;
    RCC_OscInitStructure.PLL.PLLQ=pllq;
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);
    if(ret!=HAL_OK) while(1);
    
    ret=HAL_PWREx_EnableOverDrive();
    if(ret!=HAL_OK) while(1);
    
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4;
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2;
    
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7);
    if(ret!=HAL_OK) while(1);
}


#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

u8 Get_ICahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>17)&0X01;
    return sta;
}

u8 Get_DCahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>16)&0X01;
    return sta;
}
/*

__asm void WFI_SET(void)
{
	WFI;		  
}

__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}

__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}

__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
*/
