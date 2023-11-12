#if defined(D_NEXYS_A7)
   #include <bsp_printf.h>
   #include <bsp_mem_map.h>
   #include <bsp_version.h>
#else
   PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>

#define SegEn_ADDR      0x80001038
#define SegDig_ADDR     0x8000103C

#define SPI_BASE        0x80001100
#define SPI_ENABLE      0x80001120
#define SPI_DATA        0x80001110

#define READ_IO(dir) (*(volatile unsigned *)dir)
#define WRITE_IO(dir, value) { (*(volatile unsigned *)dir) = (value); }

#define Mydelay 1000

void delay(int num)
{
    volatile int i;
    for(i=0; i<num; i++);
}

void SPI_INIT() {
    WRITE_IO(SPI_BASE, 0x3);
    delay(Mydelay);

    WRITE_IO(SPI_BASE, 0x43);
    delay(Mydelay);
}

void SPI_WRITE(unsigned data) {    
    WRITE_IO(SPI_DATA, data);
    delay(Mydelay);
}

unsigned SPI_READ() {
    unsigned data;
    
    data = READ_IO(SPI_DATA);
    delay(Mydelay);
    
    return data;
}

void ADXL362_WRITE(unsigned int addr, unsigned int data) {
    WRITE_IO(SPI_ENABLE, 1);
    delay(Mydelay);

    SPI_WRITE(0xA);
    delay(Mydelay);
    
    SPI_WRITE(addr);
    delay(Mydelay);
    
    SPI_WRITE(data);
    delay(Mydelay);

    WRITE_IO(SPI_ENABLE, 0);
    delay(Mydelay);
}

void ADXL362_READ(unsigned int addr, unsigned int* data) {
    WRITE_IO(SPI_ENABLE, 1);
    delay(Mydelay);

    SPI_WRITE(0xB);
    delay(Mydelay);

    SPI_WRITE(addr);
    delay(Mydelay);
    
    SPI_WRITE(0x0);
    delay(Mydelay);
    
    *data = SPI_READ();
    delay(Mydelay);

    *data = SPI_READ();
    delay(Mydelay);

    *data = SPI_READ();
    delay(Mydelay);

    WRITE_IO(SPI_ENABLE, 0);
    delay(Mydelay);
}

void ADXL362_INIT() {
    ADXL362_WRITE(0x20, 0xFA);
    ADXL362_WRITE(0x21, 0);
    ADXL362_WRITE(0x23, 0x96);
    ADXL362_WRITE(0x24, 0);
    ADXL362_WRITE(0x25, 0x1E);
    ADXL362_WRITE(0x27, 0x3F);
    ADXL362_WRITE(0x2B, 0x40);
    ADXL362_WRITE(0x2D, 0xA);
}


int main() {
    unsigned int displayData=0;
    unsigned int realData=0;
    unsigned int xdata, ydata, zdata;
    WRITE_IO(SegEn_ADDR, 0x24);
    WRITE_IO(SegDig_ADDR, displayData);
    SPI_INIT();
    ADXL362_INIT();
    while(1) {
        ADXL362_READ(0x8, &xdata);
        ADXL362_READ(0x9, &ydata);
        ADXL362_READ(0xA, &zdata);
        // ADXL362_READ(0x8, &displayData);
        // realData = 0;
        // for (int i = 7; i > 0; i--) {
        //     realData|=((displayData>>i)&1) ? 1 : 0;
        //     realData <<= 4;
        // }
        // realData|=((displayData)&1) ? 1 : 0;
        // WRITE_IO(SegDig_ADDR, realData);
        realData = 0;
        realData |= xdata << 24;
        realData |= ydata << 12;
        realData |= zdata;
        WRITE_IO(SegDig_ADDR, realData);
        delay(Mydelay);
    }

    return 0;
}

