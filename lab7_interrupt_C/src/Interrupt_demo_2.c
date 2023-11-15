#if defined(D_NEXYS_A7)
   #include <bsp_printf.h>
   #include <bsp_mem_map.h>
   #include <bsp_version.h>
#else
   PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>
#include "bsp_external_interrupts.h"
#include "psp_ext_interrupts_eh1.h"
#include "bsp_timer.h"

#define PWM_BASE    0x80120000

#define SegEn_ADDR      0x80001038
#define SegDig_ADDR     0x8000103C

#define UART_ISR        0x80120000

#define RPTC_CNTR       0x80001200
#define RPTC_HRC        0x80001204
#define RPTC_LRC        0x80001208
#define RPTC_CTRL       0x8000120c

#define Select_INT      0x80001018

#define READ_IO(dir) (*(volatile unsigned *)dir)
#define WRITE_IO(dir, value) { (*(volatile unsigned *)dir) = (value); }

#define M_UART_RD_REG_LSR()  (*((volatile unsigned int*)(D_UART_BASE_ADDRESS + (4*0x05) ))) 
#define D_UART_LSR_RBRE_BIT  (0x01)

int SegDisplCount=0;
unsigned int value = 0;
unsigned int period = 0;

extern D_PSP_DATA_SECTION D_PSP_ALIGNED(1024) pspInterruptHandler_t G_Ext_Interrupt_Handlers[8];

void delay() 
{
	volatile unsigned int j;
	for (j = 0; j < (1000000); j++) ;	// delay 
}

char uart_inbyte(void) 
{
	unsigned int RecievedByte;

    /* Check for space in UART FIFO */
  //while((M_UART_RD_REG_LSR() & D_UART_LSR_RBRE_BIT) == 0);
	
	RecievedByte = READ_IO(D_UART_BASE_ADDRESS);
  //printfNexys("Brightness Level selected is: %c\n", RecievedByte);
  if(RecievedByte<'0'||RecievedByte>'9')
    return '0';

  return (char)RecievedByte;
}

void uart_ISR(void)
{

    // PWM IP test
	/* Prompt the user to select a brightness value ranging from  0 to 9. */
	printfNexys("Select a Brightness between 0 and 9\n");

    /* Read an input value from the console. */
  value = uart_inbyte();

    /* Convert the input ASCII character to an integer value. */
  period = (value - 0x30) * 110000;

    /* Print the input value back to the console to provide some feedback to the user. */
  printfNexys("Brightness Level selected is: %c\n", value);

    /* Since the LED width is 1e6 clk cycles, we need to normalize
     * the period to that clk.  Since we accept values 0-9, that will
     * scale period from 0-999,000.  0 turns off LEDs, 999,000 is full brightness. */

    /* Write the duty_cycle width (Period) out to the PL PWM peripheral. */
	WRITE_IO(PWM_BASE, period);
		
  delay( );
  /* Clear UART interrupt */
  //M_PSP_WRITE_REGISTER_32(UART_ISR, 0x2);

  bspClearExtInterrupt(1);
}

void PTC_ISR(void)
{
  /* Write 7-Seg Displ */
  M_PSP_WRITE_REGISTER_32(SegDig_ADDR, SegDisplCount);
  SegDisplCount++;

  /* Modify IRQ3 priority if SegDisplCount>10 */
  if (SegDisplCount>10) pspExtInterruptSetPriority(3,5);

  /* Clear PTC interrupt */
  M_PSP_WRITE_REGISTER_32(RPTC_CNTR, 0x0);
  M_PSP_WRITE_REGISTER_32(RPTC_CTRL, 0x40);
  M_PSP_WRITE_REGISTER_32(RPTC_CTRL, 0x31);

  /* Stop the generation of the specific external interrupt */
  bspClearExtInterrupt(3);
}

void DefaultInitialization(void)
{
  u32_t uiSourceId;

  /* Register interrupt vector */
  pspInterruptsSetVectorTableAddress(&M_PSP_VECT_TABLE);

  /* Set external-interrupts vector-table address in MEIVT CSR */
  pspExternalInterruptSetVectorTableAddress(G_Ext_Interrupt_Handlers);

  /* Put the Generation-Register in its initial state (no external interrupts are generated) */
  bspInitializeGenerationRegister(D_PSP_EXT_INT_ACTIVE_HIGH);

  for (uiSourceId = D_BSP_FIRST_IRQ_NUM; uiSourceId <= D_BSP_LAST_IRQ_NUM; uiSourceId++)
  {
    /* Make sure the external-interrupt triggers are cleared */
    bspClearExtInterrupt(uiSourceId);
  }

  /* Set Standard priority order */
  pspExtInterruptSetPriorityOrder(D_PSP_EXT_INT_STANDARD_PRIORITY);

  /* Set interrupts threshold to minimal (== all interrupts should be served) */
  pspExtInterruptsSetThreshold(M_PSP_EXT_INT_THRESHOLD_UNMASK_ALL_VALUE);

  /* Set the nesting priority threshold to minimal (== all interrupts should be served) */
  pspExtInterruptsSetNestingPriorityThreshold(M_PSP_EXT_INT_THRESHOLD_UNMASK_ALL_VALUE);
}

void ExternalIntLine_Initialization(u32_t uiSourceId, u32_t priority, pspInterruptHandler_t pTestIsr)
{
  /* Set Gateway Interrupt type (Level) */
  pspExtInterruptSetType(uiSourceId, D_PSP_EXT_INT_LEVEL_TRIG_TYPE);

  /* Set gateway Polarity (Active high) */
  pspExtInterruptSetPolarity(uiSourceId, D_PSP_EXT_INT_ACTIVE_HIGH);

  /* Clear the gateway */
  pspExtInterruptClearPendingInt(uiSourceId);

  /* Set IRQ4 priority */
  pspExtInterruptSetPriority(uiSourceId, priority);
    
  /* Enable IRQ4 interrupts in the PIC */
  pspExternalInterruptEnableNumber(uiSourceId);

  /* Register ISR */
  G_Ext_Interrupt_Handlers[uiSourceId] = pTestIsr;
}

void PTC_Initialization(void)
{
  /* Configure PTC with interrupts */
  M_PSP_WRITE_REGISTER_32(RPTC_LRC, 0x2FFFFFF);
  M_PSP_WRITE_REGISTER_32(RPTC_CNTR, 0x0);
  M_PSP_WRITE_REGISTER_32(RPTC_CTRL, 0x40);
  M_PSP_WRITE_REGISTER_32(RPTC_CTRL, 0x31);
}

int main(void)
{

    DefaultInitialization();                            /* Default initialization */
	  pspExtInterruptsSetThreshold(5);                    /* Set interrupts threshold to 5 */

    ExternalIntLine_Initialization(1, 6, uart_ISR);
    ExternalIntLine_Initialization(3, 6, PTC_ISR);      /* Initialize line IRQ3 with a priority of 6. Set PTC_ISR as the Interrupt Service Routine */
    M_PSP_WRITE_REGISTER_32(Select_INT, 0x3);

    /* Initialize Uart */
    uartInit();//将该函数内配置中断的数字改为0x01
    PTC_Initialization(); 
    M_PSP_WRITE_REGISTER_32(SegEn_ADDR, 0x0); 

    pspInterruptsEnable();                              /* Enable all interrupts in mstatus CSR */
	  M_PSP_SET_CSR(D_PSP_MIE_NUM, D_PSP_MIE_MEIE_MASK);  /* Enable external interrupts in mie CSR */

    while(1){
      //value = uart_inbyte();
    }
}