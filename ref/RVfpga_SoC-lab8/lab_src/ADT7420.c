/***************************************************************************//**
*   @file   ADT7420.c
*   @brief  ADT7420 Driver Files for MicroBlaze Processor.
*   @author ATofan (alexandru.tofan@analog.com)
********************************************************************************
* Copyright 2012(c) Analog Devices, Inc.
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*  - Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*  - Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*  - Neither the name of Analog Devices, Inc. nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*  - The use of this software may or may not infringe the patent rights
*    of one or more patent holders.  This license does not release you
*    from the requirement that you obtain separate licenses from these
*    patent holders to use this software.
*  - Use of the software either in source or binary form, must be run
*    on or directly connected to an Analog Devices Inc. component.
*
* THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
********************************************************************************
*   SVN Revision: $WCREV$
*******************************************************************************/

#if defined(D_NEXYS_A7)
   #include <bsp_printf.h>
   #include <bsp_mem_map.h>
   #include <bsp_version.h>
#else
   PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>

/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/
#include "i2c.h"
#include "ADT7420.h"

#define M_UART_RD_REG_LSR()  (*((volatile unsigned int*)(D_UART_BASE_ADDRESS + (4*0x05) )))              /* Line status reg.    */

#define M_UART_RD_CH() (*((volatile unsigned int*)(D_UART_BASE_ADDRESS + (0x00) )))

extern void delay(int num);

char UartGetchar()
{
	char ch;
	//while(!((M_UART_RD_REG_LSR() & 0x1) == 0x1));
	while((M_UART_RD_REG_LSR() & 0x1) == 0);
	// read char
	ch = M_UART_RD_CH();
	return ch;
}

/*****************************************************************************/
/********************** Variable Definitions *********************************/
/*****************************************************************************/
extern volatile int rxData;
char valid  = 0;
int  TUpper = 0x1FFF;

/******************************************************************************
* @brief Performs Software Reset for ADT7420 and sets Alert Mode as Comparator.
*
* @param  None.
*
* @return None.
******************************************************************************/
void ADT7420_Init(void)
{
	unsigned char txBuffer[1] = { 0x00 };
	
	if(I2C_Init())
    {
        printfNexys("AXI IIC initialized OK!");
    }
    else
    {
        printfNexys("AXI IIC Error!");
    }
	
	I2C_Write(SOFT_RST_REG, 1, txBuffer);

	SetAlertModeComparator();
}
/******************************************************************************
* @brief Sets Alert Mode as Comparator.
*
* @param None.
*
* @return None.
******************************************************************************/
void SetAlertModeComparator(void)
{
	unsigned char txBuffer[1] = {0x00};

	txBuffer[0] = 1 << INT_CT;

	I2C_Write(CONFIG_REG, 1, txBuffer);
}

/******************************************************************************
* @brief Returns value from Configuration Register.
*
* @param None.
*
* @return rxBuffer[0] & 0x7F - all bits in the Configuration Register, except RESOLUTION bit
******************************************************************************/
char ADT7420_ReadConfigReg(void)
{
	unsigned char rxBuffer[1] = {0x00};

	I2C_Read(CONFIG_REG, 1, rxBuffer);

	return(rxBuffer[0] & 0x7F);
}

/******************************************************************************
* @brief Displays Revision ID and Manufacturer ID.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintID(void)
{
	unsigned char rxBuffer[1] = {0x00};

	I2C_Read(ID_REG, 1, rxBuffer);

	printfNexys("Revision ID = %d", (rxBuffer[0] & REVISION_ID));
	printfNexys("Manufacture ID = %d", ((rxBuffer[0] & MANUFACTURE_ID)>>3));
	printfNexys("-----------------------------------------");
}

/******************************************************************************
* @brief Displays Main Menu.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_DisplayMainMenu(void)
{
	printfNexys("ADT7420 Demo Program");

	ADT7420_PrintID();

	ADT7420_DisplayMenu();

	printfNexys("Press key to select desired option");
	printfNexys("Press [q] to exit the application");
}

/******************************************************************************
* @brief Displays Menu.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_DisplayMenu(void)
{
	printfNexys("Available options:");
	printfNexys("	[t] Read Temperature");
	printfNexys("	[r] Set Resolution");
	printfNexys("	[h] Set THigh");
	printfNexys("	[l] Set TLow");
	printfNexys("	[c] Set TCrit");
	printfNexys("	[y] Set THyst");
	printfNexys("	[f] Set Fault Queue");
	printfNexys("	[s] Display Settings");
	printfNexys("	[m] Stop the program and display this menu");

	rxData = 0;
}

/******************************************************************************
* @brief Returns resolution of ADT7420 internal ADC.
*
* @param display - 0 -> resolution is displayed on UART
*				 - 1 -> resolution is not displayed on UART.
*
* @return (rxBuffer[0] & (1 << RESOLUTION)) - bit 7 of CONFIGURATION REGISTER
* 				 - 0 -> resolution is 13 bits
* 				 - 1 -> resolution is 16 bits.
******************************************************************************/
unsigned char ADT7420_GetResolution(char display)
{
	unsigned char rxBuffer[2] = {0x00, 0x00};
	I2C_Read(CONFIG_REG, 1, rxBuffer);

	if(display == 1)
	{
		if((rxBuffer[0] & (1 << RESOLUTION)) == 0)
		{
			printfNexys("Resolution is 13 bits (0.0625 C/LSB)");
		}
		else
		{
			printfNexys("Resolution is 16 bits (0.0078 C/LSB)");
		}
	}

	return (rxBuffer[0] & (1 << RESOLUTION));
}

