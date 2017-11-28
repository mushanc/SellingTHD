#include "bsp_common.h"

u8 UsartBuffer[USART_BUFFER_LEN] = {0}; //数据缓冲区
u16 UsartWptr = 0;
u16 UsartRptr = 0;

u8 start_flash_flag = 0;
bool flag_enable_debug = FALSE;

/*
*********************************************************************************************************
*   函 数 名: USART1_IRQHandler
*   功能说明: USART1中断处理函数.
*   形    参:  无
*   返 回 值: 无
*********************************************************************************************************
*/
#if USART1_CONFIG_ENABLED > 0
void USART1_IRQHandler(void)
{
    uint8_t nTemp;
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); //clear flag
        /************************************************/
        //USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART1);    // delete data
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

#if USART2_CONFIG_ENABLED > 0
void USART2_IRQHandler(void)
{
    uint8_t nTemp;
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif

    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); //clear flag
        /************************************************/
        USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART2);    // delete data
        USART_ClearFlag(USART2, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

#if USART3_CONFIG_ENABLED > 0
void USART3_IRQHandler(void)
{
    uint8_t nTemp;
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART3);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); //clear flag
        /************************************************/
        USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART3);    // delete data
        USART_ClearFlag(USART3, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

#if UART4_CONFIG_ENABLED > 0
void UART4_IRQHandler(void)
{
    uint8_t nTemp;
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif

    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(UART4);
        USART_ClearITPendingBit(UART4, USART_IT_RXNE); //clear flag
        /************************************************/
        USART_BufferWrite(nTemp);
        //USART_SendByte(USART2, nTemp);
    }

    if(USART_GetFlagStatus(UART4, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(UART4);    // delete data
        USART_ClearFlag(UART4, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

#if UART5_CONFIG_ENABLED > 0
void UART5_IRQHandler(void)
{
    uint8_t nTemp;
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif

    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(UART5);
        USART_ClearITPendingBit(UART5, USART_IT_RXNE); //clear flag
        /************************************************/
        //USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(UART5, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(UART5);    // delete data
        USART_ClearFlag(UART5, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

void IAP_Write_UpdateFLAG(void)
{
    uint32_t Address = 0x00;
    uint32_t Data = UPDATE_FLAG_FLASH_DATA;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    Address = UPDATE_FLAG_FLASH_ADDR;
    FLASH_ErasePage(Address);
    FLASH_ProgramWord(Address, Data);
    FLASH_Lock();
}

void IAP_Reset_UpdateFLAG(void)
{
    uint32_t Address = 0x00;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    Address = UPDATE_FLAG_FLASH_ADDR;
    FLASH_ErasePage(Address);
    FLASH_Lock();
}

u8 IAP_Read_UpdateFLAG(void)
{
    uint32_t Address = 0x00;
    uint32_t Data = UPDATE_FLAG_FLASH_DATA;
    uint32_t Data1 = UPDATE_FLAG_FLASH_DATA1;
    Address = UPDATE_FLAG_FLASH_ADDR;

    if(((*(__IO uint32_t*) Address) == Data) || ((*(__IO uint32_t*) Address) == Data1))
    {
        return 1;
    }
    else
    {
        return 0;
    }

    //if((*(__IO uint32_t*) Address) != Data)
    //{
    //return 0;
    //}
    //else
    //{
    //return 1;
    //}
}

//USART发送单字节
void USART_SendByte(USART_TypeDef* USARTx, uint8_t byte)
{
    /* 发送一个字节数据到USART */
    USART_SendData(USARTx, byte);

    /* 等待发送数据寄存器为空 */
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

//USART发送长度字符串
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *str, uint8_t len)
{
    uint8_t i;

    for(i = 0; i < len; i++)
    {
        USART_SendByte(USARTx, str[i]);
    }
}

//USART发送字符串
void USART_SendBytess(USART_TypeDef* USARTx, char *str)
{
    while(*str != '\0')
    {
        USART_SendByte(USARTx, *str);
        str++;
    }
}

//打印调试信息
void USART_DEBUG(char *str)
{
    if(flag_enable_debug)
    {
        while(*str != '\0')
        {
            USART_SendByte(USART2, *str);
            str++;
        }
    }
}

void Send_CMD(USART_TypeDef* USARTx, u8 HCMD, u8 LCMD)//无数据区数据包
{
    u8 str[8] = {0};
    u8 cnt = 0;
    u16 ncrc = 0;
    str[0] = HCMD;
    str[1] = LCMD;
    str[2] = 0;
    str[3] = 2;
    ncrc = CRC16_isr(&str[0], 4);
    str[4] = HBYTE(ncrc);
    str[5] = LBYTE(ncrc);
    str[6] = 0x0D;
    str[7] = 0x0A;

    for(cnt = 0; cnt < 8; cnt++)
    {
        USART_SendByte(USARTx, str[cnt]);
    }
}

void Send_CMD_DAT(USART_TypeDef* USARTx, u8 HCMD, u8 LCMD, char *dat, u16 dat_len)//完整数据包
{
    u8 cnt = 0;
    u16 ncrc = 0;
    u8 str[255] = {0};
    u16 datalen = 0;
    str[0] = HCMD;
    str[1] = LCMD;
    memcpy(&str[2], &dat[0], dat_len);
    datalen = dat_len + 2;
    str[dat_len + 2] = HBYTE(datalen);
    str[dat_len + 3] = LBYTE(datalen);
    ncrc = CRC16_isr(&str[0], 4 + dat_len);
    str[dat_len + 4] = HBYTE(ncrc);
    str[dat_len + 5] = LBYTE(ncrc);
    str[dat_len + 6] = 0x0D;
    str[dat_len + 7] = 0x0A;

    for(cnt = 0; cnt < 8 + dat_len; cnt++)
    {
        USART_SendByte(USARTx, str[cnt]);
    }
}

u8 USART_BufferRead(u8 *data)
{
    if(UsartRptr == UsartWptr) // empty
    {
        return 0;
    }

    *data = UsartBuffer[UsartRptr];
    //UsartRptr++;
    UsartRptr = (UsartRptr + 1) % USART_BUFFER_LEN; //保证读位置值不溢出
    return 1;
}

u16 USART_BufferLength(void)
{
    return (UsartWptr - UsartRptr + USART_BUFFER_LEN) % USART_BUFFER_LEN;
}


void USART_BufferWrite(u8 ntemp)
{
    //char strtmp[32] = {0};

    //if(UsartWptr == (UsartRptr - 1))
    if((UsartWptr + 1) % USART_BUFFER_LEN == UsartRptr) // full
    {
        return;
    }

    UsartBuffer[UsartWptr] = ntemp;

    if(UsartBuffer[UsartWptr] == 0x0A && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 1) % USART_BUFFER_LEN] == 0x0D
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 2) % USART_BUFFER_LEN] == 0x20 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 3) % USART_BUFFER_LEN] == 0x38
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 4) % USART_BUFFER_LEN] == 0x02 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 5) % USART_BUFFER_LEN] == 0x00
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 6) % USART_BUFFER_LEN] == 0x12 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 7) % USART_BUFFER_LEN] == 0x01)
    {
        //复位主控
        USART_DEBUG("zhukong reset \r\n");
        __disable_irq();
        NVIC_SystemReset();
    }
    else if(UsartBuffer[UsartWptr] == 0x0A && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 1) % USART_BUFFER_LEN] == 0x0D
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 2) % USART_BUFFER_LEN] == 0x71 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 3) % USART_BUFFER_LEN] == 0xF8
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 4) % USART_BUFFER_LEN] == 0x02 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 5) % USART_BUFFER_LEN] == 0x00
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 6) % USART_BUFFER_LEN] == 0x13 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 7) % USART_BUFFER_LEN] == 0x01)
    {
        // 复位电机
        Send_CMD(UART4, 0x02, 0x13);
        USART_DEBUG("dianji reset: 0213\r\n");
    }
    else if(UsartBuffer[UsartWptr] == 0x0A && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 1) % USART_BUFFER_LEN] == 0x0D
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 2) % USART_BUFFER_LEN] == 0xC0 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 3) % USART_BUFFER_LEN] == 0x39
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 4) % USART_BUFFER_LEN] == 0x02 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 5) % USART_BUFFER_LEN] == 0x00
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 6) % USART_BUFFER_LEN] == 0x14 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 7) % USART_BUFFER_LEN] == 0x01)
    {
        // 复位对射
        Send_CMD(UART4, 0x02, 0x14);
        USART_DEBUG("duishe reset: 0214\r\n");
    }

    UsartWptr = (UsartWptr + 1) % USART_BUFFER_LEN;

    if(UsartBuffer[UsartRptr] == 0x0A && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 1) % USART_BUFFER_LEN] == 0x0D
            && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 2) % USART_BUFFER_LEN] == 0xA0 && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 3) % USART_BUFFER_LEN] == 0x2E
            && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 4) % USART_BUFFER_LEN] == 0x02 && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 5) % USART_BUFFER_LEN] == 0x00
            && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 6) % USART_BUFFER_LEN] == 0x5A && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 7) % USART_BUFFER_LEN] == 0x01)
    {
        // 结束电机升级
        start_flash_flag = 0;
        UsartWptr = 0;
        UsartRptr = 0;
        Send_CMD(UART4, 0x02, 0x5A);
        USART_DEBUG("stop update dianji: 025A\r\n");
    }
    else if(UsartBuffer[UsartRptr] == 0x0A && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 1) % USART_BUFFER_LEN] == 0x0D
            && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 2) % USART_BUFFER_LEN] == 0x40 && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 3) % USART_BUFFER_LEN] == 0x2F
            && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 4) % USART_BUFFER_LEN] == 0x02 && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 5) % USART_BUFFER_LEN] == 0x00
            && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 6) % USART_BUFFER_LEN] == 0x5C && UsartBuffer[(USART_BUFFER_LEN + UsartRptr - 7) % USART_BUFFER_LEN] == 0x01)
    {
        // 结束对射升级
        start_flash_flag = 0;
        Send_CMD(UART4, 0x02, 0x5C);
        USART_DEBUG("stop update duishe: 025C\r\n");
    }
}

