const float mRED_CO = 0.78;
//const float mRED_H2S = 0.074;
const float mRED_Ethanol = 0.5769;
const float mRED_Hydrogen = 0.5185;
const float mRED_Ammonia = 0.2;
const float mRED_Methane = 0.2125;
const float mRED_Propane = 0.5;
const float mRED_IsoButane = 0.0975;

const float yRED_CO = 3.5;
//const float yRED_H2S = 0.45;
const float yRED_Ethanol = 1.5;
const float yRED_Hydrogen = 0.85;
const float yRED_Ammonia = 1.0;
const float yRED_Methane = 4.1;
const float yRED_Propane = 10.41675;
const float yRED_IsoButane = 0.18335;

const float mOX_NO2 = 0.855;
//const float mOX_NO = 0.069;
const float mOX_Hydrogen = 0.0571;

const float yOX_NO2 = 6;
//const float yOX_NO = 0.7;
const float yOX_Hydrogen = 1.4;

const float mNH3_Hydrogen = 0.263;
const float mNH3_Ethanol = 0.28;
const float mNH3_Ammonia = 0.4211;
const float mNH3_Propane = 0.2051;
const float mNH3_IsoButane = 0.3542;

const float yNH3_Hydrogen = 2;
const float yNH3_Ethanol = 0.5;
const float yNH3_Ammonia = 0.7;
const float yNH3_Propane = 10.0;
const float yNH3_IsoButane = 10.6668;

float roRED = 233.18; //Calculated ro value using the concentration of the specified gas in air
float roOX = 11.12; //Calculated ro value using the concentration of the specified gas in air
float roNH3 = 190.09; //Calculated ro value using the concentration of the specified gas in air

void setupGas(int REDSensePin, int OXSensePin, int NH3SensePin)
{
  /*
      this function is used to calculate the ro value
      of the RED, OX and NH3 sensors on the MiCS6814 gas sensor

      inputs:
      - RED analog sensor pin
      - OX analog sensor pin
      - NH3 analog sensor pin
  */
  //initialize the storage variables for the gas sensor resistance
  float roR = 0;
  float roO = 0;
  float roN = 0;
  //define the number of data points you want to collect
  int dataSet = 100;

  for (int i = 0; i < dataSet; i++)
  {
    int REDSenseVal = analogRead(REDSensePin);
    float REDVoltage = REDSenseVal * (sysMilliVolt / 1023);
    roR = roR + senseResistance / ((sysMilliVolt / REDVoltage) - 1);

    int OXSenseVal = analogRead(OXSensePin);
    float OXVoltage = OXSenseVal * (sysMilliVolt / 1023);
    roO = roO + senseResistance / ((sysMilliVolt / OXVoltage) - 1);


    int NH3SenseVal = analogRead(NH3SensePin);
    float NH3Voltage = NH3SenseVal * (sysMilliVolt / 1023);
    roN = roN + senseResistance / ((sysMilliVolt / NH3Voltage) - 1);
  }

  roRED = roR / dataSet;
  roOX = roO / dataSet;
  roNH3 = roN / dataSet;

  Serial.println(roRED);
  Serial.println(roOX);
  Serial.println(roNH3);
}

ratio GasSense(int REDSensePin, int OXSensePin, int NH3SensePin)
{
  /*
      this function is used to calculate the rs/ro ratio
      of the RED, OX and NH3 sensors on the MiCS6814 gas sensor

      inputs:
      - RED analog sensor pin
      - OX analog sensor pin
      - NH3 analog sensor pin

      outputs:
      - Rs/Ro ratio for the RED sensor
      - Rs/Ro ratio for the OX sensor
      - Rs/Ro ratio for the NH3 sensor
  */
  int REDSenseVal = analogRead(REDSensePin);
  float REDVoltage = REDSenseVal * (sysMilliVolt / 1023);
  float rsRED = senseResistance / ((sysMilliVolt / REDVoltage) - 1);
  float REDRatio = rsRED / roRED;

  int OXSenseVal = analogRead(OXSensePin);
  float OXVoltage = OXSenseVal * (sysMilliVolt / 1023);
  float rsOX = senseResistance / ((sysMilliVolt / OXVoltage) - 1);
  float OXRatio = rsOX / roOX;


  int NH3SenseVal = analogRead(NH3SensePin);
  float NH3Voltage = NH3SenseVal * (sysMilliVolt / 1023);
  float rsNH3 = senseResistance / ((sysMilliVolt / NH3Voltage) - 1);
  float NH3Ratio = rsNH3 / roNH3;

  return {REDRatio, OXRatio, NH3Ratio};
}

RED REDConcentration(float senseVal)
{
  float CO = logFunction(senseVal, -1.179, 1 / 4.385);
  //float H2S = logFunction(senseVal, yRED_H2S, mRED_H2S);
  float Ethanol = logFunction(senseVal, -1.552, 1 / 1.622);
  float Hydrogen = logFunction(senseVal, -1.8, 1 / 0.73);
  float Ammonia = logFunction(senseVal, yRED_Ammonia, mRED_Ammonia);
  float Methane = logFunction(senseVal, -4.363, 1 / 630.957);
  float Propane = logFunction(senseVal, yRED_Propane, mRED_Propane);
  float IsoButane = logFunction(senseVal, yRED_IsoButane, mRED_IsoButane);

  return {CO, Ethanol, Hydrogen, Ammonia, Methane, Propane, IsoButane};
}

