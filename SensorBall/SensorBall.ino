//Define constants
#define sysVolt 5
#define sysMilliVolt 5000

const float roRED = 98.5986; //Calculated ro value using the concentration of the specified gas in air
const float roOX = 0.7035; //Calculated ro value using the concentration of the specified gas in air
const float roNH3 = 129.5987; //Calculated ro value using the concentration of the specified gas in air

const float mRED_CO = 0.78;
const float mRED_H2S = 0.074;
const float mRED_Ethanol = 0.5769;
const float mRED_Hydrogen = 0.5185;
const float mRED_Ammonia = 0.2;
const float mRED_Methane = 0.2125;
const float mRED_Propane = 0.5;
const float mRED_IsoButane = 0.0975;

const float yRED_CO = 3.5;
const float yRED_H2S = 0.45;
const float yRED_Ethanol = 1.5;
const float yRED_Hydrogen = 0.85;
const float yRED_Ammonia = 1.0;
const float yRED_Methane = 4.1;
const float yRED_Propane = 10.41675;
const float yRED_IsoButane = 0.18335;

const float mOX_NO2 = 0.607;
const float mOX_NO = 0.069;
const float mOX_Hydrogen = 0.0571;

const float yOX_NO2 = 6;
const float yOX_NO = 0.7;
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

//Define global variables
bool update = true;

struct ratio 
{
    float RED;
    float OX;
    float NH3;
};

struct RED
{
    float CO;
    float H2S;
    float Ethanol;
    float Hydrogen;
    float Ammonia;
    float Methane;
    float Propane;
    float IsoButane;
};

struct OX
{
    float NO2;
    float NO;
    float Hydrogen;
};

struct NH3
{
    float Hydrogen;
    float Ethanol;
    float Ammonia;
    float Propane;
    float IsoButane;
};

//initialization function
void setup()
{
    Serial.begin(9600);
    //Keyboard.begin();
}

//Main loop
void loop()
{
    update = !update;
/*
    if (Serial.available() > 0) {
        // read incoming serial data:
        char inChar = Serial.read();
        // Type the next ASCII value from what you received:
        Keyboard.write(inChar + 1);
    }
*/
    ratio gasSenseVals = GasSense(A1, A2, A3);

    RED redCon = REDConcentration(gasSenseVals.RED);
    OX oxCon = OXConcentration(gasSenseVals.OX);
    NH3 nh3Con = NH3Concentration(gasSenseVals.NH3);

    dispConcentrations(redCon, oxCon, nh3Con, update);

    updateTemp(update);
    delay(300);
}

float tempSense(int sensePin) 
{
    /*
        this function is used to calculate the 
        temperature from the TMP36 temperature sensor
    */
    int senseVal = analogRead(sensePin);
    float voltage = senseVal*(sysMilliVolt/1024);
    float temp = (voltage-400)/10;
    return temp;
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
    float REDVoltage = REDSenseVal*(sysMilliVolt/1024);
    Serial.println(REDVoltage);
    float rsRED = (sysMilliVolt-REDVoltage)/REDVoltage;
    float REDRatio = rsRED/roRED;

    int OXSenseVal = analogRead(OXSensePin);
    float OXVoltage = OXSenseVal*(sysMilliVolt/1024);
    Serial.println(OXVoltage);
    float rsOX = (sysMilliVolt-OXVoltage)/OXVoltage;
    float OXRatio = rsOX/roOX;


    int NH3SenseVal = analogRead(NH3SensePin);
    float NH3Voltage = NH3SenseVal*(sysMilliVolt/1024);
    Serial.println(REDVoltage);
    float rsNH3 = (sysMilliVolt-NH3Voltage)/NH3Voltage;
    float NH3Ratio = rsNH3/roNH3;

    return {REDRatio, OXRatio, NH3Ratio};
}

RED REDConcentration(float senseVal)
{
    float CO = logFunction(senseVal, yRED_CO, mRED_CO);
    float H2S = logFunction(senseVal, yRED_H2S, mRED_H2S);
    float Ethanol = logFunction(senseVal, yRED_Ethanol, mRED_Ethanol);
    float Hydrogen = logFunction(senseVal, yRED_Hydrogen, mRED_Hydrogen);
    float Ammonia = logFunction(senseVal, yRED_Ammonia, mRED_Ammonia);
    float Methane = logFunction(senseVal, yRED_Methane, mRED_Methane);
    float Propane = logFunction(senseVal, yRED_Propane, mRED_Propane);
    float IsoButane = logFunction(senseVal, yRED_IsoButane, mRED_IsoButane);

    return {CO, H2S, Ethanol, Hydrogen, Ammonia, Methane, Propane, IsoButane};
}

