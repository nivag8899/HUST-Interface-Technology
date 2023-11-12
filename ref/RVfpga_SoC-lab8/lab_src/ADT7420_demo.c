#if defined(D_NEXYS_A7)
   #include <bsp_printf.h>
   #include <bsp_mem_map.h>
   #include <bsp_version.h>
#else
   PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>

#include <i2c.h>
#include <ADT7420.h>

#define SegEn_ADDR      0x80001038
#define SegDig_ADDR     0x8000103C

#define DELAY           100000    

extern char UartGetchar();

int rxData = 0;

void delay(int num)
{
    volatile int i;
    for(i=0; i<num; i++);
}

int main(void)
{
    int displayData = 0;

    // Initialize 7-Seg Display
    M_PSP_WRITE_REGISTER_32(SegEn_ADDR, 0x0);
    M_PSP_WRITE_REGISTER_32(SegDig_ADDR, displayData);

    // Initialize Uart
    uartInit();

    // Begin ADT7420 test
	// Initialize ADT7420 Device
	ADT7420_Init();
    
	// Display Main Menu on UART
	ADT7420_DisplayMainMenu();

	rxData = UartGetchar();
    //printfNexys("rxData = %c", rxData);

	while(rxData != 'q')
    {
		switch(rxData)
    	{
    	case 't':
    		displayData = ADT7420_ReadTemp();
            M_PSP_WRITE_REGISTER_32(SegDig_ADDR, displayData);
		    Display_Temp(displayData);
    		break;
    	case 'r':
    		ADT7420_SetResolution();
    		break;
    	case 'h':
    		ADT7420_DisplaySetTHighMenu();
    		break;
    	case 'l':
    		ADT7420_DisplaySetTLowMenu();
    		break;
    	case 'c':
    		ADT7420_DisplaySetTCritMenu();
    		break;
        case 'y':
        	ADT7420_DisplaySetTHystMenu();
    		break;
    	case 'f':
    		ADT7420_DisplaySetFaultQueueMenu();
    		break;
    	case 's':
			ADT7420_DisplaySettings();
			break;
    	case 'm':
    		ADT7420_DisplayMenu();
    		break;
    	case 0:
    		break;
    	default:
            displayData = ADT7420_ReadTemp();
            M_PSP_WRITE_REGISTER_32(SegDig_ADDR, displayData);
		    Display_Temp(displayData);
            printfNexys("-----------------------------------------");
    		printfNexys("Wrong option! Please select one of the options below.");
    		break;
    	}

		ADT7420_DisplayMenu();
		rxData = UartGetchar();
        //printfNexys("rxData = %c", rxData);
        delay(DELAY);
    }
	
	printfNexys("Exiting ADT7420 test application!");

    return 0;
}