OX OXConcentration(float senseVal)
{
  float NO2 = logFunction(senseVal, 1.007, 1 / 6.855);
  //float NO = logFunction(senseVal, yOX_NO, mOX_NO);
  float Hydrogen = logFunction(senseVal, yOX_Hydrogen, mOX_Hydrogen);

  return {NO2, Hydrogen};
}

NH3 NH3Concentration(float senseVal)
{
  float Hydrogen = logFunction(senseVal, yNH3_Hydrogen, mNH3_Hydrogen);
  float Ethanol = logFunction(senseVal, yNH3_Ethanol, mNH3_Ethanol);
  float Ammonia = logFunction(senseVal, -1.67, 1 / 1.47);
  float Propane = logFunction(senseVal, -2.518, 1 / 570.164);
  float IsoButane = logFunction(senseVal, -2.138, 1 / 398.107);

  return {Hydrogen, Ethanol, Ammonia, Propane, IsoButane};
}

void dispConcentrations(RED redVals, OX oxVals, NH3 nh3Vals)
{
  String units = " ppm";

  float RED_Cons[7] = {redVals.CO, redVals.Ethanol, redVals.Hydrogen, redVals.Ammonia, redVals.Methane, redVals.Propane, redVals.IsoButane};
  //String RED_Gas[7] = {"CO\t  :\t", "Ethanol\t  :\t", "Hydrogen  :\t", "Ammonia\t  :\t", "Methane\t  :\t", "Propane\t  :\t", "IsoButane :\t"};
  String RED_Gas[7] = {"CO: ", "Ethanol: ", "Hydrogen: ", "Ammonia: ", "Methane: ", "Propane: ", "IsoButane: "};
  float OX_Cons[2] = {oxVals.NO2, oxVals.Hydrogen};
  //String OX_Gas[2] = {"NO2\t  :\t", "Hydrogen  :\t"};
  String OX_Gas[2] = {"NO2: ", "Hydrogen: "};
  float NH3_Cons[5] = {nh3Vals.Hydrogen, nh3Vals.Ethanol, nh3Vals.Ammonia, nh3Vals.Propane, nh3Vals.IsoButane};
  //String NH3_Gas[5] = {"Hydrogen  :\t", "Ethanol\t  :\t", "Ammonia\t  :\t", "Propane\t  :\t", "IsoButane :\t"};
  String NH3_Gas[5] = {"Hydrogen: ", "Ethanol: ", "Ammonia: ", "Propane: ", "IsoButane: "};


  for (int i = 0; i < 7; i++)
  {
    String outRED = String(RED_Cons[i]);
    Serial.print(RED_Gas[i]);
    Serial.print(outRED);
    Serial.println(units);
  }
  Serial.println();
  for (int i = 0; i < 2; i++)
  {
    String outOX = String(OX_Cons[i]);
    Serial.print(OX_Gas[i]);
    Serial.print(outOX);
    Serial.println(units);
  }
  Serial.println();
  for (int i = 0; i < 5; i++)
  {
    String outNH3 = String(NH3_Cons[i]);
    Serial.print(NH3_Gas[i]);
    Serial.print(outNH3);
    Serial.println(units);
  }
}

void printResistance(int REDSensePin, int OXSensePin, int NH3SensePin)
{
  int REDSenseVal = analogRead(REDSensePin);
  float REDVoltage = REDSenseVal * (sysMilliVolt / 1023);
  float rsRED = senseResistance / ((sysMilliVolt / REDVoltage) - 1);

  int OXSenseVal = analogRead(OXSensePin);
  float OXVoltage = OXSenseVal * (sysMilliVolt / 1023);
  float rsOX = senseResistance / ((sysMilliVolt / OXVoltage) - 1);

  int NH3SenseVal = analogRead(NH3SensePin);
  float NH3Voltage = NH3SenseVal * (sysMilliVolt / 1023);
  float rsNH3 = senseResistance / ((sysMilliVolt / NH3Voltage) - 1);

  Serial.print("Ro RED\t  :\t");
  Serial.print(roRED);
  Serial.println("\tkΩ");
  Serial.print("Rs RED\t  :\t");
  Serial.print(rsRED);
  Serial.println("\tkΩ");
  Serial.print("Ro OX\t  :\t");
  Serial.print(roOX);
  Serial.println("\tkΩ");
  Serial.print("Rs OX\t  :\t");
  Serial.print(rsOX);
  Serial.println("\tkΩ");
  Serial.print("Ro NH3\t  :\t");
  Serial.print(roNH3);
  Serial.println("\tkΩ");
  Serial.print("Rs NH3\t  :\t");
  Serial.print(rsNH3);
  Serial.println("\tkΩ");
}

float logFunction(float y, float power, float modifier)
{
  /*
      This function is used to calculate the concentration
      of a specified gas using a power function

      This function takes the inputs y, k and m.
      - y is the rs/ro ratio from the specified sensor
      - k is the y intercept of the specified gas on the specified sensor graph, found using the MiCS6814 data sheet
      - m is the gradient of the specified gas on the specified sensor graph, found using the MiCS6814 data sheet
  */
  float concentration = pow(y, power) * modifier;
  return concentration;
}
/*
  float logFunction(float y, float power, float modifier)
  {
    /*
        This function is used to calculate the concentration
        of a specified gas using a power function

        This function takes the inputs y, k and m.
        - y is the rs/ro ratio from the specified sensor
        - k is the y intercept of the specified gas on the specified sensor graph, found using the MiCS6814 data sheet
        - m is the gradient of the specified gas on the specified sensor graph, found using the MiCS6814 data sheet

    float concentration = pow(y/k, 1/m);
    return concentration;
  }
*/
