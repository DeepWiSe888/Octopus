#include "radar_hal.h"
#include "Radar.h"
#define DEBUG
#include "x4driver.h"
#include "xep_hal.h"
//#include "usart.h"
//#include "delay.h"
//#include "hw_config.h"
//#include "MyBspInc.h"
#include "stdlib.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsmc.h"
volatile xtx4driver_errors_t x4_initialize_status = XEP_ERROR_X4DRIVER_UNINITIALIZED;
X4Driver_t* x4driver = 0;

uint8_t isr_done;
tagWiFiFrameHead   FrameHead;

//float32_t g_data_frame_normolized[2]  = 0;

//using namespace std;
//std::recursive_mutex x4driver_mutex;

//TaskHandle_t radar_task_handle;
typedef struct
{    
    radar_handle_t* radar_handle;				// Some info separating different radar chips on the same module.
} XepRadarX4DriverUserReference_t;

typedef struct
{
    X4Driver_t* x4driver;
} RadarTaskParameters_t;  

float32_t data_frame_normolized[512];
void x4driver_data_ready(void)
{
		uint32_t status = XEP_ERROR_X4DRIVER_OK;
		uint32_t bin_count = 0;
		uint32_t frame_counter;
		uint8_t down_conversion_enabled;
		uint32_t fdata_count;
		uint32_t i;

		x4driver_get_frame_bin_count(x4driver,&bin_count);
		down_conversion_enabled = 0;
		x4driver_get_downconversion(x4driver,&down_conversion_enabled);

		fdata_count = bin_count;
		if(down_conversion_enabled == 1)
		{
				fdata_count = bin_count*2;
		}

		frame_counter=0;   

		status = x4driver_read_frame_normalized(x4driver,&frame_counter,data_frame_normolized,fdata_count);

		if(XEP_ERROR_X4DRIVER_OK == status)
		{
			printf("x4 frame data ready : %d! \r\n", frame_counter);

		}
		else
		{
				printf("fail to get x4 frame data errorcode:%d! \r\n", status);
		}

		//printf("Size:%d,New Frame Data Normolized(%d){\r\n",fdata_count,frame_counter);
		for( i=0; i<fdata_count; i++)
		{
		    printf(" %f, \r\n",data_frame_normolized[i] );
		}
		//printf("}\r\n");
		x4driver_start_sweep(x4driver);
}


//void EXTI9_5_IRQHandler(void)         //ADS1293�ⲿ�жϴ�����
//{  
//	uint32_t status = XEP_ERROR_X4DRIVER_OK;
//	uint32_t bin_count = 0;
//	uint32_t frame_counter;
//	uint8_t down_conversion_enabled;
//	uint32_t fdata_count;
//	uint32_t i;

//    if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
//    { 			
//		EXTI_ClearITPendingBit(EXTI_Line7);	
//		if(isr_done) 
//		{	
//			//wake_up(Radar_event);
//			//QueueIn(&ecg_data,ECG_data) ;	//����FIFO
//			x4driver_get_frame_bin_count(x4driver,&bin_count);
//			down_conversion_enabled = 0;
//			x4driver_get_downconversion(x4driver,&down_conversion_enabled);	 		
//			fdata_count = bin_count;
//			if(down_conversion_enabled == 1)
//			{
//				fdata_count = bin_count*2;
//			} 		
//			frame_counter=0; 
//			
//			status = x4driver_read_frame_normalized(x4driver,&frame_counter,data_frame_normolized,fdata_count);
//	//		for(i = 0; i < fdata_count; i ++)
//	//		{
//	//				////printf("%f\r\n", data_frame_normolized[i]);
//	//		}
//			if(XEP_ERROR_X4DRIVER_OK == status)
//			{
//			//	//printf("x4 frame data ready!  fdata_count:%d,frame_counter:%d\r\n",fdata_count,frame_counter);
//			
//			}
//			else
//			{
//			//	//printf("fail to get x4 frame data errorcode:%d! \r\n", status);
//			}
//		}
//			
//	}  		
//} 

static uint32_t x4driver_callback_take_sem(void * sem,uint32_t timeout)
{
//    x4driver_mutex.lock();
//	mutex_lock(x4driver_mutex);
    return 1;
}

static void x4driver_callback_give_sem(void * sem)
{
//    x4driver_mutex.unlock();
//	mutex_unlock(x4driver_mutex);
}


static uint32_t x4driver_callback_pin_set_enable(void* user_reference, uint8_t value)
{
   
    int status = radar_hal_pin_set_enable(0, value);
    return status;
}

static uint32_t x4driver_callback_spi_write(void* user_reference, uint8_t* data, uint32_t length)
{
    
    return radar_hal_spi_write(0, data, length);
}
static uint32_t x4driver_callback_spi_read(void* user_reference, uint8_t* data, uint32_t length)
{
   
    return radar_hal_spi_read(0, data, length);
}

static uint32_t x4driver_callback_spi_write_read(void* user_reference, uint8_t* wdata, uint32_t wlength, uint8_t* rdata, uint32_t rlength)
{
    
    return radar_hal_spi_write_read(0, wdata, wlength, rdata, rlength);
}

