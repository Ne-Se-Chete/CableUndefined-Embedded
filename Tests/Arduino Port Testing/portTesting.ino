void setup() {
  DDRC |= (1 << DDC3) | (1 << DDC4); 

}

void loop() {
  PORTC |= (1 << PORTC3) | (1 << PORTC4);   
  _delay_ms(500);   
  PORTC &= ~(1 << PORTC3) | (1 << PORTC4);    
  _delay_ms(500);         

  //PORTB ^= (1 << PB5); 
}
