/**************************************************************************//**
*   @file   i2c.h
*   @brief  I2C header file.
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

#ifndef __I2C_H__
#define __I2C_H__

/*****************************************************************************/
/***************************** Include Files *********************************/

//ADT7420 IIC Address (JP1 - OPEN, JP2 - OPEN)
#define ADT7420_IIC_ADDR 	0x4B


/*****************************************************************************/
/******************* I2C Registers Definitions *******************************/
/*****************************************************************************/
#define IIC_GIE             0x8013001C
#define IIC_ISR       	    0x80130020
#define IIC_IER       	    0x80130028
#define IIC_SOFTR      	    0x80130040
#define IIC_CR      	    0x80130100
#define IIC_SR      	    0x80130104
#define IIC_TX_FIFO  	    0x80130108
#define IIC_RX_FIFO  	    0x8013010C
#define IIC_ADR       	    0x80130110
#define IIC_TX_FIFO_OCY	    0x80130114
#define IIC_RX_FIFO_OCY	    0x80130118
#define IIC_TEN_ADDR        0x8013011C
#define IIC_RX_FIFO_PIRQ    0x80130120
#define IIC_GPO			    0x80130124

/*****************************************************************************/
/************************ Functions Declarations *****************************/
/*****************************************************************************/
unsigned int I2C_Init();
unsigned int I2C_Read(unsigned int regAddr, unsigned int rxSize, unsigned char* rxBuf);
void I2C_Write(unsigned int regAddr, unsigned int txSize, unsigned char* txBuf);

#endif /* __I2C_H__ */
