
#include "radar_hal.h"
#include "xep_hal.h"
#include "cmd_ctrl_fpga.h"
#include "stddef.h"

int radar_hal_init(radar_handle_t* handler)
{
    uint32_t status = 0;
    if (NULL == handler) {
        return -1;
    }

    return status;
}

int radar_hal_pin_set_enable(radar_handle_t* radar_handle, int value)
{
    if(value == 1)
    {

        set_enable();
    }
    else if(value == 0)
    {
        set_disenable();
    }
    else
    {
        return XT_ERROR;
    }
    return XT_SUCCESS;
}

int radar_hal_spi_write_read(radar_handle_t* radar_handle, uint8_t* wdata, uint32_t wlen, uint8_t* rdata, uint32_t rlen)
{
    if ((0 == wlen) && (0 == rlen)) {
        return XT_SUCCESS;
    }

    spi_write_read(wdata, wlen, rdata, rlen);
    return XT_SUCCESS;
}

int radar_hal_spi_write(radar_handle_t* radar_handle, uint8_t* data, uint32_t length)
{
    if (0 == length) {
        return XT_SUCCESS;
    }
    if (NULL == data) {
        return XT_ERROR;
    }
    spi_write(data, length);

    return XT_SUCCESS;
}

int radar_hal_spi_read(radar_handle_t* radar_handle, uint8_t* data, uint32_t length)
{
    if (0 == length) {
        return XT_SUCCESS;
    }
    if (NULL == data) {
        return XT_ERROR;
    }

    return XT_SUCCESS;
}


void x4_isr_init()
{

}