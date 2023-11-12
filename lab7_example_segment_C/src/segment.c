#if defined(D_NEXYS_A7)
   #include <bsp_printf.h>
   #include <bsp_mem_map.h>
   #include <bsp_version.h>
#else
   PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>

#define IO_LEDR     0x80100000
#define IO_SWs		0x80100008
#define IO_INOUT    0x8010000C
#define SEG_BASE    0x80130000
#define PWM_BASE    0x80120000

#define READ_IO(dir) (*(volatile unsigned *)dir)
#define WRITE_IO(dir, value) { (*(volatile unsigned *)dir) = (value); }

#define M_UART_RD_REG_LSR()  (*((volatile unsigned int*)(D_UART_BASE_ADDRESS + (4*0x05) ))) 
#define D_UART_LSR_RBRE_BIT  (0x01)

void delay() 
{
	volatile unsigned int j;
	for (j = 0; j < (1000000); j++) ;	// delay 
}

char uart_inbyte(void) 
{
	unsigned int RecievedByte;

    /* Check for space in UART FIFO */
    while((M_UART_RD_REG_LSR() & D_UART_LSR_RBRE_BIT) == 0);
	
	RecievedByte = READ_IO(D_UART_BASE_ADDRESS);

	return (char)RecievedByte;
}

int main(void)
{
    int i, j = 1;
    int count = 0xF;
    unsigned int value = 0;
    unsigned int period = 0;
    unsigned int seg[8];
    unsigned int segdata = 0;

    /* Initialize Uart */
    uartInit();

    printfNexys("hello world\n");

    while (1)
    {
        printfNexys("Select eight nums to show in segment(0-F)\n");
        for (i = 0; i < 8; i++) {
            seg[i] = uart_inbyte();
        }
        printfNexys("The nums to show is:%c%c%c%c%c%c%c%c", seg[0], seg[1], seg[2], seg[3], seg[4], seg[5], seg[6], seg[7]);
        for (i = 0; i < 8; i++) {
            if (seg[i] <= '9' && seg[i] >= '0')
                seg[i] -= '0';
            else if (seg[i] <= 'f' && seg[i] >= 'a')
                seg[i] -= 'a' - 10;
            else if (seg[i] <= 'F' && seg[i] >= 'A')
                seg[i] -= 'A' - 10;
            else
                seg[i] = 15;
            segdata <<= 4;
            segdata |= seg[i];
        }
        WRITE_IO(SEG_BASE, segdata);
        delay();
   }

   return 0;
}