/******************************************************************************
* @brief Displays Set Resolution Menu.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_DisplayResolutionMenu(void)
{
	printfNexys("Set Resolution Menu");
	printfNexys("-----------------------------------------");
	printfNexys("Possible resolutions are:");
	printfNexys("	1. 13 bits (0.0625 C/LSB)");
	printfNexys("	2. 16 bits (0.0078 C/LSB)");
}

/******************************************************************************
* @brief Sets and displays resolution for ADC of ADT7420
*
* @param None
*
* @return None
******************************************************************************/
void ADT7420_SetResolution(void)
{
	unsigned char txBuffer[1] = { 0x00 };
	char          rx          = 0;

	ADT7420_DisplayResolutionMenu();

	// Check if data is available on the UART
	// Store and display received data
	rx = UartGetchar();

	switch (rx)
		{
		case '1' :
			txBuffer[0] = (0 << RESOLUTION) | ADT7420_ReadConfigReg() ; // so as not to change other configuration parameters
			printfNexys("Resolution is 13 bits (0.0625 C/LSB)");
			TUpper = 0x1FFF;
			rxData = 'm';
			printfNexys("Returning to Main Menu...");
			break;
		case '2' :
			txBuffer[0] = (1 << RESOLUTION) | ADT7420_ReadConfigReg();
			printfNexys("Resolution is 16 bits (0.0078 C/LSB)");
			TUpper = 0xFFFF;
			rxData = 'm';
			printfNexys("Returning to Main Menu...");
			break;
		default:
			printfNexys("Wrong option!");
			break;
		}

	I2C_Write(CONFIG_REG, 1, txBuffer);
}

/******************************************************************************
* @brief Reads data from the Temperature MSB and LSB registers of ADT7420.
*
* @param None.
*
* @return data - value read from the Temperature MSB and LSB registers.
******************************************************************************/
int ADT7420_ReadTemp(void)
{
	unsigned char rxBuffer[2]  = {0x00,0x00};
	int           data         = 0;

	printfNexys("Please wait the ADT7420 to get the temperature ... \n");
	//delay(100000000);

	I2C_Read(TEMP_REG, 2, rxBuffer);

	if(ADT7420_GetResolution(0) == 0)
	{
		data = (rxBuffer[0] << 5) | (rxBuffer[1] >> 3);
	}
	else
	{
		data = (rxBuffer[0] << 8) | (rxBuffer[1]);
	}

	//printfNexys("The temperature read from ADT7420 is %d", data);

	return (data);
}

/******************************************************************************
* @brief Displays temperature data in degrees Celsius.
*
* @param data - value read from the Temperature MSB and LSB registers.
*
* @return None.
******************************************************************************/
void Display_Temp(short int data)
{
	int     value     = 0;

	// converting data for display
	if(ADT7420_GetResolution(0) == 0)
	{
		if(data&0x1000)
		{
			data = data	| 0xffffe000;
		}
		value = data / 16;
	}
	else
	{
		value = data / 128;
	}

	if(value >= 0)
	{
	    printfNexys("T = %d .00 ℃", value);
	}
	else
	{
		value = value * (-1);
		printfNexys("T = - %d .00 ℃", value);
	}
}

