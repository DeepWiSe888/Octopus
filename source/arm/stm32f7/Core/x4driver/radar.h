#ifndef _RADAR_H
#define _RADAR_H 

#include "x4driver.h"

/**
 * @brief Radar task.
 *
 * @return nothing
 */
int Radar(void);
void x4driver_data_ready(void);
/**
 * @brief Initialize Radar task.
 *
 * @return Status of execution
 */
uint32_t task_radar_init(X4Driver_t** x4driver);

/*
class appDebug_xep : public AbstractDebugOutput
{
public:
    appDebug_xep(XepDispatch_t *dispatch);

    virtual uint32_t send_byte_stream(uint32_t contentID, uint32_t info, uint8_t *data, uint32_t length);
    virtual void stop();

    // cpu
    virtual uint64_t getTick_hw() ;
    virtual uint64_t getTick_os() ;

    void setDebugLevel(uint8_t debug_level){debug_level_ = debug_level;}

private:
    uint8_t debug_level_;

    XepDispatch_t* dispatch_ = nullptr;
};
*/	 
typedef struct
{
    char        flag[4];        //"x4m5";
    char        RadarID[16];    //radar id
    uint32_t    frameno;
    uint32_t    bincnt;
}tagWiFiFrameHead;
extern  uint8_t isr_done;
extern tagWiFiFrameHead   FrameHead;
extern X4Driver_t* x4driver ;
//EXPORT_C void* AppDebug_xep_new(XepDispatch_t *dispatch);
#endif // TASK_RADAR_H
