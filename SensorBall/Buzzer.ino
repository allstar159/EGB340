#define TONE_PIN 8 // Pin for positive connection of buzzer
#define TONE_DURATION_SHORT 300
#define TONE_DURATION_LONG 1000
#define TONE_FREQUENCY_LOW 2000
#define TONE_FREQUENCY_MID 2500
#define TONE_FREQUENCY_HIGH 3000



int tonesSinceLastUpdate = 0;

// Play a tone to the buzzer
void buzzerTone(unsigned int frequency, unsigned long duration)
{
  tone(TONE_PIN, frequency, duration);
  tonesSinceLastUpdate += 1;
}

void buzzerStartTone()
{
  if (gasDanger && tempDanger)
  {
    buzzerTone(TONE_FREQUENCY_HIGH, TONE_DURATION_LONG);
  }
  else if (gasDanger)
  {
    buzzerTone(TONE_FREQUENCY_MID, TONE_DURATION_SHORT);
  }
  else if (tempDanger)
  {
    buzzerTone(TONE_FREQUENCY_HIGH, TONE_DURATION_SHORT);
  }
  else // No danger
  {
    buzzerTone(TONE_FREQUENCY_LOW, TONE_DURATION_SHORT);
  }

  tonesSinceLastUpdate = 1;
}

// Play different on the TONE_PIN according to the specified danger
void buzzerLoop()
{
  if (tonesSinceLastUpdate == 1 && (millis() - lastUpdateTime) > TONE_DURATION_LONG)
  {
    if (gasDanger && tempDanger)
    {
      buzzerTone(TONE_FREQUENCY_HIGH, TONE_DURATION_LONG);
    }
    else if (gasDanger)
    {
      buzzerTone(TONE_FREQUENCY_HIGH, TONE_DURATION_LONG);
    }
    else if (tempDanger)
    {
      buzzerTone(TONE_FREQUENCY_MID, TONE_DURATION_LONG);
    }
    else // No danger
    {
      // No second tone
    }
  }
}