/******************************************************************************
* @brief Reads data from the UART console.
*
* @param None.
*
* @return value -> data converted to hex value
* 			0 	-> characters read are not hex values.
******************************************************************************/
int ADT7420_ConsoleRead(void)
{
	char rx    = 0;
	char c[4]  = "0000";
	char *c_ptr;
	int  i     = 0;
	char cnt   = 0;
	int  value = 0;

	valid = 0;
	c_ptr = c;

	while(i < 6)
	{
		// Check if data is available on the UART
		// Store and display received data
		rx = UartGetchar();
		printfNexys("rx = %c", rx);

		// Check if pressed key is [Enter]
		if(rx == 0x0D)
		{
			i = 5;
		}
		else if(rx == 0x0A)
		{
			i = 5;
		}
		else if(((rx > 0x00)&&(rx < 0x30))|| // Not 0 - 9
				((rx > 0x39)&&(rx < 0x41))|| // Not A - F
				((rx > 0x46)&&(rx < 0x61))|| // Not a - f
				(rx > 0x66))
		{
			printfNexys("Characters entered must be HEX values (0 to 9 and A B C D E F)");
			i = 6;
			valid = 0;
		}
		else
		{
			*c_ptr++ = rx;
			cnt = cnt + 1;
			valid = 1;
		}
		if(cnt == 4)
		{
			i = 6;
		}
		i++;
	}

	// Translate from ASCII to hex
	for(i = 0; i < cnt; i++)
	{
		if(c[i] > 0x60)
		{
			value = value * 16 + (c[i] - 0x57);
		}
		else if(c[i] > 0x39)
		{
			value = value * 16 + (c[i] - 0x37);
		}
		else
		{
			value = value * 16 + (c[i] - 0x30);
		}
	}

	if(valid == 1)
	{
		return value;
	}
	else
	{
		return 0;
	}
}

/******************************************************************************
* @brief Menu for setting data in THigh register.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_DisplaySetTHighMenu(void)
{
	int THigh = 0;

	printfNexys("Set THigh Menu");
	printfNexys("-----------------------------------------");
	printfNexys("Please enter a value between 0x0000 and 0x003C");

	THigh = ADT7420_ConsoleRead();

	while(!((THigh>=0x0000)&(THigh<=0x003C)))
	{
		printfNexys("Value for THigh must be in the range 0x0000 and 0x003C");
		printfNexys("Please enter a valid value: 0x");
		THigh = ADT7420_ConsoleRead();
	}

	if(valid == 1)
	{
		ADT7420_SetTHigh(THigh);
		rxData = 'm';
		printfNexys("Returning to Main Menu...");
	}
}

/******************************************************************************
* @brief Sets value of THigh register.
*
* @param THigh - value to be placed in the register.
*
* @return None.
******************************************************************************/
void ADT7420_SetTHigh(int THigh)
{
	unsigned char txBuffer[2] = {0x00, 0x00};

	if(ADT7420_GetResolution(0) == 0)
	{
		txBuffer[0] = (THigh & 0x1FE0) >> 5;
		txBuffer[1] = (THigh & 0x001F) << 3;
	}
	else
	{
		txBuffer[0] = (THigh & 0xFF00) >> 8;
		txBuffer[1] = THigh & 0x00FF;
	}

	I2C_Write(TH_SETP_MSB, 2, txBuffer);
}

/******************************************************************************
* @brief Displays value of THigh setpoint.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintTHigh(void)
{
	unsigned char rxBuffer[2] = {0x00, 0x00};
	int           val         = 0;

	I2C_Read(TH_SETP_MSB, 2, rxBuffer);

	if(ADT7420_GetResolution(0) == 0)
		val = ( rxBuffer[0] << 5 ) | ( rxBuffer[1] >> 3);
	else
		val = (rxBuffer[0] << 8) | rxBuffer[1];

	printfNexys("THigh Setpoint ");
	Display_Temp(val);
}

/******************************************************************************
* @brief Menu for setting data in TLow register.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_DisplaySetTLowMenu(void)
{
	int TLow = 0;

	printfNexys("Set TLow Menu");
	printfNexys("-----------------------------------------");
	printfNexys("Please enter a value between 0x0000 and 0x000A");
	
	TLow = ADT7420_ConsoleRead();

	while(!((TLow>=0x0000)&(TLow<=0x000A)))
	{
		printfNexys("Value for TLow must be in the range 0x0000 and 0x000A");
		printfNexys("Please enter a valid value: 0x");
		TLow = ADT7420_ConsoleRead();
	}

	if(valid == 1)
	{
		ADT7420_SetTLow(TLow);
		rxData = 'm';
		printfNexys("Returning to Main Menu...");
	}
}

/******************************************************************************
* @brief Sets value of  TLow register.
*
* @param TLow - value to be placed in the register.
*
* @return None.
******************************************************************************/
void ADT7420_SetTLow(int TLow)
{
	unsigned char txBuffer[2] = {0x00, 0x00};

	if(ADT7420_GetResolution(0) == 0)
	{
		txBuffer[0] = (TLow & 0x1FE0) >> 5;
		txBuffer[1] = (TLow & 0x001F) << 3;
	}
	else
	{
		txBuffer[0] = (TLow & 0xFF00) >> 8;
		txBuffer[1] = TLow & 0x00FF;
	}

	I2C_Write(TL_SETP_MSB, 2, txBuffer);
}

