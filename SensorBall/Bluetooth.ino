// Setup Bluetooth for MLDP
void setupBluetooth()
{
  // set WAKE_SW to HIGH
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  // Unnecessary to set other pins due to software settings
  // set WAKE_HW to HIGH
  //pinMode(3, OUTPUT);
  //digitalWrite(3, HIGH);
  // set CMD/MDLP to LOW (CMD)
  //pinMode(4, OUTPUT);
  //digitalWrite(4, LOW);

  Serial.begin(115200); // use RN4020 default baud rate
  delay(1000);

  Serial.println("SF,2"); // perform complete factory reset
  delay(500);
  Serial.println("+"); // echo on
  delay(500);
  Serial.println("SR,32000800"); // set Enable MDLP, Auto MLDP, UART Flow Control, Auto Advertise
  delay(500);
  Serial.println("SN,SensorBall"); // set device name
  delay(500);
  Serial.println("R,1"); // reboot to apply settings
  delay(1000);
}
