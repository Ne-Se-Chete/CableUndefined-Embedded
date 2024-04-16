void setup() {
  pinMode(0, OUTPUT); // Set pin 0 as output for the LED
  pinMode(4, OUTPUT); // Set pin 4 as output for the buzzer
  pinMode(3, INPUT);  // Set pin 3 as input for the potentiometer
}

void loop() {
  int sensorValue = analogRead(3); // Read the analog input from pin 3
  
  // Map the input range (0-1023) to the output range (0-255) more linearly
  int outputValue = map(sensorValue, 0, 1023, 0, 255);
  outputValue = constrain(outputValue, 0, 255); // Ensure the output value is within valid range

  analogWrite(0, outputValue); // Output the mapped value to pin 0 (for LED brightness control)

  // Define an array of frequencies
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 659};
  
  // Calculate the index of the frequency based on the potentiometer value
  int index = map(sensorValue, 0, 1023, 0, 9); // Assuming 10 predefined frequencies

  int frequency = frequencies[index]; // Select the frequency from the array
  
  tone(4, frequency); // Generate tone on pin 4 with the selected frequency
}