/******************************************************************************
* @brief Displays value of TLow setpoint.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintTLow(void)
{
	unsigned char rxBuffer[2] = {0x00, 0x00};
	int           val         = 0;

	I2C_Read(TL_SETP_MSB, 2, rxBuffer);

	if(ADT7420_GetResolution(0) == 0)
	{
		val = ( rxBuffer[0] << 5 ) | ( rxBuffer[1] >> 3);
	}
	else
	{
		val = (rxBuffer[0] << 8) | rxBuffer[1];
	}

	printfNexys("TLow Setpoint ");
	Display_Temp(val);
}

/******************************************************************************
* @brief Sets value of  TCrit register.
*
* @param TCrit - value to be placed in the register.
*
* @return None.
******************************************************************************/
void ADT7420_SetTCrit(int TCrit)
{
	unsigned char txBuffer[2] = {0x00, 0x00};
	if(ADT7420_GetResolution(0) == 0)
	{
		txBuffer[0] = (TCrit & 0x1FE0) >> 5;
		txBuffer[1] = (TCrit & 0x001F) << 3;
	}
	else
	{
		txBuffer[0] = (TCrit & 0xFF00) >> 8;
		txBuffer[1] = TCrit & 0x00FF;
	}

	I2C_Write(TCRIT_SETP_MSB, 2, txBuffer);
}

/******************************************************************************
* @brief Menu for setting data in TCrit register.
*
* @param None.
*
* @return None.
******************************************************************************/

void ADT7420_DisplaySetTCritMenu(void)
{
	int TCrit = 0;

	printfNexys("Set TCrit Menu");
	printfNexys("-----------------------------------------");
	printfNexys("Please enter a value between 0x0000 and 0x0064");
	
	TCrit = ADT7420_ConsoleRead();

	while(!((TCrit>=0x0000)&(TCrit<=0x0064)))
	{
		printfNexys("Value for TCrit must be in the range 0x0000 and 0x0064");
		printfNexys("Please enter a valid value: 0x");
		TCrit = ADT7420_ConsoleRead();
	}

	if(valid == 1)
	{
		ADT7420_SetTCrit(TCrit);
		rxData = 'm';
		printfNexys("Returning to Main Menu...");
	}
}

/******************************************************************************
* @brief Displays value of TCrit setpoint.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintTCrit(void)
{
	unsigned char rxBuffer[2] = {0x00, 0x00};
	int           val         = 0;

	I2C_Read(TCRIT_SETP_MSB, 2, rxBuffer);

	if(ADT7420_GetResolution(0) == 0)
	{
		val = ( rxBuffer[0] << 5 ) | ( rxBuffer[1] >> 3);
	}
	else
	{
		val = (rxBuffer[0] << 8) | rxBuffer[1];
	}

	printfNexys("TCrit Setpoint ");
	Display_Temp(val);
}

/******************************************************************************
* @brief Menu for setting data in THyst register.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_DisplaySetTHystMenu(void)
{
	int THyst = 0;

	printfNexys("Set THyst Menu");
	printfNexys("-----------------------------------------");
	printfNexys("Enter a value from 0x0000 to 0x000F: 0x");

	THyst = ADT7420_ConsoleRead();

	while(!((THyst>=0)&(THyst<16)))
	{
		printfNexys("Value for THyst must be in the range 0 to 15");
		printfNexys("Please enter a valid value: 0x");
		THyst = ADT7420_ConsoleRead();
	}

	if(valid == 1)
	{
		ADT7420_SetHysteresis(THyst);
		rxData = 'm';
		printfNexys("Returning to Main Menu...");
	}
}

/******************************************************************************
* @brief Sets value of THyst register.
*
* @param THyst - value from 0x0000 to 0x000F to be placed in the register.
*
* @return None.
******************************************************************************/
void ADT7420_SetHysteresis(int THyst)
{
	unsigned char txBuffer[1] = {0x00};
	txBuffer[0] = THyst & 0x0F;
	I2C_Write(T_HYST_SETP, 1, txBuffer);
}

