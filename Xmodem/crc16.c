#include "crc16.h"


uint16_t CalCRC16(const uint8_t* pfu8_Data, int32_t fs32_Len)
{
	uint8_t i;
	uint16_t lu16_CRC = 0;

    while(--fs32_Len >= 0)
    {
    	i = 8;
    	lu16_CRC = lu16_CRC ^ (((uint16_t)*pfu8_Data++) << 8);

    	do
        {
    		if (lu16_CRC & 0x8000)
    		{
    			lu16_CRC = lu16_CRC << 1 ^ 0x1021;
    		}
    		else
    		{
    			lu16_CRC = lu16_CRC << 1;
    		}
        }
    	while(--i);
    }
    return lu16_CRC;
}
