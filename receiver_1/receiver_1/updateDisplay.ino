//_______________updateDisplay-to print on screen the updated time _________________________________________________________________
void updateDisplay(int remainingTime) {
  dmd.clearScreen( true ); 
  char char_array_remainingTime[String(remainingTime).length() + 1];
  String(remainingTime).toCharArray(char_array_remainingTime, String(remainingTime).length() + 1);
  dmd.drawString(6,2,char_array_remainingTime, String(remainingTime).length(), GRAPHICS_NORMAL);
  // Serial.println(remainingTime);
  // Serial.println(char_array_remainingTime);


}