static void x4driver_callback_wait_us(uint32_t us)
{
		//Delay_nUS(us);
		//printf("delay_nus\r\n");
	  delay_us(us);
}


void x4driver_enable_ISR(void* user_reference,uint32_t enable)
{
    if(enable == 1)
    {
		GPIO_EXTI_Config();

    }
    else
	{
		GPIO_DISENABLE_EXTI();
	}
	
}


	uint8_t txbuff[2];
	uint8_t rxbuff[2];
uint32_t task_radar_init(X4Driver_t** x4driver)
{

	X4DriverLock_t lock;
	X4DriverTimer_t timer_sweep;
	X4DriverTimer_t timer_action; 	
	void* x4driver_instance_memory;
	RadarTaskParameters_t* task_parameters;
	XepRadarX4DriverUserReference_t* x4driver_user_reference;
	X4DriverCallbacks_t x4driver_callbacks;
	int status;
//    x4driver_mutex = mutex_init(__FUNCTION__);
	x4driver_user_reference = (XepRadarX4DriverUserReference_t*)malloc(sizeof(XepRadarX4DriverUserReference_t));
	memset(x4driver_user_reference, 0, sizeof(XepRadarX4DriverUserReference_t));
//	vTaskDelay(50);
	delay_ms(50);
	status = radar_hal_init();
	

 radar_hal_pin_set_enable(NULL, 0);
 delay_ms(1000);
 radar_hal_pin_set_enable(NULL, 1);
  delay_ms(1000);
// while(1)
// {
//	   
//	   
//		  txbuff[0]  = 0x04 | 0x80;
//		  txbuff[1]  = 0x33 ;
//			memset(rxbuff,0,sizeof(rxbuff));
//	  	radar_hal_spi_write_read(NULL,txbuff, 2, rxbuff, 0);	

//	  	txbuff[0]  = 4;
//			memset(rxbuff,0,sizeof(rxbuff));
//	  	radar_hal_spi_write_read(NULL,txbuff, 1, rxbuff, 1);
//	  	printf("flash id 4:%x\r\n", rxbuff[0]);
// }
//	  	
	


   
//    lock.object = (void* )&x4driver_mutex;
    lock.lock = x4driver_callback_take_sem;
    lock.unlock = x4driver_callback_give_sem;


   

    x4driver_callbacks.pin_set_enable = x4driver_callback_pin_set_enable;   // X4 ENABLE pin
    x4driver_callbacks.spi_read = x4driver_callback_spi_read;               // SPI read method
    x4driver_callbacks.spi_write = x4driver_callback_spi_write;             // SPI write method
    x4driver_callbacks.spi_write_read = x4driver_callback_spi_write_read;   // SPI write and read method
    x4driver_callbacks.wait_us = x4driver_callback_wait_us;                 // Delay method
    x4driver_callbacks.enable_data_ready_isr = x4driver_enable_ISR;         // Control data ready notification ISR
    x4driver_instance_memory = (void*)malloc(x4driver_get_instance_size());//pvPortMalloc(x4driver_get_instance_size());
    //x4driver_create(x4driver, x4driver_instance_memory, &x4driver_callbacks,&lock,&timer_sweep,&timer_action, (void*)x4driver_user_reference);
    x4driver_create(x4driver, x4driver_instance_memory, &x4driver_callbacks,&lock,&timer_sweep,&timer_action, x4driver_user_reference);


#ifdef DEBUG
    if(status == XEP_ERROR_X4DRIVER_OK)
    {
        printf("x4driver_create success\r\n");
    }
    else
    {
        printf("x4driver_create unknow situcation\r\n");
    }
#endif // DEBUG

    task_parameters = (RadarTaskParameters_t*)malloc(sizeof(RadarTaskParameters_t));
    task_parameters->x4driver = *x4driver;

    task_parameters->x4driver->spi_buffer_size = 192*32;
    task_parameters->x4driver->spi_buffer = (uint8_t*)malloc(task_parameters->x4driver->spi_buffer_size);
    if ((((uint32_t)task_parameters->x4driver->spi_buffer) % 32) != 0)
    {
        int alignment_diff = 32 - (((uint32_t)task_parameters->x4driver->spi_buffer) % 32);
        task_parameters->x4driver->spi_buffer += alignment_diff;
        task_parameters->x4driver->spi_buffer_size -= alignment_diff;
    }
    task_parameters->x4driver->spi_buffer_size -= task_parameters->x4driver->spi_buffer_size % 32;

    return XT_SUCCESS;
}





