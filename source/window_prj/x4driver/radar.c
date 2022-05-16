#include <unistd.h>
#include "radar_hal.h"
#include "x4driver.h"
#include <pthread.h>
#include <malloc.h>
#include <stdio.h>

#include "memory.h"
#include "radar.h"

#define RADAR_MODNAME "Radar data collector"


#define check_status_fallthrough(status, funcname) do { \
    if ((status == 0)) printf("%s SUCCESS.\n", funcname); \
    else {printf("%s fail.\n", funcname); return (-1);} \
    } while(0)
#define check_status(status, funcname) do { \
    if ((status == 0)) printf("%s SUCCESS.\n", funcname); \
    else printf("%s FAIL.\n", funcname); \
    } while(0)

typedef struct
{
    //TaskHandle_t radar_task_handle;
    radar_handle_t* radar_handle;				// Some info separating different radar chips on the same module.
} XepRadarX4DriverUserReference_t;

//volatile xtx4driver_errors_t x4_initialize_status = XEP_ERROR_X4DRIVER_UNINITIALIZED;
//X4Driver_t _x4driver;
X4Driver_t *x4driver = NULL;
radar_handle_t radar_handle_local;
XepRadarX4DriverUserReference_t x4driver_user_reference;
pthread_mutex_t x4driver_mutex;

//void* memcli = NULL;
int xgl_iter = 16;
int xgl_pulsestep = 128;
int xgl_dacmin = 949;//949;
int xgl_dacmax = 1100;//1100
float xgl_fps = 17;
int xgl_getiq = 1;
float area_from = 0.2, area_to = 3.0;

Complex data_2_math[20*17*138];

//int g_readDone = 0;
int x4driver_data_ready(void)
{
    //g_readDone = 1;
    uint32_t status = XEP_ERROR_X4DRIVER_OK;
    uint32_t bin_count = 0;
    uint8_t down_conversion_enabled = 0;
    uint32_t fdata_count;
 
    //struct timeval tv1, tv2;
    //gettimeofday(&tv1, NULL);

    x4driver_get_frame_bin_count(x4driver, &bin_count);
    x4driver_get_downconversion(x4driver, &down_conversion_enabled);

    if (down_conversion_enabled == 1)
        fdata_count = bin_count*2;
    else
        fdata_count = bin_count;
    printf("bin_count:%d\n", bin_count);
    printf("fdata_count:%d\n", fdata_count);
    uint32_t frame_counter = 0;
    datatype data_frame_normolized[fdata_count];

    status = x4driver_read_frame_normalized(x4driver, &frame_counter, data_frame_normolized, fdata_count);
    //gettimeofday(&tv2, NULL);

    // Processing
    if (XEP_ERROR_X4DRIVER_OK == status)
    {
   
        //DataIn(data_frame_normolized);
        printf("%d\n", frame_counter);
        for(int i = 0; i < 20; i ++ )
        printf("%f\n", data_frame_normolized[i]);
       
      /*  if(bin_2math == 340)
        {
            start   = 1;
            bin_2math = 0;        
        }
        else if( start  ==  0)
        {

            for(int i = 0; i < fdata_count/2; i ++)
            {
                data_2_math[i + bin_2math * (fdata_count/2)].imag = data_frame_normolized[i];
                data_2_math[i + bin_2math * (fdata_count/2)].real = data_frame_normolized[i + fdata_count / 2];
            }
            bin_2math ++;
          
        }
       */
        //
        //vpas_logv("Frame number %d, size %d, bin count %d", frame_counter, fdata_count, bin_count);
        // Push frame to memcache
        //radardata_input(memcli, frame_counter, fdata_count, data_frame_normolized);
        //ESP_LOGI("aasdasd","frame_counter %d \n", frame_counter);
        // radarQueue_push(frame_counter, fdata_count, data_frame_normolized);
        //for(i = 0; i < fdata_count; i ++ )
        //printf("%f\n", data_frame_normolized[i]);
        // worker_update(worker_radar);
    }
    else
    {
        // 不可修复错误,实际运行发现，一旦发生此错误后
        ///再也读不到任何帧, 直接退出
       // vpaslog_perror("Radar: Frame error.");
        return -1;
    }
    // x4driver_start_sweep(x4driver);
    return 0;
}


static uint32_t x4driver_callback_take_sem(void * sem, uint32_t timeout)
{
    (void)sem;(void)timeout;
    //pthread_mutex_lock(&x4driver_mutex);
    //x4driver_mutex.lock();
    return 1;
}

static void x4driver_callback_give_sem(void * sem)
{
    (void)sem;
    //pthread_mutex_unlock(&x4driver_mutex);
}


static uint32_t x4driver_callback_pin_set_enable(void* user_reference, uint8_t value)
{
    XepRadarX4DriverUserReference_t* x4driver_user_reference = (XepRadarX4DriverUserReference_t* )user_reference;
    int status = radar_hal_pin_set_enable(x4driver_user_reference->radar_handle, value);
    return status;
}

