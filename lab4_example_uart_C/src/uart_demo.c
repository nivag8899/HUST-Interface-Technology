#if defined(D_NEXYS_A7)
   #include <bsp_printf.h>
   #include <bsp_mem_map.h>
   #include <bsp_version.h>
#else
   PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>

int main(void)
{
   int i;

   /* Initialize Uart */
   uartInit();

   while(1){
      /* Print "hello world" message on the serial output (be carrefoul not all the printf formats are supported) */
      printfNexys("hello world\n");
      /* Delay */
      for (i=0;i<10000000;i++){};
   }

}