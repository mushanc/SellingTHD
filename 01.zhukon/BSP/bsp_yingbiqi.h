#ifndef __BSP_YINGBIQI_H
#define __BSP_YINGBIQI_H
#include "stm32f10x_conf.h"
#include "bsp_common.h"

/* 硬币器串口指令 */

//常规指令
#define     RESET_YING       0x08
#define     STATUS_YING      0x09
#define     TUBE_STATUS_YING 0x0A
#define     POLL_YING        0x0B
#define     COIN_TYPE_YING   0x0C
#define     DISPENSE_YING    0x0D
//扩展指令
#define     IDENTIFICATION_YING      0x0F00
#define     FEATURE_ENABLE_YING      0x0F01
#define     PAYOUT_YING              0x0F02
#define     PAYOUT_STATUS_YING       0x0F03
#define     PAYOUT_VALUE_POLL_YING   0x0F04
#define     SEND_DIAGNOSTIC_YING     0x0F05
//常规指令回复数据长度，不包括CHK校验字节
#define     LEN_RESET_YING           0
#define     LEN_STATUS_YING          23
#define     LEN_TUBE_STATUS_YING     18
#define     LEN_POLL_YING            16
#define     LEN_COIN_TYPE_YING       4
#define     LEN_DISPENSE_YING        1
//常规指令发送数据区长度，只有2条指令有数据区
#define     DAT_COIN_TYPE           4
#define     DAT_DISPENSE            1
//扩展指令回复数据长度，不包括CHK校验字节
#define     LEN_IDENTIFICATION_YING       33
#define     LEN_FEATURE_ENABLE_YING      0
#define     LEN_PAYOUT_YING              0
#define     LEN_PAYOUT_STATUS_YING       16
#define     LEN_PAYOUT_VALUE_POLL_YING   1
#define     LEN_SEND_DIAGNOSTIC_YING     16
//扩展指令发送数据区长度，只有2条指令有数据区
#define     DAT_FEATURE_ENABLE_YING      4
#define     DAT_PAYOUT_YING              1

/* 纸币器串口指令 */

//常规指令
#define     RESET_ZHI           0x30
#define     STATUS_ZHI          0x31
#define     SECURITY_ZHI        0x32
#define     POLL_ZHI            0x33
#define     BILL_TYPE_ZHI       0x34
#define     ESCROW_ZHI          0X35
#define     STACKER_ZHI         0X36
//扩展指令
#define     IDENTIFICATION_ZHI  0x3700
#define     FEATURE_ENABLE_ZHI  0x3701
#define     IDENTIFICATION_ZHI2 0x3702
//常规指令回复数据长度，不包括CHK校验字节
#define     LEN_RESET_ZHI           0
#define     LEN_STATUS_ZHI          27
#define     LEN_SECURITY_ZHI        0
#define     LEN_POLL_ZHI            16
#define     LEN_BILL_TYPE_ZHI       0
#define     LEN_ESCROW_ZHI          0
#define     LEN_STACKER_ZHI         2
//常规指令发送数据区长度，只有2条指令有数据区
#define     DAT_SECURITY_ZHI        2
#define     DAT_BILL_TYPE_ZHI       4
#define     DAT_ESCROW_ZHI          1
//扩展指令回复数据长度，不包括CHK校验字节
#define     LEN_IDENTIFICATION_ZHI      29
#define     LEN_FEATURE_ENABLE_ZHI      0
#define     LEN_IDENTIFICATION_ZHI2     33
//扩展指令发送数据区长度，只有1条指令有数据区
#define     DAT_FEATURE_ENABLE_ZHI      4


//自定义扩展指令长度处理错误标志位
#define     FLAG_ERROR_EXP      2
//发送常规指令校验，判断硬币器和纸币器
#define     IS_COIN_BASIC_COMMAND(cmd)      ((cmd == RESET_YING) || (cmd == STATUS_YING) || (cmd == TUBE_STATUS_YING) || \
               (cmd == POLL_YING) || (cmd == COIN_TYPE_YING) || (cmd == DISPENSE_YING) || (cmd == RESET_ZHI) || (cmd == STATUS_ZHI) || \
               (cmd == SECURITY_ZHI) || (cmd == POLL_ZHI) || (cmd == BILL_TYPE_ZHI) || (cmd == ESCROW_ZHI) || (cmd == STACKER_ZHI))

//发送扩展指令校验，判断硬币器
#define     IS_COIN_EXP_COMMAND_YING(exp_cmd)   ((exp_cmd == IDENTIFICATION_YING) || (exp_cmd == FEATURE_ENABLE_YING) || (exp_cmd == PAYOUT_YING) || \
                (exp_cmd == PAYOUT_STATUS_YING) || (exp_cmd == PAYOUT_VALUE_POLL_YING) || (exp_cmd == SEND_DIAGNOSTIC_YING))
//发送扩展指令校验，判断纸币器
#define     IS_COIN_EXP_COMMAND_ZHI(exp_cmd)   ((exp_cmd == IDENTIFICATION_ZHI) || (exp_cmd == FEATURE_ENABLE_ZHI) || (exp_cmd == IDENTIFICATION_ZHI2))

//发送地址字节和CHK校验和
u8 Send_CMD_BASIC_coin(u8 basic_cmd, u8 *data);
//发送扩展命令函数，包括数据区
u8 Send_CMD_EXP_coin(u16 exp_cmd, u8 *data);
//硬币器初始化
void YingBiQi_Init(void);
//纸币器初始化
void ZhiBiQi_Init(void);



#endif

