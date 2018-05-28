// Define constants
#define sysVolt 5
#define sysMilliVolt 5000
#define senseResistance 68 // Note this is in killiohms
#define SAFE_LED_PIN 4
#define TEMP_LED_PIN 5
#define TEMP_SAFE_LIMIT 26.0
#define UPDATE_INTERVAL 4000

struct ratio
{
  float RED;
  float OX;
  float NH3;
};

struct RED
{
  float CO;
  //float H2S;
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
  //float NO;
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



// Global Variables
unsigned long lastUpdateTime = 0;
unsigned long lastToneTime = 0;
int updateNumber = 0;
bool tempDanger = false;
bool gasDanger = false;


void setup()
{
  // Set LED indicator PINs to output
  pinMode(SAFE_LED_PIN, OUTPUT);
  pinMode(TEMP_LED_PIN, OUTPUT);

  setupBluetooth();
  setupGas(A1, A2, A3);

  //Keyboard.begin();
  //ledFlash();
  //Serial.print("What data would you like to see? (Gas/Temperature/Resistance/All)\t");
  //Serial.print("Would you like to see the current gas concentrations? (y/n)\t");
}

void loop()
{
  // Check if another update is due
  if ((millis() - lastUpdateTime) > UPDATE_INTERVAL)
  {
    updateNumber++;
    Serial.println(updateNumber);

    // Update temperature
    // Delay and remeasure for accurate results (due to interference or something)
    float temp = senseTemp(A0);
    delay(10);
    temp = senseTemp(A0);
    printTemp(temp);

    // Update gas concentrations
    ratio gasSenseVals = GasSense(A1, A2, A3);
    RED redCon = REDConcentration(gasSenseVals.RED);
    OX oxCon = OXConcentration(gasSenseVals.OX);
    NH3 nh3Con = NH3Concentration(gasSenseVals.NH3);
    //dispConcentrations(redCon, oxCon, nh3Con);
    Serial.println("Safe gas concentrations.");

    Serial.println(); // Blank line

    tempDanger = temp > 26.0;
    gasDanger = false; // TODO

    // Control LED indicator
    if (!tempDanger && !gasDanger)
    {
      digitalWrite(SAFE_LED_PIN, HIGH);
    }
    else
    {
      digitalWrite(SAFE_LED_PIN, LOW);
    }

    if (tempDanger)
    {
      digitalWrite(TEMP_LED_PIN, HIGH);
    }
    else
    {
      digitalWrite(TEMP_LED_PIN, LOW);
    }

    // Start buzzer tone
    buzzerStartTone();

    lastUpdateTime = millis();
  }

  buzzerLoop();
}

/*
  void ledFlash(void)
  {
    // This function is called in setup to notify the user when the calibration of the device has been complete
    int LED = 13;
    int wait = 100;

    pinMode(LED, OUTPUT);

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED, HIGH);
        delay(wait);
        digitalWrite(LED, LOW);
        delay(wait);
    }
  }
*/

/*
  void userInputs(RED redCon, OX oxCon, NH3 nh3Con)
  {
    int mode;

    while (Serial.available() > 0)
    {
        delay(3);
        // read incoming serial data:
        inputChar = Serial.read();
        input += inputChar;
        Serial.print(String(inputChar));

        if (inputChar == '\r')
        {
            if (input.equalsIgnoreCase("gas\r"))
            {
                mode = 1;
            }else if (input.equalsIgnoreCase("temp\r") || input.equalsIgnoreCase("temperature\r"))
            {
                mode = 2;
            }else if (input.equalsIgnoreCase("res\r") || input.equalsIgnoreCase("resistance\r"))
            {
                mode = 3;
            }else if (input.equalsIgnoreCase("all\r"))
            {
                mode = 4;
            }else
            {
                mode = 5;
            }

            switch (mode)
            {
                case 1:
                    Serial.println();
                    dispConcentrations(redCon, oxCon, nh3Con, update);
                    Serial.flush();
                    inputChar = (char)0;
                    input = "";
                    Serial.print("What data would you like to see? (Gas/Temperature/Resistance/All)\t");
                    break;
                case 2:
                    Serial.println();
                    updateTemp(update);
                    Serial.flush();
                    inputChar = (char)0;
                    input = "";
                    Serial.print("What data would you like to see? (Gas/Temperature/Resistance/All)\t");
                    break;
                case 3:
                    Serial.println();
                    printResistance(A1, A2, A3);
                    Serial.flush();
                    inputChar = (char)0;
                    input = "";
                    Serial.print("What data would you like to see? (Gas/Temperature/Resistance/All)\t");
                    break;
                case 4:
                    Serial.println();
                    dispConcentrations(redCon, oxCon, nh3Con, update);
                    updateTemp(update);
                    printResistance(A1, A2, A3);
                    Serial.flush();
                    inputChar = (char)0;
                    input = "";
                    Serial.print("What data would you like to see? (Gas/Temperature/Resistance/All)\t");
                    break;
                case 5:
                    Serial.println();
                    Serial.flush();
                    inputChar = (char)0;
                    input = "";
                    Serial.println("Please enter a valid response!");
                    Serial.print("What data would you like to see? (Gas/Temperature/Resistance/All)\t");
                    break;
            }
        }
    }
  }
*/

/*
  void userInput(RED redCon, OX oxCon, NH3 nh3Con)
  {
    while (Serial.available() > 0) {
        delay(3);
        // read incoming serial data:
        inputChar = Serial.read();
        input += inputChar;
        Serial.print(String(inputChar));

        if (inputChar == '\r')
        {
            if (input.equalsIgnoreCase("yes\r") || input.equalsIgnoreCase("y\r"))
            {
                Serial.println();
                updateTemp(update);
                dispConcentrations(redCon, oxCon, nh3Con, update);
                Serial.flush();
                inputChar = (char)0;
                input = "";
                Serial.print("Would you like to see the current gas concentrations? (y/n)\t");
            } else if (input.equalsIgnoreCase("no\r") || input.equalsIgnoreCase("n\r"))
            {
                Serial.flush();
                inputChar = (char)0;
                input = "";
                Serial.print("\r\nWould you like to see the current gas concentrations? (y/n)\t");
                return;
            } else
            {
                Serial.println();
                inputChar = (char)0;
                input = "";
                Serial.println("Please enter a valid response");
                Serial.print("Would you like to see the current gas concentrations? (y/n)\t");
            }
        }
    }
  }
*/
