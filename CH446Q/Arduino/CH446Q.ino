#include "Arduino.h"
// #include "CH446Q.h"

#define STB (1 << PORTB3)
#define STB_DDR (1 << DDB3)

#define DAT (1 << PORTB4)
#define DAT_DDR (1 << DDB4)

#define AY0 (1 << PORTB0) 
#define AY1 (1 << PORTB1)
#define AY2 (1 << PORTB2)

#define AY0_DDR (1 << DDB0)
#define AY1_DDR (1 << DDB1)
#define AY2_DDR (1 << DDB2)

#define AX0 (1 << PORTC0) //A0
#define AX1 (1 << PORTC1) //A1
#define AX2 (1 << PORTC2) //A2
#define AX3 (1 << PORTC3) //A3

#define AX0_DDR (1 << DDC0) //A0
#define AX1_DDR (1 << DDC1) //A1
#define AX2_DDR (1 << DDC2) //A2
#define AX3_DDR (1 << DDC3) //A3

#define AX_PORT PORTC
#define AY_PORT PORTB
#define CONTROL_PORT PORTB

#define AX_PORT_DDR DDRC
#define AY_PORT_DDR DDRB 
#define CONTROL_PORT_DDR DDRB

#define ADDR0 (1 << PORTD4)
#define ADDR1 (1 << PORTD5)
#define ADDR2 (1 << PORTD6)
#define ADDR3 (1 << PORTD7)

#define ADDR0_DDR (1 << DDD4)
#define ADDR1_DDR (1 << DDD5)
#define ADDR2_DDR (1 << DDD6)
#define ADDR3_DDR (1 << DDD7)

#define ADDR_PORT PORTD
#define ADDR_PORT_DDR DDRD

#define MAX_ADDRESS 15 // change later
#define MAX_AX 15 // 2^n -1
#define MAX_AY 7

int setConnection(uint8_t addr, uint8_t AX, uint8_t AY, bool mode){
  if (addr > MAX_ADDRESS || AX > MAX_AX || AY > MAX_AY){
    return -1;
  }

  ADDR_PORT = (ADDR_PORT & 0x0F) | (addr << 4); /// pazq starta stojnost na 4-te bita koito ne iskam da pipam i zadavam nova stojnost na 4 bita, kojto promenqm

  AX_PORT = (AX_PORT & 0xF0) | AX;

  AY_PORT = (AY_PORT & 0xF8) | AY; 

  if (mode){
    CONTROL_PORT |= DAT;
  }else{
    CONTROL_PORT &= ~DAT;
  }

  CONTROL_PORT |= STB;

  delayMicroseconds(2);

  CONTROL_PORT &= ~STB;

  return 1;
} 


void setup(){
    ADDR_PORT_DDR |= ADDR0_DDR | ADDR1_DDR | ADDR2_DDR | ADDR3_DDR; // Init D Port Arduino

    ADDR_PORT &= ~(ADDR0 | ADDR1 | ADDR2 | ADDR3);


    AY_PORT_DDR |= AY0_DDR | AY1_DDR | AY2_DDR;// Init B Port Arduino

    AY_PORT &= ~(AY0 | AY1 | AY2);


    AX_PORT_DDR |= AX0_DDR | AX1_DDR | AX2_DDR | AX3_DDR;// Init C Port Arduino

    AX_PORT &= ~(AX0 | AX1 | AX2 | AX3);


    CONTROL_PORT_DDR |= STB_DDR | DAT_DDR;

    CONTROL_PORT |= DAT;
    CONTROL_PORT &= ~STB;

    setConnection(0b1111, 1, 1, true);

}

void loop(){
  
}
