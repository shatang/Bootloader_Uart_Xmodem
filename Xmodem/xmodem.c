#include "xmodem.h"
#include "flash_if.h"
#include "common.h"
#include "crc16.h"

/********************************************************************************
* ������ :  Receive_Byte
* ��  �� :  �涨ʱ���ڣ����յ�һ���ֽ�
* ˵  �� :  �ӿڻ�������ymodem.c�ļ�����
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
********************************************************************************/
static int32_t Receive_Byte (uint8_t fu8_Channel,uint8_t *c, uint32_t fu32_timeout)
{
	while(fu32_timeout -- > 0)
    {
        if(SerialKeyPressed(fu8_Channel,c) == 1)return 0;
    }
	
    return -1;
}
/********************************************************************************
* ������ :  Send_Byte
* ��  �� :  ����һ���ֽ�
* ˵  �� :  �ӿڻ�������ymodem.c�ļ�����
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
********************************************************************************/
static uint32_t Send_Byte (uint8_t fu8_Channel,uint8_t c)
{
    Serial_PutChar(fu8_Channel,c);
    return 0;
}
/********************************************************************************
* ������ : Receive_Packet
* ��  �� : �ӷ��Ͷ˽���һ�����ݰ�
* ˵  �� : none
* ��  �� : data ������ָ��
           length������
           timeout ����ʱʱ��

* ��  �� : ���յĽ��
            0: ��������
            -1: ��ʱ�������ݰ�����
            1: �û�ȡ��
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
static int32_t Receive_Packet (uint8_t fu8_Channal, uint8_t *data, int32_t *length, uint32_t timeout)
{
    uint16_t i, packet_size, computedcrc;
    uint8_t c;
    *length = 0;
    if (Receive_Byte(fu8_Channal, &c, timeout) != 0)
    {
        return -1;
    }
    switch (c)
    {
        case SOH:
            packet_size = PACKET_SIZE;
            break;
        case STX:
            packet_size = PACKET_1K_SIZE;
            break;
        case EOT:
            return 0;
        case CA:
            if ((Receive_Byte(fu8_Channal, &c, timeout) == 0) && (c == CA))
            {
                *length = -1;
                return 0;
            }
            else
            {
                return -1;
            }
        default:
            return -1;
    }
    *data = c;
    for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
    {
        if (Receive_Byte(fu8_Channal, data + i, timeout) != 0)
        {
            return -1;
        }
    }
    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))//���ݰ��� & ���ݰ�������
    {
        return -1;
    }
    
    computedcrc = CalCRC16(&data[3], packet_size);
    if(computedcrc != (uint16_t)((data[packet_size+3]<<8) | data[packet_size+4]))
    {
        return -1;
    }
    
    *length = packet_size;
    return 0;
}

static const char cPassWCode[32] = "Shatang-Shatang-Shatang-Shatang-";//������ܵ�����
/********************************************************************************
* ������ : Xmodem_Receive
* ��  �� : ͨ�� ymodemЭ�����һ���ļ�
* ˵  �� : none
* ��  �� : buf: �׵�ַָ��
* ��  �� : �ļ���С(����)
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
int32_t Xmodem_Receive(uint8_t fu8_Channal, uint8_t *buf)
{
    uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD] = {0};
    uint8_t *file_ptr;
    uint8_t *buf_ptr;

    int32_t i = 0; 
    
    int32_t session_begin = 0;  //�Ự��ʼ��־λ
    int32_t session_done = 0;   //�Ự������־λ
    int32_t file_done = 0;      //�ļ�������ɱ�־λ
    int32_t packet_length = 0;      //���ݰ�������������
    int32_t packets_received = 0;   //���ݰ����ո���
    int32_t errors;                 //���ݰ����մ������
    
	uint32_t RamSource = 0;
    
    uint32_t FlashDestination = 0;
    
    //��ʼ��Flash��ַ����
    FlashDestination = APPLICATION_START_ADDRESS;

    for (session_done = 0, errors = 0, session_begin = 0; ;)
    {
        for (packets_received = 1, file_done = 0, buf_ptr = buf; ;)
        {
            switch (Receive_Packet(fu8_Channal, packet_data, &packet_length, NAK_TIMEOUT))
            {
                case 0:
                    errors = 0;
                    switch (packet_length)
                    {
                        //���Ͷ���ֹ
                        case -1:
                            Send_Byte(fu8_Channal, ACK);
                            return 0;
                        //��������
                        case 0:
                            Send_Byte(fu8_Channal, ACK);
                            file_done = 1;
                            session_done = 1;
                            break;
                        //���������ݰ�
                        default:
                            if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))//���ݰ���� & ��������������
                            {
                                Send_Byte(fu8_Channal, NAK);
                            }
                            else
                            {
                                uint8_t lu8_Index = 0;
                                
                                memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
                                RamSource = (uint32_t)buf;

//                                //��(����)�ļ����н���
//                                for(i = 0,lu8_Index = 0; i < packet_length; i++)
//                                {
//                                    buf_ptr[i] ^= cPassWCode[lu8_Index++];
//                    
//                                    if (lu8_Index >= sizeof(cPassWCode))lu8_Index = 0;
//                                }

                                //�ѽ��յ������ݱ�д��Flash��
                                if (FLASH_If_Write(&FlashDestination, (uint32_t*) RamSource, (uint16_t) packet_length/4)  == 0)
                                {
                                    Send_Byte(fu8_Channal,ACK);
                                }
                                else /* An error occurred while writing to Flash memory */
                                {
                                    //����
                                    Send_Byte(fu8_Channal,CA);
                                    Send_Byte(fu8_Channal,CA);
                                    return -2;
                                }
                                packets_received ++;
                                session_begin = 1;
                            }
                   }
                break;
                case 1:
                    Send_Byte(fu8_Channal, CA);
                    Send_Byte(fu8_Channal, CA);
                    return -3;
                default:
                    if (session_begin > 0)
                    {
                        errors ++;
                    }
                    if (errors > MAX_ERRORS)
                    {
                        Send_Byte(fu8_Channal, CA);
                        Send_Byte(fu8_Channal, CA);
                        return 0;
                    }
                    Send_Byte(fu8_Channal, CRC16);
                    break;
            }
            if (file_done != 0)
            {
                break;
            }
        }
        if (session_done != 0)
        {
            break;
        }
    }
    return (int32_t)1;
}
