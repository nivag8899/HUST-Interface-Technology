/**************************************************************************//**
*   @file   i2c.c
*   @brief  I2C functions implementation.
*   @author acozma (andrei.cozma@analog.com)
*
*******************************************************************************
* Copyright 2011(c) Analog Devices, Inc.
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
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
* THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT, MERCHANTABILITY
* AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************
*   SVN Revision: 468
******************************************************************************/

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

#define DELAY_IIC	10000

extern void delay(int num);

/**************************************************************************//**
* @brief Initializes the communication with the Microblaze I2C peripheral.
*
* @param axiBaseAddr - Microblaze I2C peripheral AXI base address.
* @param i2cAddr - The address of the I2C slave.
* @return TRUE.
******************************************************************************/
unsigned int I2C_Init()
{
	//disable the I2C core
	M_PSP_WRITE_REGISTER_32(IIC_CR, 0x00);
	delay(DELAY_IIC);
	
	//set the Rx FIFO depth to maximum
	M_PSP_WRITE_REGISTER_32(IIC_RX_FIFO_PIRQ, 0x0F);
	delay(DELAY_IIC);
	
	//reset the I2C core and flush the Tx fifo
	M_PSP_WRITE_REGISTER_32(IIC_CR, 0x02);
	delay(DELAY_IIC);
	
	//enable the I2C core
	M_PSP_WRITE_REGISTER_32(IIC_CR, 0x01);
	delay(DELAY_IIC);

	return 1;
}

/**************************************************************************//**
* @brief Reads data from an I2C slave.
*
* @param axiBaseAddr - Microblaze I2C peripheral AXI base address.
* @param i2cAddr - The address of the I2C slave.
* @param regAddr - Address of the I2C register to be read. 
*				   Must be set to -1 if it is not used.
* @param rxSize - Number of bytes to read from the slave.
* @param rxBuf - Buffer to store the read data.
* @return Returns the number of bytes read.
******************************************************************************/
unsigned int I2C_Read(unsigned int regAddr, unsigned int rxSize, unsigned char* rxBuf)
{
	unsigned int rxCnt   = 0;
	unsigned int timeout = 0x1FFFFFF;
	volatile int temp;

	// Reset tx fifo
	M_PSP_WRITE_REGISTER_32(IIC_CR, 0x02);
	delay(DELAY_IIC);

	// Enable iic
	M_PSP_WRITE_REGISTER_32(IIC_CR, 0x01);
	delay(DELAY_IIC);

	if(regAddr != -1)
	{
		// Set the slave I2C address
		M_PSP_WRITE_REGISTER_32(IIC_TX_FIFO, 0x100 | (ADT7420_IIC_ADDR << 1));
		delay(DELAY_IIC);
		
		// Set the slave register address
		M_PSP_WRITE_REGISTER_32(IIC_TX_FIFO, regAddr);
		delay(DELAY_IIC);
	}

	// Set the slave I2C address
	M_PSP_WRITE_REGISTER_32(IIC_TX_FIFO, 0x101 | (ADT7420_IIC_ADDR << 1));
	delay(DELAY_IIC);
	
	// Start a read transaction
	M_PSP_WRITE_REGISTER_32(IIC_TX_FIFO, 0x200 + rxSize);
	delay(DELAY_IIC);

	// Read data from the I2C slave
	while(rxCnt < rxSize)
	{
		//wait for data to be available in the RxFifo
		//temp = M_PSP_READ_REGISTER_32(axiBaseAddr + SR);
		while((M_PSP_READ_REGISTER_32(IIC_SR) & 0x00000040) && (timeout--));

		if(timeout == -1)
		{
			//printfNexys("The I2C timeout!\n");
			//disable the I2C core
			M_PSP_WRITE_REGISTER_32(IIC_CR, 0x00);
			delay(DELAY_IIC);

			//set the Rx FIFO depth to maximum
			M_PSP_WRITE_REGISTER_32(IIC_RX_FIFO_PIRQ, 0x0F);
			delay(DELAY_IIC);

			//reset the I2C core and flush the Tx fifo
			M_PSP_WRITE_REGISTER_32(IIC_CR, 0x02);
			delay(DELAY_IIC);

			//enable the I2C core
			M_PSP_WRITE_REGISTER_32(IIC_CR, 0x01);
			delay(DELAY_IIC);

			return rxCnt;
		}
		timeout = 0x1FFFFFF;

		//read the data
		rxBuf[rxCnt] = M_PSP_READ_REGISTER_32(IIC_RX_FIFO) & 0xFFFF;

		//increment the receive counter
		rxCnt++;
	}

	return rxCnt;
}

/**************************************************************************//**
* @brief Writes data to an I2C slave.
*
* @param axiBaseAddr - Microblaze I2C peripheral AXI base address.
* @param i2cAddr - The address of the I2C slave.
* @param regAddr - Address of the I2C register to be read. 
*				   Must be set to -1 if it is not used.
* @param txSize - Number of bytes to write to the slave.
* @param txBuf - Buffer to store the data to be transmitted.
* @return Returns the number of bytes written.
******************************************************************************/
void I2C_Write(unsigned int regAddr, unsigned int txSize, unsigned char* txBuf)
{
	unsigned int txCnt = 0;
	unsigned int txTemp;

	// Reset tx fifo
	M_PSP_WRITE_REGISTER_32(IIC_CR, 0x02);
	delay(DELAY_IIC);
	
	// enable iic
	M_PSP_WRITE_REGISTER_32(IIC_CR, 0x01);
	delay(DELAY_IIC);

	// Set the I2C address
	M_PSP_WRITE_REGISTER_32(IIC_TX_FIFO, 0x100 | (ADT7420_IIC_ADDR << 1));
	delay(DELAY_IIC);

	if(regAddr != -1)
	{
		// Set the slave register address
		M_PSP_WRITE_REGISTER_32(IIC_TX_FIFO, regAddr);
		delay(DELAY_IIC);
	}

	// Write data to the I2C slave
	while(txCnt < txSize)
	{
		// put the Tx data into the Tx FIFO
		txTemp = (txCnt == txSize - 1) ? (0x200 | txBuf[txCnt]) : txBuf[txCnt];
		M_PSP_WRITE_REGISTER_32(IIC_TX_FIFO, txTemp);
		delay(DELAY_IIC);
		txCnt++;
	}

	//return txCnt;
}