OX OXConcentration(float senseVal)
{
    float NO2 = logFunction(senseVal, yOX_NO2, mOX_NO2);
    float NO = logFunction(senseVal, yOX_NO, mOX_NO);
    float Hydrogen = logFunction(senseVal, yOX_Hydrogen, mOX_Hydrogen);

    return {NO2, NO, Hydrogen};
}

NH3 NH3Concentration(float senseVal)
{
    float Hydrogen = logFunction(senseVal, yNH3_Hydrogen, mNH3_Hydrogen);
    float Ethanol = logFunction(senseVal, yNH3_Ethanol, mNH3_Ethanol);
    float Ammonia = logFunction(senseVal, yNH3_Ammonia, mNH3_Ammonia);
    float Propane = logFunction(senseVal, yNH3_Propane, mNH3_Propane);
    float IsoButane = logFunction(senseVal, yNH3_IsoButane, mNH3_IsoButane);

    return {Hydrogen, Ethanol, Ammonia, Propane, IsoButane};
}

/*
float REDSense(int sensePin)
{
    /*
        this funcation is used to calculate the rs/ro ratio 
        of the RED sensor on the MiCS6814 gas sensor
    
    int senseVal = analogRead(sensePin);
    float voltage = senseVal*(sysVolt/1024);
    float rsRED = (sysVolt-voltage)/voltage;
    float ratio = rsRED/roRED;
    return ratio;
}

float OXSense(int sensePin)
{
    /*
        this funcation is used to calculate the rs/ro ratio 
        of the OX sensor on the MiCS6814 gas sensor
    
    int senseVal = analogRead(sensePin);
    float voltage = senseVal*(sysVolt/1024);
    float rsOX = (sysVolt-voltage)/voltage;
    float ratio = rsOX/roOX;
    return ratio;
}

float NH3Sense(int sensePin)
{
    /*
        this funcation is used to calculate the rs/ro ratio 
        of the NH3 sensor on the MiCS6814 gas sensor
    
    int senseVal = analogRead(sensePin);
    float voltage = senseVal*(sysVolt/1024);
    float rsNH3 = (sysVolt-voltage)/voltage;
    float ratio = rsNH3/roNH3;
    return ratio;
}
*/

void dispConcentrations(RED redVals, OX oxVals, NH3 nh3Vals, bool update)
{
    String units = " ppm";

    float RED_Cons[8] = {redVals.CO, redVals.H2S, redVals.Ethanol, redVals.Hydrogen, redVals.Ammonia, redVals.Methane, redVals.Propane, redVals.IsoButane};
    float OX_Cons[3] = {oxVals.NO2, oxVals.NO, oxVals.Hydrogen};
    float NH3_Cons[5] = {nh3Vals.Hydrogen, nh3Vals.Ethanol, nh3Vals.Ammonia, nh3Vals.Propane, nh3Vals.IsoButane};

    if (update == true)
    {
        for (int i = 0; i<sizeof(RED_Cons);i++)
        {
            String out = String(RED_Cons[i]+units);
            Serial.println(out);
        }
        Serial.println();
        for (int i = 0; i<sizeof(OX_Cons);i++)
        {
            String out = String(OX_Cons[i]+units);
            Serial.println(out);
        }
        Serial.println();
        for (int i = 0; i<sizeof(NH3_Cons);i++)
        {
            String out = String(NH3_Cons[i]+units);
            Serial.println(out);
        }
    }
}

void updateTemp(bool update) 
{
    /*
        this function is used to display the temperature found in tempSense
        this function also displays the unit of the temperature degrees celcius 
    */
    float temp1 = tempSense(A0);
  
    if (update == true)
    {
        String Deg = "°C";
        String temperature = String(temp1+Deg);
        Serial.println(temperature);
    }
}

float logFunction(float y, float k, float m)
{
    /*
        This function is used to calculate the concentration 
        of a specified gas using a power function 

        This function takes the inputs y, k and m. 
        - y is the rs/ro ratio from the specified sensor
        - k is the y intercept of the specified gas on the specified sensor graph, found using the MiCS6814 data sheet
        - m is the gradient of the specified gas on the specified sensor graph, found using the MiCS6814 data sheet
    */
    float concentration = pow((y/k), (1/m));
    return concentration;
}
