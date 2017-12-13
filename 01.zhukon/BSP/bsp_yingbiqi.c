#include "bsp_yingbiqi.h"

//硬币器，纸币器初始化，串口指令延时等待标志位
#define FLAG_WAIT   1

//功能：发送常规命令函数
//入口参数：basic_cmd为发送的常规命令字节，data为需要发送数据区常规指令的数据
//返回值：正常返回1，异常返回0
//说明：MDB协议定义了地址字节的格式，低3位为命令值，高5位为硬币识别器地址。命令后，接着CHK检验和
u8 Send_CMD_BASIC_coin(u8 basic_cmd, u8 *data)
{
    u16 cmd = 0;
    u8 dat_len = 0, i = 0;
    u8 dat[5] = {0};
    u8 num = 0;     //数据区总和

    if(IS_COIN_BASIC_COMMAND(basic_cmd) == 0)     //常规命令校验
    {
        return 0;   //如果不是定义的命令,会直接退出
    }

    //根据常规指令设置数据区长度，纸币器和硬币器
    if((COIN_TYPE_YING == basic_cmd) || (BILL_TYPE_ZHI == basic_cmd))
    {
        dat_len = DAT_COIN_TYPE;    //VMC数据长度为4
    }
    else if((DISPENSE_YING == basic_cmd) || (ESCROW_ZHI == basic_cmd))
    {
        dat_len = DAT_DISPENSE;    //VMC数据长度为1
    }
    else if(SECURITY_ZHI == basic_cmd)
    {
        dat_len = DAT_SECURITY_ZHI;    //VMC数据长度为2
    }

    cmd = (0x01 << 8) | basic_cmd;  //对应模式位置1，表示地址字节
    USART_Send2Byte(USART2, cmd);   //发送对应地址字节
    USART_Send2Byte(USART1, cmd);   //PC调试，发送对应地址字节

    if(dat_len != 0)    //常规指令对应数据区初始化及发送指令
    {
        for(i = 0; i < dat_len; i++)
        {
            dat[i] = data[i];
            USART_SendByte(USART2, dat[i]);     //发送基础指令数据区
            USART_SendByte(USART1, dat[i]);     //PC调试，发送基础指令数据区
            num += dat[i];      //检验和
        }
    }

    cmd = basic_cmd + num;          //计算校验和
    USART_Send2Byte(USART2, cmd);           //发送CHK检验和
    USART_Send2Byte(USART1, cmd);           //PC调试，发送CHK检验和
    return 1;
}

//功能：发送扩展命令函数
//入口参数：exp_cmd为发送的扩展命令字节，data为需要发送数据区扩展指令的数据
//说明：MDB协议定义了地址字节的格式，低3位为命令值，高5位为硬币识别器地址
u8 Send_CMD_EXP_coin(u16 exp_cmd, u8 *data)
{
    u16 cmd = 0;
    u8 dat_len = 0, i = 0;
    u8 dat[5] = {0};
    u8 num = 0;     //数据区总和

    if((IS_COIN_EXP_COMMAND_YING(exp_cmd) == 0) && (IS_COIN_EXP_COMMAND_ZHI(exp_cmd) == 0))     //地址字节命令校验
    {
        return 0;   //如果不是定义的命令,会直接退出
    }

    //根据扩展指令设置数据区长度
    if((FEATURE_ENABLE_YING == exp_cmd) || (FEATURE_ENABLE_ZHI == exp_cmd))
    {
        dat_len = DAT_FEATURE_ENABLE_YING;
    }
    else if(PAYOUT_YING == exp_cmd)
    {
        dat_len = DAT_PAYOUT_YING;
    }

    if(IS_COIN_EXP_COMMAND_YING(exp_cmd) == 1)     //硬币器扩展指令
    {
        cmd = (0x01 << 8) | 0x0F;           //对应模式位置1，表示地址字节,所以扩展字节高位都是0x0F
        USART_Send2Byte(USART2, cmd);       //发送对应地址字节
        USART_Send2Byte(USART1, cmd);       //PC调试，发送对应地址字节
    }
    else
    {
        //纸币器扩展指令
        cmd = (0x01 << 8) | 0x37;           //对应模式位置1，表示地址字节,所以扩展字节高位都是0x37
        USART_Send2Byte(USART2, cmd);       //发送对应地址字节
        USART_Send2Byte(USART1, cmd);       //PC调试，发送对应地址字节
    }

//    if(IS_COIN_EXP_COMMAND_ZHI(exp_cmd) == 1)     //纸币器扩展指令
//    {
//        cmd = (0x01 << 8) | 0x37;           //对应模式位置1，表示地址字节,所以扩展字节高位都是0x37
//        USART_Send2Byte(USART2, cmd);       //发送对应地址字节
//        USART_Send2Byte(USART1, cmd);       //PC调试，发送对应地址字节
//    }
    cmd = (exp_cmd & 0xFF);             //副指令，实际测试结果不需要地址位置1
    USART_Send2Byte(USART2, cmd);       //发送副指令
    USART_Send2Byte(USART1, cmd);       //PC调试，发送副指令

    if(dat_len != 0)    //扩展指令对应数据区初始化及发送指令
    {
        for(i = 0; i < dat_len; i++)
        {
            dat[i] = data[i];
            USART_SendByte(USART2, dat[i]);     //发送扩展数据区
            USART_SendByte(USART1, dat[i]);     //PC调试，发送扩展数据区
            num += dat[i];      //检验和
        }
    }

    cmd = (exp_cmd >> 0x08) + (exp_cmd & 0xFF) + num;       //计算校验和
    USART_SendByte(USART2, (u8) cmd);          //发送CHK检验和
    USART_SendByte(USART1, (u8) cmd);          //PC调试，发送CHK检验和
    return 1;
}

