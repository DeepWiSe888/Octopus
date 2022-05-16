#ifndef VPAS_CONFIG_DEFAULTS_H
#define VPAS_CONFIG_DEFAULTS_H

/** 配置文件，默认内容 */
// 配置版本号
#define CFG_VERSION "20200313000000"
// 程序配置
// 日志输出等级
#define CFG_COMON_DL 1
// 日志输出路径文件名
#define CFG_COMON_DF "/tmp/ArtsEngine.log"
// 进程ID
#define CFG_COMON_PF "/tmp/ArtsEngine.pid"
// 日志上报地址
#define CFG_COMON_LU "https://www.qiyuku.com/api/wxService/deviceLogUpload"
// 雷达配置
#define CFG_RADAR_ITER    4
#define CFG_RADAR_STEP    2
#define CFG_RADAR_DACMIN  949
#define CFG_RADAR_DACMAX  1100
#define CFG_RADAR_DOWNVER 1
#define CFG_RADAR_FS 40
#define CFG_RADAR_AN 0.2
#define CFG_RADAR_AF 5.0
#define CFG_RADAR_LS ""
// Alg
#define CFG_ALG_DW 20
#define CFG_ALG_CW 1
#define CFG_ALG_RM 1
// 摔倒配置
#define CFG_LDALG_V "v1.0"
#define CFG_LDALG_HBIV  60
#define CFG_LDALG_WT    10
#define CFG_LDALG_CT    10
#define CFG_LDALG_FCCT  10
#define CFG_LDALG_SCCT  5
#define CFG_LDALG_PS    0
#define CFG_LDALG_DEB   0
// Outcome dealing
// 计算结果本地存储路径和文件名
#define CFG_OUTCOME_LS ""
// 计算结果实时提交地址
#define CFG_OUTCOME_WS "wss://www.qiyuku.com"
// 计算结果定时提交地址
#define CFG_OUTCOME_HB "https://www.qiyuku.com/api/wxService/heartBeat"
// 配置更新地址
#define CFG_OUTCOME_UC "https://www.qiyuku.com/api/wxService/getDeviceConfig"
// 外部定时提交地址
#define CFG_OUTCOME_EX ""
// 事件上报地址
#define CFG_OUTCOME_EU "https://www.qiyuku.com/api/deviceService/deviceEvent"
// 计算结果本地UDP端口
#define CFG_OUTCOME_BC -1
// In or out range

// Voice
#define CFG_VOICE_DEFAULT "/home/vpas/media/default.wav"

#endif // VPAS_CONFIG_DEFAULTS_H
