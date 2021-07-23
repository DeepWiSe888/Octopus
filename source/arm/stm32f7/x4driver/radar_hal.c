#include "radar_hal.h"
#include "xep_hal.h"
//#include "fmc.h"
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

#include "fsmc.h"

//#include "spi.h"
//#include "hw_config.h"
int x4_isr = 0;
  
void GPIO_EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOG_CLK_ENABLE();
   
    
    GPIO_Initure.Pin=GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
 
}

void EXTI9_5_IRQHandler(void)
{
 
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
	  x4_isr = 1;
}

void GPIO_DISENABLE_EXTI(void)
{

}

	

int radar_hal_init(void)
{
	int status = XT_SUCCESS; 
//    spi_init(); 
//	ENABLE_GPIO_Init();
	GPIO_EXTI_Config();
	return status;
}

int radar_hal_get_instance_size(void)
{
    int size = sizeof(radar_handle_t);
    //size += xtio_spi_get_instance_size();
    return size;
}



/************************
*
* addr 1 cmd  write:0x11 read:0x22
* addr 2 wlen  write len
* addr 3 2len  read len
* DATA 4 data  data begin
*************************/
#define SPI_CMD_ADDR		1
#define SPI_WLEN_ADDR		2
#define SPI_RLEN_ADDR   3
#define SPI_DATA_ADDR		4
#define SPI_WIRTE_CMD		0x11
#define SPI_READ_CMD		0X22
#define SPI_WR_CMD		0X33
#define SET_X4_ENABLE		0X44
#define SET_X4_DISENABLE		0X55

uint32_t radar_hal_pin_set_enable(radar_handle_t * radar_handle, uint8_t value)
{
    if(value == 1)
		{
        	fpga_write(SPI_CMD_ADDR, SET_X4_ENABLE);   //
		}			
    else if(value == 0)
		{
        	fpga_write(SPI_CMD_ADDR, SET_X4_DISENABLE);   //
		}
    else
    {
        return XT_ERROR;
    }
    return XT_SUCCESS;
}

uint32_t radar_hal_spi_write_read(radar_handle_t * radar_handle, uint8_t* wdata, uint32_t wlength, uint8_t* rdata, uint32_t rlength)
{
	int rlen = 0;
	if ((0 == wlength) && (0 == rlength))
	{
		return XT_SUCCESS;
	}
	if ((0 == wdata) || (0 == rdata))
	{
		return XT_ERROR;
	}


	for(int i = 0; i < wlength; i++)
	{
		fpga_write(SPI_DATA_ADDR + i, wdata[i]);
	}
	fpga_write(SPI_RLEN_ADDR, rlength);
	fpga_write(SPI_WLEN_ADDR, wlength);
	fpga_write(SPI_CMD_ADDR, SPI_WR_CMD);   //
  delay_us(3);
//  vTaskDelay(100);
	//需要延时等FPGA

		if(rlength < 200) 
		{
				for(int i = 0; i < rlength; i++)  //
				{

						rdata[i] = fpga_read(SPI_DATA_ADDR + i);//sh


				//printf("SPI_DATA_ADDR %d i:%x\r\n",i, rdata[i]);
				//for(int k=0;k<200;k++);
				}
			
	}else
		{
			while(1)
			{
	     if(fpga_read(205))
			 {
					for(int i = 0; i < 200; i++)  //
					{

						rdata[i] = fpga_read(SPI_DATA_ADDR + i);//sh
            rlen ++;
						if(rlen == rlength + 5)
						{
							fpga_write(205, 0);
							return 1;
						}
					}
					fpga_write(205, 0);
			 }
		 }
			printf("asdasdasdasdasd\n");
	}
		
	return XT_SUCCESS;
}


uint32_t radar_hal_spi_write(radar_handle_t * radar_handle, uint8_t* data, uint32_t length)
{
	if (0 == length)
	{
		return XT_SUCCESS;
	}
	if (0 == data)
	{
		return XT_ERROR;
	
	}
 
	for(int i = 0; i < length; i++)
	{
		// delay_us(3);
		 fpga_write(SPI_DATA_ADDR + i, data[i]);
	}
	// delay_us(3);
	fpga_write(SPI_WLEN_ADDR, length);
	//	 delay_us(3);
	fpga_write(SPI_CMD_ADDR, SPI_WIRTE_CMD);
		 delay_us(3);
	return XT_SUCCESS;
}

uint32_t radar_hal_spi_read(radar_handle_t * radar_handle, uint8_t* data, uint32_t length)
{
    if (0 == length)
    {
        return XT_SUCCESS;
    }
    if (0 == data)
    {
        return XT_ERROR;
    }

  	// delay_us(3);
    fpga_write(SPI_RLEN_ADDR, length);      //发送读长度
			// delay_us(3);
		fpga_write(SPI_CMD_ADDR, SPI_READ_CMD);//发送读指令
		
		 delay_us(3);
				//需要延时等FPGA
	  for(int i = 0; i < length; i++)
	  {
				data[i] = fpga_read(SPI_DATA_ADDR + i);
				 
	  }
 //    return spi_read( data, length);
}


