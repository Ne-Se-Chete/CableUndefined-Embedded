#include "Arduino.h"
#include <FastLED.h>

#define LED_PIN_1   3
#define NUM_LEDS_1  8

#define LED_PIN_2   2   
#define NUM_LEDS_2  32   

CRGB leds_1[NUM_LEDS_1];
CRGB leds_2[NUM_LEDS_2];

void(* resetFunc) (void) = 0;

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

  if (addr == 1000){
    addr = 0b1000;
  }else if (addr == 1001){
    addr = 0b1001;
  }else{
    return;
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

    Serial.begin(9600);

    ADDR_PORT_DDR |= ADDR0_DDR | ADDR1_DDR | ADDR2_DDR | ADDR3_DDR; // Init D Port Arduino

    ADDR_PORT &= ~(ADDR0 | ADDR1 | ADDR2 | ADDR3);


    AY_PORT_DDR |= AY0_DDR | AY1_DDR | AY2_DDR;// Init B Port Arduino

    AY_PORT &= ~(AY0 | AY1 | AY2);


    AX_PORT_DDR |= AX0_DDR | AX1_DDR | AX2_DDR | AX3_DDR;// Init C Port Arduino

    AX_PORT &= ~(AX0 | AX1 | AX2 | AX3);


    CONTROL_PORT_DDR |= STB_DDR | DAT_DDR;

    CONTROL_PORT |= DAT;
    CONTROL_PORT &= ~STB;

    FastLED.addLeds<WS2812, LED_PIN_1, GRB>(leds_1, NUM_LEDS_1);
    FastLED.addLeds<WS2812, LED_PIN_2, GRB>(leds_2, NUM_LEDS_2);

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 8; y++) {
            setConnection(0b1000, x, y, false);
            setConnection(0b1001, x, y, false);
        }
    }
    
    for (int i = 0; i < NUM_LEDS_1; i++) {
        leds_1[i] = CRGB(0, 0, 0);;
    }

    for (int i = 0; i < NUM_LEDS_2; i++) {
        leds_2[i] = CRGB(0, 0, 0);;
    }

    FastLED.show();

    Serial.println("Ready");
}

void loop(){
  if(Serial.available() > 0){
    // String input = "1000;x15;y7;false;MCUBreadboard 15;MainBreadboard 1";
    String input = Serial.readStringUntil('\n');

    if(input.equals("Clear")) {
      for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 8; y++) {
            setConnection(0b1000, x, y, false);
            setConnection(0b1001, x, y, false);
        }
      }
    }

    Serial.println(input);


    char* token;
    token = strtok((char*)input.c_str(), ";"); 
    int instruction = atoi(token);

    Serial.println(instruction);

    // get x
    token = strtok(NULL, ";");
    char* xStr = token;

    // get y
    token = strtok(NULL, ";"); 
    char* yStr = token;

    if (xStr[0] == 'y') {
      char* temp = yStr;
      yStr = xStr;
      xStr = temp;
    }

    xStr[0] = '0';
    yStr[0] = '0';

    int x = atoi(xStr);
    int y = atoi(yStr);

    // get MODE
    token = strtok(NULL, ";"); 
    bool mode = (strcmp(token, "true") == 0);
    
    /// NOT ALWAYS vvv

    int red = random(5, 26);
    int green = random(5, 26);
    int blue = random(5, 26);

    // get LED 1
    token = strtok(NULL, ";"); 
    String led1Instruction = token;
    int spaceIndex = led1Instruction.indexOf(' ');
    String boardName = led1Instruction.substring(0, spaceIndex);
    if (mode){
      if (boardName.equals("MCUBreadboard")) {
        int led1Idx = atoi(led1Instruction.substring(spaceIndex + 1).c_str());
        // Serial.println(led1Idx);
        leds_1[led1Idx] = CRGB(red, green, blue);
      }
      else  if (boardName.equals("MainBreadboard")) {
        int led2Idx = atoi(led1Instruction.substring(spaceIndex + 1).c_str());
        // Serial.println(led1Idx);
        leds_2[led2Idx] = CRGB(red, green, blue);
      }
      
      // get LED 2
      token = strtok(NULL, ";"); 
      String led2Instruction = token;
      spaceIndex = led2Instruction.indexOf(' ');
      boardName = led2Instruction.substring(0, spaceIndex);

      if (boardName.equals("MCUBreadboard")) {
        int led2Idx = atoi(led2Instruction.substring(spaceIndex + 1).c_str());
        // Serial.println(led2Idx);
        leds_1[led2Idx] = CRGB(red, green, blue);
      }
      else  if (boardName.equals("MainBreadboard")) {
        int led2Idx = atoi(led2Instruction.substring(spaceIndex + 1).c_str());
        // Serial.println(led2Idx);
        leds_2[led2Idx] = CRGB(red, green, blue);
      }
    }else{
       if (boardName.equals("MCUBreadboard")) {
        int led1Idx = atoi(led1Instruction.substring(spaceIndex + 1).c_str());
        // Serial.println(led1Idx);
        leds_1[led1Idx] = CRGB(0, 0, 0);
      }
      else  if (boardName.equals("MainBreadboard")) {
        int led2Idx = atoi(led1Instruction.substring(spaceIndex + 1).c_str());
        // Serial.println(led1Idx);
        leds_2[led2Idx] = CRGB(0, 0, 0);
      }
      
      // get LED 2
      token = strtok(NULL, ";"); 
      String led2Instruction = token;
      spaceIndex = led2Instruction.indexOf(' ');
      boardName = led2Instruction.substring(0, spaceIndex);

      if (boardName.equals("MCUBreadboard")) {
        int led2Idx = atoi(led2Instruction.substring(spaceIndex + 1).c_str());
        // Serial.println(led2Idx);
        leds_1[led2Idx] = CRGB(0, 0, 0);
      }
      else  if (boardName.equals("MainBreadboard")) {
        int led2Idx = atoi(led2Instruction.substring(spaceIndex + 1).c_str());
        // Serial.println(led2Idx);
        leds_2[led2Idx] = CRGB(0, 0, 0);
      }
    }

    FastLED.show();

    setConnection(instruction, x, y, mode);

  }
}