static uint32_t x4driver_callback_spi_write(void* user_reference, uint8_t* data, uint32_t length)
{
    XepRadarX4DriverUserReference_t* x4driver_user_reference = (XepRadarX4DriverUserReference_t*)user_reference;
    return radar_hal_spi_write(x4driver_user_reference->radar_handle, data, length);
}
static uint32_t x4driver_callback_spi_read(void* user_reference, uint8_t* data, uint32_t length)
{
    XepRadarX4DriverUserReference_t* x4driver_user_reference = (XepRadarX4DriverUserReference_t*)user_reference;
    return radar_hal_spi_read(x4driver_user_reference->radar_handle, data, length);
}

static uint32_t x4driver_callback_spi_write_read(void* user_reference, uint8_t* wdata, uint32_t wlength, uint8_t* rdata, uint32_t rlength)
{
    XepRadarX4DriverUserReference_t* x4driver_user_reference = (XepRadarX4DriverUserReference_t*)user_reference;
    return radar_hal_spi_write_read(x4driver_user_reference->radar_handle, wdata, wlength, rdata, rlength);
}

static void x4driver_callback_wait_us(uint32_t us)
{
    usleep(us);
}

void x4driver_enable_ISR(void* user_reference, uint32_t enable)
{
  //  XepRadarX4DriverUserReference_t* x4driver_user_reference = (XepRadarX4DriverUserReference_t*)user_reference;
  /*  if (enable == 1) {
        if (radar_hal_enable_isr(x4driver_user_reference->radar_handle, x4driver_data_ready)) {
            printf("Radar: unable to setup ISR\n");
        }
    }
    else {
        radar_hal_disable_isr(x4driver_user_reference->radar_handle);//disable Interrupt
    }*/
}

uint32_t task_radar_init()
{
    //XepRadarX4DriverUserReference_t* x4driver_user_reference = (XepRadarX4DriverUserReference_t*)malloc(sizeof(XepRadarX4DriverUserReference_t));
    memset(&x4driver_user_reference, 0, sizeof(XepRadarX4DriverUserReference_t));

    memset(&radar_handle_local, 0, sizeof(radar_handle_t));
    x4driver_user_reference.radar_handle = &radar_handle_local;
    int status = radar_hal_init(&radar_handle_local);
    check_status(status, "radar_hal_init");
   // return 0;
    //! [X4Driver Platform Dependencies]
    // X4Driver lock mechanism, including methods for locking and unlocking.
    X4DriverLock_t lock;
    lock.object = (void* )&x4driver_mutex;
    lock.lock = x4driver_callback_take_sem;
    lock.unlock = x4driver_callback_give_sem;

    // X4Driver timer for generating sweep FPS on MCU. Not used when sweep FPS is generated on X4.
    //uint32_t timer_id_sweep = 2;
    X4DriverTimer_t timer_sweep;
    //timer_sweep.object = xTimerCreate("X4Driver_sweep_timer", 1000 / portTICK_PERIOD_MS, pdTRUE, (void*)timer_id_sweep, x4driver_timer_sweep_timeout);
    //timer_sweep.configure = x4driver_timer_set_timer_timeout_frequency;

    // X4Driver timer used for driver action timeout.
    //uint32_t timer_id_action = 3;
    X4DriverTimer_t timer_action;
    //timer_action.object = xTimerCreate("X4Driver_action_timer", 1000 / portTICK_PERIOD_MS, pdTRUE, (void*)timer_id_action, x4driver_timer_action_timeout);
    //timer_action.configure = x4driver_timer_set_timer_timeout_frequency;

    // X4Driver callback methods.
    X4DriverCallbacks_t x4driver_callbacks;
    x4driver_callbacks.pin_set_enable = x4driver_callback_pin_set_enable;   // X4 ENABLE pin
    x4driver_callbacks.spi_read = x4driver_callback_spi_read;               // SPI read method
    x4driver_callbacks.spi_write = x4driver_callback_spi_write;             // SPI write method
    x4driver_callbacks.spi_write_read = x4driver_callback_spi_write_read;   // SPI write and read method
    x4driver_callbacks.wait_us = x4driver_callback_wait_us;                 // Delay method
    //  x4driver_callbacks.notify_data_ready = x4driver_notify_data_ready;      // Notification when radar data is ready to read
    //  x4driver_callbacks.trigger_sweep = x4driver_trigger_sweep_pin;          // Method to set X4 sweep trigger pin
    x4driver_callbacks.enable_data_ready_isr = x4driver_enable_ISR;         // Control data ready notification ISR


    //void* x4driver_instance_memory = malloc(x4driver_get_instance_size());//pvPortMalloc(x4driver_get_instance_size());
    //x4driver_create(x4driver, x4driver_instance_memory, &x4driver_callbacks,&lock,&timer_sweep,&timer_action, (void*)x4driver_user_reference);
    x4driver_set_predefined(x4driver, &x4driver_callbacks, &lock, &timer_sweep, &timer_action, &x4driver_user_reference);

    x4driver->spi_buffer_size = SPI_BUFFER_SIZE;
    x4driver->spi_buffer = x4driver->_spi_buffer;
    // 为什么首地址要32对齐？
    if ((((intptr_t)(x4driver->spi_buffer)) % 32) != 0)
    {
        int alignment_diff = 32 - (((intptr_t)(x4driver->spi_buffer)) % 32);
        x4driver->spi_buffer += alignment_diff;
        x4driver->spi_buffer_size -= alignment_diff;
    }
    x4driver->spi_buffer_size -= x4driver->spi_buffer_size % 32;

    return XT_SUCCESS;
}

