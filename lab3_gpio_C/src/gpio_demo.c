#define IO_LEDR 0x80100000
#define IO_SWs 0x80100008

#define READ_GPIO(dir) (*(volatile unsigned *)dir)
#define WRITE_GPIO(dir, value)                 \
	{                                          \
		(*(volatile unsigned *)dir) = (value); \
	}

int main(void)
{
	int i= 1, delay = 30000000;
	int En_Value=0x0000,switches_value;
	WRITE_GPIO(IO_LEDR,En_Value);
	while (1)
	{
		switches_value=READ_GPIO(IO_SWs);
		WRITE_GPIO(IO_LEDR, switches_value);
		/*
		if (j == 1)
		{
			count = count << 1;
			if (count == 0xf000)
				j = 0;
		}
		else
		{
			count = count >> 1;
			if (count == 0x000f)
				j = 1;
		}
		*/
		for (i = 0; i < delay; i++);
	}
	
	return (0);
}