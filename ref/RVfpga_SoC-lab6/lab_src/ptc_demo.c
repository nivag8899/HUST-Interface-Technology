#define SegEn_ADDR    0x80001038
#define SegDig_ADDR   0x8000103C

#define RPTC_CNTR     0x80001200
#define RPTC_HRC      0x80001204
#define RPTC_LRC      0x80001208
#define RPTC_CTRL     0x8000120c

#define READ_Reg(dir) (*(volatile unsigned *)dir)
#define WRITE_Reg(dir, value) { (*(volatile unsigned *)dir) = (value); }

int main ( void )
{
    int counter_value, count=0;

    WRITE_Reg(SegEn_ADDR, 0x0);

    WRITE_Reg(RPTC_LRC, 0x2FFFFFF);

    while (1) {

        WRITE_Reg(SegDig_ADDR, count);

        count++;

        WRITE_Reg(RPTC_CTRL, 0xC0);
        WRITE_Reg(RPTC_CTRL, 0x21);
        while(1){
            counter_value = READ_Reg(RPTC_CTRL);
            counter_value = counter_value & 0x40;
            if(counter_value!=0){
                break;
            }
        }

    }

    return(0);
}
