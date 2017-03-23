#include "string_utils.h"

uint32_t	BinToString
(
	uint8_t *data, 
	uint32_t data_len, 
	char *buffer, 
	uint32_t buffer_len
)
{
	uint32_t	i;

	if ((data_len == 0) || (buffer_len < data_len*2 + 1))
	{
		return	0;	
	}

	for(i = 0 ; i < data_len ; i++)
	{
		uint8_t	hi = (data[i] >> 4) & 0x0F;
		uint8_t	lo = data[i] & 0x0F;

		if (hi < 10)
		{
			buffer[i*2] = '0' + hi;
		}
		else
		{
			buffer[i*2] = 'A' + hi - 10;
		}

		if (lo < 10)
		{
			buffer[i*2 + 1] = '0' + lo;
		}
		else
		{
			buffer[i*2 + 1] = 'A' + lo - 10;
		}
	}

	buffer[i*2] = '\0';

	return	i*2;
}

uint32_t	StringToBin
(
	const char *data, 
	uint32_t data_len, 
	uint8_t *buffer, 
	uint32_t buffer_len
)
{
	uint32_t	i;

	if ((data_len == 0) || (data_len % 2 != 0) || (buffer_len < data_len/2))
	{
		return	0;	
	}

	for(i = 0 ; i < data_len / 2; i++)
	{
		if ('0' <= data[i*2] && data[i*2] <= '9')
		{
			buffer[i] = (data[i*2] - '0') << 4;
		}
		else
		{
			buffer[i] = (data[i*2] - 'A' + 10) << 4;
		}
	

		if ('0' <= data[i*2 + 1] && data[i*2 + 1] <= '9')
		{
			buffer[i] += (data[i*2 + 1] - '0');
		}
		else
		{
			buffer[i] += (data[i*2 + 1] - 'A' + 10);
		}
	}

	return	i;

}
