#include<p18f4550.h>
#pragma config FOSC = HS              //High Speed Crystal Oscillator
#pragma config WDT=OFF               //Watch Dog Timer disabled
#pragma config LVP=OFF                 //Single-Supply ICSP disabled
#pragma config MCLRE = OFF        //RE3 input pin enabled; MCLR pin disabled. So now we dont need                                                   
                                                             to give High Logic on this pin to keep the PIC functioning
#define  cs PORTBbits.RB0   
void dac(unsigned int);
void delay(unsigned int time);

void main()
{
 int i;
 TRISB=0;                            // PORTB is configured as an output port
 TRISC=0;                            // PORTC is configured as an output port
 PORTC=0;
 PORTB=0;

 SSPSTAT=0xC0;              //Status Register SSPSTAT=11000000
 SSPCON1=0x20;             //Enables serial port pins & set the SPI clock as clock = FOSC/4
 while(1)
   {
      dac(255);  delay(1000);
      dac(127);  delay(1000);
      dac(63);   delay(1000);
   }
}

void dac(unsigned int data)
{
  unsigned int c ;
  unsigned int lower_bits;
  unsigned int upper_bits;  
  c = ((data+1)*16) -1;                       // here we obtain 12 bit data

  //first obtain the upper 8 bits
  upper_bits = c/256;                          // obtain the upper 4 bits 
  upper_bits = (48) | upper_bits;         // append 0011 to the above 4 bits


  //now obtain the lower 8 bits
  lower_bits = 255 & c;                      // ANDing separates the lower 8 bits
  
  cs=0;
  SSPBUF=upper_bits;                      // sending the upper 8 bits serially     
  while(!SSPSTATbits.BF);                // wait until the upper 8 bits are sent
  SSPBUF=lower_bits;                      // sending the lower 8 bits serially   
  while(!SSPSTATbits.BF);                // wait until the lower 8 bits are sent
  cs=1;
}


void delay(unsigned int time)
{
 unsigned int i,j;
 for(i=0;i<time;i++)
   for(j=0;j<120;j++);
}