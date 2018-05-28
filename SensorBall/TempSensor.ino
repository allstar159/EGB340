

float senseTemp(int sensePin)
{
  /*
      this function is used to calculate the
      temperature from the TMP36 temperature sensor
  */
  int senseVal = analogRead(sensePin);
  float voltage = senseVal * (sysMilliVolt / 1024);
  return (voltage - 500) / 10;
}

void printTemp(float temp)
{
  /*
      this function is used to display the temperature found in tempSense
      this function also displays the unit of the temperature degrees Celcius
  */
  String deg = "°C";
  String temperature = String(temp + deg);
  Serial.print("Temperature: ");
  Serial.println(temperature);
}