int device_init()
{
    uint32_t status = 0;
    //initialize radar task
    status = task_radar_init();
    check_status_fallthrough(status, "task_radar_init");

    xtx4driver_errors_t tmp_status =  (xtx4driver_errors_t) x4driver_init(x4driver);
    check_status_fallthrough(tmp_status, "x4driver_init");

    status = x4driver_set_sweep_trigger_control(x4driver, SWEEP_TRIGGER_MANUAL); // By default let sweep trigger control done by X4
    //status = x4driver_set_sweep_trigger_control(x4driver,SWEEP_TRIGGER_X4); // Trigger by manual
    check_status_fallthrough(status, "x4driver_set_sweep_trigger_control");

    status = x4driver_set_dac_min(x4driver, xgl_dacmin);
    check_status_fallthrough(status, "x4driver_set_dac_min");

    status=x4driver_set_dac_max(x4driver, xgl_dacmax);
    check_status_fallthrough(status, "x4driver_set_dac_max");

    status=x4driver_set_iterations(x4driver, xgl_iter);//32);
    check_status_fallthrough(status, "x4driver_set_iterations");

    status=x4driver_set_pulses_per_step(x4driver, xgl_pulsestep);//140);
    check_status_fallthrough(status, "x4driver_set_pulses_per_step");

    status=x4driver_set_downconversion(x4driver, xgl_getiq);//1);// Radar data as downconverted baseband IQ, not RF.
    check_status_fallthrough(status, "x4driver_set_downconversion");

    status=x4driver_set_frame_area_offset(x4driver, 0); // Given by module HW. Makes frame_area start = 0 at front of module.
    check_status_fallthrough(status, "x4driver_set_frame_area_offset");

    status=x4driver_set_frame_area(x4driver, area_from, area_to); // Observe from 0.5m to 4.0m.
    check_status_fallthrough(status, "x4driver_set_frame_area");

    status = x4driver_check_configuration(x4driver);
    check_status_fallthrough(status, "x4driver_check_configuration");

    return 0;
}

uint32_t bin_cnt;

#include "defaults.h"
#include "cmd_ctrl_fpga.h"



int radar_start(void)
{
    uint32_t status = 0;
    // 参数初始化
    xgl_iter = CFG_RADAR_ITER;
    xgl_pulsestep = CFG_RADAR_STEP;
    xgl_dacmin = CFG_RADAR_DACMIN;
    xgl_dacmax = CFG_RADAR_DACMAX;
    xgl_getiq = CFG_RADAR_DOWNVER;
    xgl_fps = 17;
    area_from = 0.2;
    area_to = 7.2;
    printf(  "Radar paramers, iteration=%d, pulse step=%d, dac=(%d~%d), "
              "downversion=%d, fps=%.01f, range=(%.02f~%.02f)\n",
              xgl_iter, xgl_pulsestep, xgl_dacmin, xgl_dacmax, xgl_getiq,
              xgl_fps, area_from, area_to);

    if (NULL == x4driver) {
        x4driver = (X4Driver_t*) malloc(sizeof(X4Driver_t));
    }
    printf(  "device_init \n");
    // 雷达硬件初始化
    if (device_init()) {
        printf(  "Radar init fail.\n");
        return -1;
    }
    printf(  "Radar init ok.\n");

    //开始产生数据
    // status = x4driver_set_dac_step(x4driver, DAC_STEP_1);
    // check_status_fallthrough(status, "x4driver_set_dac_step");
    //  status = x4driver_set_fps(x4driver, xgl_fps);
    //  check_status_fallthrough(status, "x4driver_set_fps");
    // printf("Radar is ready for input\n");

    x4driver_get_frame_bin_count(x4driver, &bin_cnt);
    printf("X4driver get bin count :%d\n", bin_cnt);
    if (bin_cnt <= 5 || bin_cnt > 200) {
        printf("X4driver get bin count error\n");
        return 0;
    }
    printf("  x4driver->frame_read_size :%d\n",   x4driver->frame_read_size);
    send_frame_read_size( x4driver->frame_read_size);
    return 0;
}

void radar_stop(void* args)
{
    (void)args;
//  radar_hal_deinit(x4driver_user_reference.radar_handle);
//  radar_hal_disable_isr(x4driver_user_reference.radar_handle);
}

int radar_enable()
{
    return 1;
}


int radar_needrestart()
{
 /*   globalcfg* gcfg = globalcfg_get();
    if (xgl_fps != gcfg->radar.xgl_fps ||
            area_from != gcfg->radar.xgl_near ||
            area_to != gcfg->radar.xgl_far)
        return 1;*/
    return 0;
}
