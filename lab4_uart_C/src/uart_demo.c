#if defined(D_NEXYS_A7)
#include <bsp_printf.h>
#include <bsp_mem_map.h>
#include <bsp_version.h>
#else
PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>
#define UART_BASE 0x80002000
#define rbr 0*4
#define ier 1*4
#define fcr 2*4
#define lcr 3*4
#define mcr 4*4
#define lsr 5*4
#define msr 6*4
#define scr 7*4

#define thr rbr
#define iir fcr
#define dll rbr
#define dlm ier

#define READ_GPIO(dir) (*(volatile unsigned *)dir)
#define WRITE_GPIO(dir, value)                 \
    {                                          \
        (*(volatile unsigned *)dir) = (value); \
    }
void delay(){
   for(volatile unsigned int i=1;i<1000000;i++);
   return;
}


int main(void)
{      
    //volatile unsigned int pushbotton,count = 0xF;
    //volatile unsigned int j=1;
    //uartInit();
    char str[20]="hello world\n";
    int i=0;
    WRITE_GPIO((UART_BASE + lcr), 0x00000080);
    delay();
    WRITE_GPIO((UART_BASE + dll), 27);
    delay();
    WRITE_GPIO((UART_BASE + dlm), 0);
    delay();
    WRITE_GPIO((UART_BASE + lcr), 0x00000003);
    delay();
    WRITE_GPIO((UART_BASE + ier), 0x00000000);
    delay();
    while (1)
    {   while(str[i]!='\0')
        WRITE_GPIO((UART_BASE + thr), str[i++]);
        i=0;
        delay();
    }
}

