/**
 * @file
 *
 * @brief Local header file for Radar HAL.
 */

#ifndef RADAR_HAL_H
#define RADAR_HAL_H
#include <stdint.h>

/**
 * Structure to hold information about the current radar.
 */


#ifdef __cplusplus
extern "C" {
#endif


//int pinMode(int pin, int mode);



/**
 * Calculate necessary memory for radar_hal_init.
 *
 * Return value used to allocate memory before calling @ref radar_hal_init.
 *
 * @return  Memory size in bytes needed to initialize Radar HAL.
 */
typedef struct {
    int radar_id;                   ///< Id of current radar
    int spi_handle; ///< Pointer to spi handle used to communicate with the radar IC
    int speed;
}radar_handle_t;

/**
 * Initiate Radar HAL
 *
 * @param  radar_handle    Pointer to Radar handle is returned in this variable
 * @param  instance_memory Pointer to memory allocated before calling this function
 * @return                 Status of execution.
 */
int radar_hal_init(radar_handle_t* handler);

/**
 * Set Radar IC enable pin to specified level.
 *
 * @param  radar_handle Pointer to initialized Radar handle
 * @param  value        Value/level to set enable pin to
 * @return              Status of execution.
 */
int radar_hal_pin_set_enable(radar_handle_t * radar_handle, int value);

/**
 * Write data to Radar using SPI
 *
 * @param  radar_handle Pointer to initialized Radar handle
 * @param  data         Pointer to buffer with data to write
 * @param  length       Length of data to write
 * @return              Status of execution.
 */
int radar_hal_spi_write(radar_handle_t * radar_handle, uint8_t* data, uint32_t length);

/**
 * Read data from Radar using SPI
 *
 * @param  radar_handle Pointer to initialized Radar handle
 * @param  data         Pointer to buffer where data is returned
 * @param  length       Length of data to read
 * @return              Status of execution.
 */
int radar_hal_spi_read(radar_handle_t * radar_handle, uint8_t* data, uint32_t length);

/**
 * Write and read data to Radar using SPI
 *
 * @param  radar_handle Pointer to initialized Radar handle
 * @param  wdata        Pointer to buffer with data to write
 * @param  wlen      Length of data to write
 * @param  rdata        Pointer to buffer where data is returned
 * @param  rlen      Length of data to read
 * @return              Status of execution.
 */
int radar_hal_spi_write_read(radar_handle_t* radar_handle, uint8_t* wdata, uint32_t wlen, uint8_t* rdata, uint32_t rlen);

void radar_hal_deinit(radar_handle_t* radar_handle);

typedef int (*data_ready_cb)(void);

 void  x4_isr_init();

#ifdef __cplusplus
}
#endif

#endif // RADAR_HAL_H