void Handle_USART_CMD(u16 Data, char *Dat, u16 dat_len)
{
    char strtmp[100] = {0};
    //sprintf(strtmp, "Data: %04X\r\n", Data);
    //USART_DEBUG(strtmp);
    //memset(strtmp, 0, sizeof(strtmp));

    if(dat_len != 0) // 处理数据区域
    {
        if(Data == DIANJI_ZHUKON_NUMb6)//货道检测异常
        {
            Send_CMD_DAT(USART3, HBYTE(ZHUKON_ANZHUO_NUMb6), LBYTE(ZHUKON_ANZHUO_NUMb6), Dat, dat_len);
            sprintf(strtmp, "ZHUKON_ANZHUO_NUMb6: %04X\r\n", ZHUKON_ANZHUO_NUMb6);
            USART_DEBUG(strtmp);
        }
        else if(Data == ANZHUO_ZHUKON_HANGLIE)  // 取"x行y列"货,发送到电机板
        {
            Send_CMD_DAT(UART4, HBYTE(ZHUKON_DIANJI_HANGLIE), LBYTE(ZHUKON_DIANJI_HANGLIE), Dat, 2);
            sprintf(strtmp, "ZHUKON_DIANJI_HANGLIE: %04X,%d-%d\r\n", ZHUKON_DIANJI_HANGLIE, Dat[0], Dat[1]);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_GetDianjiVer) // 电机版本
        {
            char strstr[10] = {0};
            sprintf(strstr, "%s", Dat);
            Send_CMD_DAT(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_GetDianjiVer), LBYTE(USARTCMD_ANDROID_ZHUKONG_GetDianjiVer), strstr, 7);
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_GetDianjiVer:%04X, %s\r\n", USARTCMD_ANDROID_ZHUKONG_GetDianjiVer, strstr);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_GetDuisheVer) // 对射版本
        {
            char strstr[10] = {0};
            sprintf(strstr, "%s", Dat);
            Send_CMD_DAT(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_GetDuisheVer), LBYTE(USARTCMD_ANDROID_ZHUKONG_GetDuisheVer), strstr, 7);
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_GetDuisheVer:%04X, %s\r\n", USARTCMD_ANDROID_ZHUKONG_GetDuisheVer, strstr);
            USART_DEBUG(strtmp);
        }
    }
    else
    {
        if(Data == DIANJI_ZHUKON_NUMb1)//出货成功
        {
            Send_CMD(USART3, HBYTE(ZHUKON_ANZHUO_NUMb1), LBYTE(ZHUKON_ANZHUO_NUMb1));
            sprintf(strtmp, "ZHUKON_ANZHUO_NUMb1:%04X\r\n", ZHUKON_ANZHUO_NUMb1);
            USART_DEBUG(strtmp);
        }
        else if(Data == DIANJI_ZHUKON_NUMb2)//出货失败
        {
            Send_CMD(USART3, HBYTE(ZHUKON_ANZHUO_NUMb2), LBYTE(ZHUKON_ANZHUO_NUMb2));
            sprintf(strtmp, "ZHUKON_ANZHUO_NUMb2:%04X\r\n", ZHUKON_ANZHUO_NUMb2);
            USART_DEBUG(strtmp);
        }
        else if(Data == DIANJI_ZHUKON_NUMb3)//取货失败
        {
            Send_CMD(USART3, HBYTE(ZHUKON_ANZHUO_NUMb3), LBYTE(ZHUKON_ANZHUO_NUMb3));
            sprintf(strtmp, "ZHUKON_ANZHUO_NUMb3:%04X\r\n", ZHUKON_ANZHUO_NUMb3);
            USART_DEBUG(strtmp);
        }
        else if(Data == DIANJI_ZHUKON_NUMb4)//取货成功
        {
            Send_CMD(USART3, HBYTE(ZHUKON_ANZHUO_NUMb4), LBYTE(ZHUKON_ANZHUO_NUMb4));
            sprintf(strtmp, "ZHUKON_ANZHUO_NUMb4:%04X\r\n", ZHUKON_ANZHUO_NUMb4);
            USART_DEBUG(strtmp);
        }
        else if(Data == DIANJI_ZHUKON_NUMb5)//层反馈异常
        {
            Send_CMD(USART3, HBYTE(ZHUKON_ANZHUO_NUMb5), LBYTE(ZHUKON_ANZHUO_NUMb5));
            sprintf(strtmp, "ZHUKON_ANZHUO_NUMb5:%04X\r\n", ZHUKON_ANZHUO_NUMb5);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ANDROID_ZHUKONG_WillUpdateZhukong) //准备升级主控
        {
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_WillUpdateZhukong), LBYTE(USARTCMD_ANDROID_ZHUKONG_WillUpdateZhukong));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_WillUpdateZhukong: %04X\r\n", USARTCMD_ANDROID_ZHUKONG_WillUpdateZhukong);
            USART_DEBUG(strtmp);
            IAP_Reset_UpdateFLAG();
            __disable_irq();
            NVIC_SystemReset();
        }
        else if(Data == USARTCMD_ANDROID_ZHUKONG_WillUpdateDianji) //准备升级电机
        {
            Send_CMD(UART4, HBYTE(USARTCMD_ZHUKONG_DIANJI_WillUpdateDianji), LBYTE(USARTCMD_ZHUKONG_DIANJI_WillUpdateDianji));
            sprintf(strtmp, "USARTCMD_ZHUKONG_DIANJI_WillUpdateDianji: %04X\r\n", USARTCMD_ZHUKONG_DIANJI_WillUpdateDianji);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_WillUpdateDianji) //返回准备升级电机
        {
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_WillUpdateDianji), LBYTE(USARTCMD_ANDROID_ZHUKONG_WillUpdateDianji));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_WillUpdateDianji: %04X\r\n", USARTCMD_ANDROID_ZHUKONG_WillUpdateDianji);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ANDROID_ZHUKONG_StartUpdateDianji) //开始升级电机
        {
            Send_CMD(UART4, HBYTE(USARTCMD_ZHUKONG_DIANJI_StartUpdateDianji), LBYTE(USARTCMD_ZHUKONG_DIANJI_StartUpdateDianji));
            sprintf(strtmp, "USARTCMD_ZHUKONG_DIANJI_StartUpdateDianji: %04X\r\n", USARTCMD_ZHUKONG_DIANJI_StartUpdateDianji);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_StartUpdateDianji) //返回开始升级电机
        {
            start_flash_flag = 1;
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_StartUpdateDianji), LBYTE(USARTCMD_ANDROID_ZHUKONG_StartUpdateDianji));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_StartUpdateDianji: %04X\r\n", USARTCMD_ANDROID_ZHUKONG_StartUpdateDianji);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_StopUpdateDianji) //返回结束升级电机
        {
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_StopUpdateDianji), LBYTE(USARTCMD_ANDROID_ZHUKONG_StopUpdateDianji));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_StopUpdateDianji: %04X\r\n", USARTCMD_ANDROID_ZHUKONG_StopUpdateDianji);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ANDROID_ZHUKONG_WillUpdateDuishe) //准备升级对射
        {
            Send_CMD(UART4, HBYTE(USARTCMD_ZHUKONG_DIANJI_WillUpdateDuishe), LBYTE(USARTCMD_ZHUKONG_DIANJI_WillUpdateDuishe));
            sprintf(strtmp, "USARTCMD_ZHUKONG_DIANJI_WillUpdateDuishe: %04X\r\n", USARTCMD_ZHUKONG_DIANJI_WillUpdateDuishe);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_WillUpdateDuishe) //返回准备升级对射
        {
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_WillUpdateDuishe), LBYTE(USARTCMD_ANDROID_ZHUKONG_WillUpdateDuishe));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_WillUpdateDuishe: %04X\r\n", USARTCMD_ANDROID_ZHUKONG_WillUpdateDuishe);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ANDROID_ZHUKONG_StartUpdateDuishe) //开始升级对射
        {
            Send_CMD(UART4, HBYTE(USARTCMD_ZHUKONG_DIANJI_StartUpdateDuishe), LBYTE(USARTCMD_ZHUKONG_DIANJI_StartUpdateDuishe));
            sprintf(strtmp, "USARTCMD_ZHUKONG_DIANJI_StartUpdateDuishe: %04X\r\n", USARTCMD_ZHUKONG_DIANJI_StartUpdateDuishe);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_StartUpdateDuishe) //返回开始升级对射
        {
            start_flash_flag = 1;
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_StartUpdateDuishe), LBYTE(USARTCMD_ANDROID_ZHUKONG_StartUpdateDuishe));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_StartUpdateDuishe: %04X\r\n", USARTCMD_ANDROID_ZHUKONG_StartUpdateDuishe);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_StopUpdateDuishe) //返回结束升级对射
        {
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_StopUpdateDuishe), LBYTE(USARTCMD_ANDROID_ZHUKONG_StopUpdateDuishe));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_StopUpdateDuishe: %04X\r\n", USARTCMD_ANDROID_ZHUKONG_StopUpdateDuishe);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ANDROID_ZHUKONG_GetZhukongVer) // 主控版本
        {
            char strstr[10] = {0};
            sprintf(strstr, "%s.%s%s", Version_Year, Version_Month, Version_Day);
            Send_CMD_DAT(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_GetZhukongVer), LBYTE(USARTCMD_ANDROID_ZHUKONG_GetZhukongVer), strstr, 7);
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_GetZhukongVer: %04X, %s\r\n", USARTCMD_ANDROID_ZHUKONG_GetZhukongVer, strstr);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ANDROID_ZHUKONG_GetDianjiVer)  // 获取电机版本
        {
            Send_CMD(UART4, HBYTE(USARTCMD_ZHUKONG_DIANJI_GetDianjiVer), LBYTE(USARTCMD_ZHUKONG_DIANJI_GetDianjiVer));
            sprintf(strtmp, "USARTCMD_ZHUKONG_DIANJI_GetDianjiVer: %04X\r\n", USARTCMD_ZHUKONG_DIANJI_GetDianjiVer);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ANDROID_ZHUKONG_GetDuisheVer) // 获取对射版本
        {
            Send_CMD(UART4, HBYTE(USARTCMD_ZHUKONG_DIANJI_GetDuisheVer), LBYTE(USARTCMD_ZHUKONG_DIANJI_GetDuisheVer));
            sprintf(strtmp, "USARTCMD_ZHUKONG_DIANJI_GetDuisheVer: %04X\r\n", USARTCMD_ZHUKONG_DIANJI_GetDuisheVer);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_DIANJI1VOLT) // 升降电机+门电机电压超压
        {
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_DIANJI1VOLT), LBYTE(USARTCMD_ANDROID_ZHUKONG_DIANJI1VOLT));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_DIANJI1VOLT:%04X\r\n", USARTCMD_ANDROID_ZHUKONG_DIANJI1VOLT);
            USART_DEBUG(strtmp);
        }
        else if(Data == USARTCMD_ZHUKONG_DIANJI_DIANJI2VOLT) // 货道电机电压超压
        {
            Send_CMD(USART3, HBYTE(USARTCMD_ANDROID_ZHUKONG_DIANJI2VOLT), LBYTE(USARTCMD_ANDROID_ZHUKONG_DIANJI2VOLT));
            sprintf(strtmp, "USARTCMD_ANDROID_ZHUKONG_DIANJI2VOLT:%04X\r\n", USARTCMD_ANDROID_ZHUKONG_DIANJI2VOLT);
            USART_DEBUG(strtmp);
        }
        else if(Data == 0x0117) // 开启打印
        {
            if(!flag_enable_debug)
            {
                flag_enable_debug = TRUE;
            }

            USART_DEBUG("debug\r\n");
        }
    }
}