/******************************************************************************
* @brief Displays value of THyst.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintHysteresis(void)
{
	unsigned char rxBuffer[2] = { 0x00 };
	I2C_Read(T_HYST_SETP, 1, rxBuffer);
	printfNexys("THyst Setpoint T = %d ℃", rxBuffer[0]);
}

/******************************************************************************
* @brief Menu for setting Fault Queue.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_DisplaySetFaultQueueMenu(void)
{
	unsigned char txBuffer[1] = { 0x00 };
	char          rx          = 0;

	printfNexys("Fault Queue Menu");
	printfNexys("-----------------------------------------");
	printfNexys("Number of fault queues:");
	printfNexys("	1. 1 fault");
	printfNexys("	2. 2 faults");
	printfNexys("	3. 3 faults");
	printfNexys("	4. 4 faults");

	// Check if data is available on the UART
	// Store and display received data
	rx = UartGetchar( );

	switch (rx)
	{
	case '1' :
			txBuffer[0] = 0x00 << FAULT_QUEUE;
			printfNexys("1 fault queue");
			rxData = 'm';
			printfNexys("Returning to Main Menu...");
			break;
		case '2' :
			txBuffer[0] = 0x01 << FAULT_QUEUE;
			printfNexys("2 fault queue");
			rxData = 'm';
			printfNexys("Returning to Main Menu...");
			break;
		case '3' :
			txBuffer[0] = 0x02 << FAULT_QUEUE;
			printfNexys("3 fault queue");
			rxData = 'm';
			printfNexys("Returning to Main Menu...");
			break;
		case '4' :
			txBuffer[0] = 0x03 << FAULT_QUEUE;
			printfNexys("4 fault queue");
			rxData = 'm';
			printfNexys("Returning to Main Menu...");
			break;
		default:
			printfNexys("Wrong option!");
			break;
	}
	I2C_Write(CONFIG_REG, 1, txBuffer);
}

/******************************************************************************
* @brief Displays value of Fault Queues.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintFaultQueue(void)
{
	unsigned char rxBuffer[1] = { 0x00 };
	char          rx          = 0;

	I2C_Read(CONFIG_REG, 1, rxBuffer);

	rx = rxBuffer[0] & (0x03 << FAULT_QUEUE);

	switch (rx)
	{
		case 0x00 :
			printfNexys("1 fault queue");
			break;
		case 0x01 :
			printfNexys("2 fault queue");
			break;
		case 0x02 :
			printfNexys("3 fault queue");
			break;
		case 0x03 :
			printfNexys("4 fault queue");
			break;
		default:
			break;
	}
}

/******************************************************************************
* @brief Displays Alert Mode setting.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintAlertMode(void)
{
	unsigned char rxBuffer[2] = { 0x00 };
	I2C_Read(CONFIG_REG, 1, rxBuffer);

	if (rxBuffer[0] & (1 << INT_CT))
	{
		printfNexys("Alert Mode: Comparator");
	}
	else
	{
		printfNexys("Alert Mode: Interrupt");
	}
}

/******************************************************************************
* @brief Displays output polarity setting for CT pin.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintCTPolarity(void)
{
	unsigned char rxBuffer[1] = { 0x00 };
	I2C_Read(CONFIG_REG, 1, rxBuffer);

	if (rxBuffer[0] & (1 << CT_POL))
	{
		printfNexys("CT pin is Active High");
	}
	else
	{
		printfNexys("CT pin is Active Low");
	}
}


/******************************************************************************
* @brief Displays output polarity setting for INT pin.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_PrintINTPolarity(void)
{
	unsigned char rxBuffer[1] = { 0x00 };
	I2C_Read(CONFIG_REG, 1, rxBuffer);

	if (rxBuffer[0] & (1 << INT_POL))
	{
		printfNexys("INT pin is Active High");
	}
	else
	{
		printfNexys("INT pin is Active Low");
	}
}

/******************************************************************************
* @brief Displays current settings of the ADT7420.
*
* @param None.
*
* @return None.
******************************************************************************/
void ADT7420_DisplaySettings(void)
{
	printfNexys("ADT7420 Settings ");
	printfNexys("-----------------------------------------");
	ADT7420_GetResolution(1);

	ADT7420_PrintTHigh();
	ADT7420_PrintTLow();
	ADT7420_PrintTCrit();
	ADT7420_PrintHysteresis();

	ADT7420_PrintFaultQueue();

	ADT7420_PrintAlertMode();
	ADT7420_PrintCTPolarity();
	ADT7420_PrintINTPolarity();

	rxData = 'm';
	printfNexys("Returning to Main Menu...");
}