int Radar(void)
{   

    uint16_t tmp;	
    xtx4driver_errors_t tmp_status;
    uint32_t status = 0;
  	printf("task_radar start!\r\n");
    //uint8_t* data_frame;
    //initialize radar task
    status = task_radar_init(&x4driver);
  	isr_done = 1;
#ifdef DEBUG
    if(status == XT_SUCCESS)
    {
	   
        printf("task_radar_init success\r\n");
    }
    else if(status == XT_ERROR)
    {
        printf("task_radar_init failure\r\n");
    }
    else
    {
        printf("task_radar_init unknow situcation\r\n");
    }
#endif // DEBUG5
	
    tmp_status = (xtx4driver_errors_t) x4driver_init(x4driver);
  	

#ifdef DEBUG
    if(tmp_status == XEP_ERROR_X4DRIVER_OK)
    {
        printf("x4driver_init success\r\n");
    }
    else
    {
        printf("x4driver_init unknow situcation\r\n");
    }
#endif // DEBUG

    status = x4driver_set_sweep_trigger_control(x4driver,SWEEP_TRIGGER_MANUAL); // By default let sweep trigger control done by X4
#ifdef DEBUG
    if(status == XEP_ERROR_X4DRIVER_OK)
    {
        printf("x4driver_set_sweep_trigger_control success\r\n");
    }
    else
    {
        printf("x4driver_set_sweep_trigger_control unknow situcation\r\n");
    }
#endif // DEBUG


//    x4_initialize_status = tmp_status;



    status=x4driver_set_dac_min(x4driver, 949);
     x4driver_get_dac_min(x4driver, &tmp);
    if(tmp == 949)
		{
				status = XEP_ERROR_X4DRIVER_OK;
		}
		else
		{
			  status = 5;
		}
    if (status != XEP_ERROR_X4DRIVER_OK)
    {
#ifdef DEBUG
        printf("Error setting dac minimum\r\n");
        printf("Error code=%d",status);
#endif
        //return 1;
    }
	
#ifdef DEBUG
    printf("x4driver_set_dac_min success\r\n");
#endif
    status=x4driver_set_dac_max(x4driver, 1100);
		x4driver_get_dac_max(x4driver, &tmp);
    if(tmp == 1100)
		{
				status = XEP_ERROR_X4DRIVER_OK;
		}
		else
		{
			  status = 5;
		}
    if (status != XEP_ERROR_X4DRIVER_OK)
    {
#ifdef DEBUG
        printf("Error setting dac maximum\r\n");
        printf("Error code=%d",status);
#endif
        return 1;
    }
#ifdef DEBUG
    printf("x4driver_set_dac_max success\r\n");
#endif
    status=x4driver_set_iterations(x4driver, 16);
    if (status != XEP_ERROR_X4DRIVER_OK)
    {
#ifdef DEBUG
        printf("Error in x4driver_set_iterations\r\n");
        printf("Error code=%d",status);
#endif
        return 1;

    }
#ifdef DEBUG
    printf("x4driver_set_iterations success\r\n");
#endif
    status=x4driver_set_pulses_per_step(x4driver, 128);
		 
    
    if (status != XEP_ERROR_X4DRIVER_OK)
    {
#ifdef DEBUG
        printf("Error in x4driver_set_pulses_per_step\r\n");
        printf("Error code=%d",status);
#endif
        return 1;

    }
#ifdef DEBUG
    printf("x4driver_set_pulses_per_step success\r\n");
#endif
    status=x4driver_set_downconversion(x4driver, 1);// Radar data as downconverted baseband IQ, not RF.
    if (status != XEP_ERROR_X4DRIVER_OK)
    {
#ifdef DEBUG
        printf("Error in x4driver_set_downconversion\r\n");
        printf("Error code=%d",status);
#endif
        return 1;

    }
#ifdef DEBUG
    printf("x4driver_set_downconversion success\r\n");
#endif


    status=x4driver_set_frame_area_offset(x4driver, 0); // Given by module HW. Makes frame_area start = 0 at front of module.
    if (status != XEP_ERROR_X4DRIVER_OK)
    {
#ifdef DEBUG
        printf("Error in x4driver_set_frame_area_offseto\r\n");
        printf("Error code=%d",status);
#endif
        return 1;

    }
#ifdef DEBUG
    printf("x4driver_set_frame_area_offset success\r\n");
#endif


     status=x4driver_set_frame_area(x4driver,(float32_t)0.5, (float32_t) 4); // Observe from 0.5m to 4.0m.
     if (status != XEP_ERROR_X4DRIVER_OK)
     	{
	      	#ifdef DEBUG
	     	printf("Error in x4driver_set_frame_area\r\n");
	     	printf("Error code=%d",status);
	     	#endif
	     	return 1;

      	}
      printf("x4driver_set_frame_area success\r\n");



    status = x4driver_check_configuration(x4driver);
#ifdef DEBUG
    if(status == XEP_ERROR_X4DRIVER_OK)
    {
        printf("x4driver_check_configuration success\r\n");
    }
    else
    {
        printf("x4driver_check_configuration unknow situcation\r\n");
    }
#endif // DEBUG



/***************set fps, this will trigger data output***************/
//		status = x4driver_set_dac_step(x4driver, DAC_STEP_1);
 //   status = x4driver_set_fps(x4driver,40); // Generate 5 frames per second
    if (status != XEP_ERROR_X4DRIVER_OK)
    {
#ifdef DEBUG
        printf("Error in x4driver_set_fps\r\n");
        printf("Error code=%d\r\n",status);
#endif
        return 1;

    }
#ifdef DEBUG
    printf("x4driver_set_fps success\r\n");
#endif


  x4driver_start_sweep(x4driver);

   
	  
	return 0;

}