//功能：由扩展命令解析出需要的数据区长度
//说明：返回值为需要的数据区长度值，出错返回FLAG_ERROR_EXP
//u8 Get_len_EXP_coin(u16 exp_cmd)
//{
//    if(IDETIFICATION_EXP == exp_cmd)
//    {
//        return LEN_IDENTIFICATION_YING;
//    }
//    else if(FEATURE_ENABLE_YING == exp_cmd)
//    {
//        return LEN_FEATURE_ENABLE_YING;
//    }
//    else if(PAYOUT_YING == exp_cmd)
//    {
//        return LEN_PAYOUT_YING;
//    }
//    else if(PAYOUT_STATUS_YING == exp_cmd)
//    {
//        return LEN_PAYOUT_STATUS_YING;
//    }
//    else if(PAYOUT_VALUE_POLL_YING == exp_cmd)
//    {
//        return LEN_PAYOUT_VALUE_POLL_YING;
//    }
//    else if(SEND_DIAGNOSTIC_YING == exp_cmd)
//    {
//        return LEN_SEND_DIAGNOSTIC_YING;
//    }
//    else
//        return FLAG_ERROR_EXP;  //自定义，值区别于其他的返回值
//}

//功能：发送带地址的多个命令函数
//入口参数：cmd为发送命令的地址，len为命令长度
//
void Send_CMD_coin(u8 *cmd, u8 len)
{
}

//功能：硬币器初始化
//入口参数：
//说明：硬币器初始化函数，按照文档初始化流程
void YingBiQi_Init(void)
{
    u8 coin_dat[4] = {0};
    Send_CMD_BASIC_coin(RESET_YING, NULL);      //发送复位指令
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    Send_CMD_BASIC_coin(STATUS_YING, NULL);      //发送硬币器状态指令
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    Send_CMD_EXP_coin(IDENTIFICATION_YING, NULL);     //发送扩展指令0x0F00
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    coin_dat[0] = 0x00;    //发送的第一个字节，实际顺序待测
    coin_dat[1] = 0x00;
    coin_dat[2] = 0x00;
    coin_dat[3] = 0x03;
    Send_CMD_EXP_coin(FEATURE_ENABLE_YING, coin_dat);     //发送扩展指令0x0F01和数据区
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    Send_CMD_BASIC_coin(TUBE_STATUS_YING, NULL);    //发送钱管状态指令，回复剩余各个钱管状态
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    Send_CMD_BASIC_coin(POLL_YING, NULL);    //回复机器动作类型
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    Send_CMD_EXP_coin(SEND_DIAGNOSTIC_YING, NULL);     //发送扩展指令0x0F05
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    coin_dat[0] = 0x00;
    coin_dat[1] = 0x03;
    coin_dat[2] = 0xFF;
    coin_dat[3] = 0xFF;
    Send_CMD_BASIC_coin(COIN_TYPE_YING, coin_dat);    //回复机器可用硬币类型
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif

    while(1)
    {
        Send_CMD_BASIC_coin(TUBE_STATUS_YING, NULL);    //发送钱管状态指令，回复剩余各个钱管状态
#if(FLAG_WAIT == 1)
        delay_ms(10);
#endif
        Send_CMD_BASIC_coin(POLL_YING, NULL);    //回复机器动作类型
        delay_ms(1000);
    }
}

//功能：纸币器初始化
//入口参数：
//说明：纸币器初始化函数，按照文档初始化流程
void ZhiBiQi_Init(void)
{
    u8 coin_dat[4] = {0};
    Send_CMD_BASIC_coin(POLL_ZHI, NULL);      //发送指令0x33
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    Send_CMD_BASIC_coin(STATUS_ZHI, NULL);      //发送状态指令0x31
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    Send_CMD_EXP_coin(IDENTIFICATION_ZHI, NULL);      //发送扩展指令0x3700
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    Send_CMD_BASIC_coin(STACKER_ZHI, NULL);      //发送指令0x36
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
    coin_dat[0] = 0x00;    //发送的第一个字节，实际顺序待测
    coin_dat[1] = 0x0F;     //纸币器可使用纸币类型B0~B3，1,5,10,20
    coin_dat[2] = 0x00;
    coin_dat[3] = 0x0F;     //使用暂保留功能
    Send_CMD_BASIC_coin(BILL_TYPE_ZHI, coin_dat);      //发送指令0x34
#if(FLAG_WAIT == 1)
    delay_ms(10);
#endif
//    Send_CMD_BASIC_coin(POLL_ZHI, NULL);      //发送指令0x33

    while(1)
    {
        Send_CMD_BASIC_coin(POLL_ZHI, NULL);      //发送指令0x33
        delay_ms(500);
    }
}



