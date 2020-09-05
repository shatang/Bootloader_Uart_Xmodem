/* ����ͷ�ļ� *****************************************************************/
#include "download.h"
#include "common.h"

#include "Rs232_Uart2_Driver.h"
#include "Rs485_Uart4_Driver.h"
#include "Timer_Driver.h"

#include "flash_if.h"
/* �������� ------------------------------------------------------------------*/
uint8_t tab_1024[1024] = {0};


/********************************************************************************
* ������ : IAP_Init
* ��  �� : IAP��������ײ�������ʼ��
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.18
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void IAP_Init(void)
{
	TIM3_Init();
    Uart2_Init(38400,USART_WordLength_8b,USART_Parity_No,USART_StopBits_1);//38400��8-n-1
    Uart4_Init(38400,USART_WordLength_8b,USART_Parity_No,USART_StopBits_1);//38400��8-n-1 
}
/********************************************************************************
* ������ : IAP_Close
* ��  �� : IAP���ܹر�
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.18
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void IAP_Close(void)
{
    TIM_Cmd(TIM3, DISABLE);
    USART_Cmd(USART2, DISABLE);
    USART_Cmd(UART4,DISABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, DISABLE);   
}



static const char cStartTips[] 			= "\r\n\r\nready to upgrade\r\n�ֿɴ����ļ�\r\n\r\n";
static const char cFinishThreadTips[] 	= "\r\n\r\nupgrade successfully ,please  re-power the indicator\r\n�����ɹ��������������ϵ�\r\n\r\n";

static const char cErrorOverFlow[]		= "\r\nInsufficient storage space, file unavailable, upgrade failed\r\n�����ļ������洢�ռ䣬����ʧ��\r\n";
static const char cErrorVerification[] 	= "\r\nverification failed, upgrade unsuccessful\r\nУ��ʧ�ܣ��������ɹ�\r\n";
/********************************************************************************
* ������ : SerialDownload
* ��  �� : ͨ�����ڽ���һ���ļ�
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void SerialDownload(uint8_t fu8_Channal)
{
    int32_t Size = 0;

    SerialPutString(fu8_Channal, "׼������������Flash��...(���ɿ��ո��)\n\r");
    SerialPutString(fu8_Channal, "�ȴ���Ļ��ʾ'C'�����󣬷��ɴ����ļ�\n\r");
    FLASH_If_Erase(APPLICATION_START_ADDRESS);
    SerialPutString(fu8_Channal, cStartTips);
    Size = Xmodem_Receive(fu8_Channal, &tab_1024[0]);
    if (Size > 0)
    {
        SerialPutString(fu8_Channal,cFinishThreadTips);
    }
    else if (Size == -2)
        SerialPutString(fu8_Channal,cErrorVerification);

    else if (Size == -3)
        SerialPutString(fu8_Channal, "\r\n\nAborted by user.\n\r");
    else
        SerialPutString(fu8_Channal, "\n\rFailed to receive the file!\n\r");
}

#define CHECK_VALID_CHAR        (' ')
#define CHECK_CHAR_LIMIT_LEN    (15)
#define TIME_DELAY_COUNT        (8000)

static uint8_t Uart_Channel = 0;
/********************************************************************************
* ������ : Serial_GetDownloadChannel
* ��  �� : ��ȡ��ǰ���������ļ���ͨ��
* ˵  �� : none
* ��  �� : none
* ��  �� : ��ǰ���������ļ���ͨ��
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
uint8_t Serial_GetDownloadChannel(void)
{
    return Uart_Channel;
}
/********************************************************************************
* ������ : Serial_CheckDownloadChar
* ��  �� : ��鴮���Ƿ񱻰��£��ҷ��Ϳո��ַ�' '
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
uint8_t Serial_CheckDownloadChar(void)
{
    uint8_t lu8_RxChar[Uart_NumMax] = {0};
    uint8_t lu8_RxLen[Uart_NumMax] = {0};
    uint32_t TickCache = 0;//ʹ��whileѭ�������þֲ�����
    
    TickCache = SysTimeGet();
    while(1)
    {      
        //����2
        if(SerialKeyPressed(Uart2_232, &lu8_RxChar[Uart2_232]))
        {
            if (CHECK_VALID_CHAR == lu8_RxChar[Uart2_232])
            {
                lu8_RxChar[Uart2_232] = 0;
                if (++lu8_RxLen[Uart2_232] > CHECK_CHAR_LIMIT_LEN)
                {
                    lu8_RxLen[Uart2_232] = 0;
                    Uart_Channel = Uart2_232;
                    return 1;
                }
            }
            else
                lu8_RxLen[Uart2_232] = 0;
        }
        //����4
        if(SerialKeyPressed(Uart4_485, &lu8_RxChar[Uart4_485]))
        {
            if (CHECK_VALID_CHAR == lu8_RxChar[Uart4_485])
            {
                lu8_RxChar[Uart4_485] = 0;
                if (++lu8_RxLen[Uart4_485] > CHECK_CHAR_LIMIT_LEN)
                {
                    lu8_RxLen[Uart4_485] = 0;
                    Uart_Channel = Uart4_485;
                    return 2;
                }   
            }
            else
                lu8_RxLen[Uart4_485] = 0;
        }
        //��ⳬʱ
        if(SysTimeGet() - TickCache > TIME_DELAY_COUNT)return 0;
    }
    return 0;
}
