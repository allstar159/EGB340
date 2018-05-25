#define TONE_PIN 8 // Pin for positive connection of buzzer
#define TONE_CYCLE_TIME 4000
#define TONE_DURATION_SHORT 500
#define TONE_DURATION_MID 2000
#define TONE_DURATION_LONG 3500
#define TONE_FREQUENCY_LOW 2500
#define TONE_FREQUENCY_MID 3500
#define TONE_FREQUENCY_HIGH 4500



// Play different on the TONE_PIN according to the specified danger
void buzzerAlert(bool gasDanger, bool tempDanger)
{
  if (gasDanger && tempDanger)
  {
    tone(TONE_PIN, TONE_FREQUENCY_HIGH, TONE_DURATION_LONG);
  }
  else if (gasDanger)
  {
    tone(TONE_PIN, TONE_FREQUENCY_HIGH, TONE_DURATION_SHORT);
  }
  else if (tempDanger)
  {
    if(millis() - lastToneTime > UPDATE_INTERVAL)
    {
      tone(TONE_PIN, TONE_FREQUENCY_MID, TONE_DURATION_MID);
    }
  }
  else // No danger
  {
    if (millis() - lastToneTime > UPDATE_INTERVAL)
    {
      tone(TONE_PIN, TONE_FREQUENCY_LOW, TONE_DURATION_SHORT);
    }
  }
}
