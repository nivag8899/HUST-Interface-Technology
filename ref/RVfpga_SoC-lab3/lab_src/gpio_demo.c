
#define IO_LEDR     0x80100000

#define READ_GPIO(dir) (*(volatile unsigned *)dir)
#define WRITE_GPIO(dir, value) { (*(volatile unsigned *)dir) = (value); }

int main ( void )
{
    int i, j=1, count=0xF, delay=10000000;

    while (1) { 
        WRITE_GPIO(IO_LEDR, count);

        if(j == 1) {
		    count = count << 1;
		    if(count == 0xf000)
		        j = 0;
		} else {
				count = count >> 1;
		    if(count == 0x000f)
		        j = 1;
		}

		for (i=0; i<delay; i++);
    }

    return(0);
}
