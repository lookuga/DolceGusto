int powerSwitchBtn = 2; //need a counter to see which step? or we have a held/press time (if long press turn off ex)
int powerSwitchGreen = 3; //PWM
int powerSwitchRed = 5; //PWM -- change wire to 5 as it is 4 atm without PWM

int capPodSensor = 6;

int pullLeverCold = 8;
int pullLeverHot = 9;

int heater = 10;
int pump = 11;

int tempSensor = A4;

//http://computers.tutsplus.com/tutorials/how-to-read-temperatures-with-arduino--mac-53714
int IdealTemp = 300; //400 //????need to calibrate this..

void setup() {

  pinMode(powerSwitchBtn, INPUT);
  pinMode(powerSwitchGreen, OUTPUT);
  pinMode(powerSwitchRed, OUTPUT);

  pinMode(capPodSensor, INPUT_PULLUP);

  pinMode(pullLeverCold, INPUT_PULLUP);
  pinMode(pullLeverHot, INPUT_PULLUP);

  pinMode(heater, OUTPUT);
  pinMode(pump, OUTPUT);

  digitalWrite(powerSwitchRed, HIGH);
  digitalWrite(powerSwitchGreen, HIGH);

  digitalWrite(heater, HIGH);
  digitalWrite(pump, HIGH);

  Serial.begin(9600);

}

int StandByBrightness = 0;    // how bright the LED is
int StandByFadeAmount = 5;    // how many points to fade the LED by

int brightness = 0;    // how bright the LED is
int fadeAmount = 10;    // how many points to fade the LED by


bool notInUse = true;



int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 5;    // the debounce time; increase if the output flickers

bool pullLever = false;
void loop() {
  /*
  Workflow:
  once pluged in wall -- will be on standby.. do PWM red/green dance
  Turn on
  check if pod is in - stay red - do not continue
  Start heat up of heater - flash yellow
  Turn green once heated to correct temp (resistance)
  Pull lever starts pump - cold/hot (need to check how to pump either of them ?)

  Perhaps we should pulse the leds instead? need to check about if available on attiny?
  Standby mode?
  */
  int reading = !digitalRead(powerSwitchBtn);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        //ledState = !ledState;
        Serial.println("ON");
        notInUse = !notInUse;
      }
    }
  }

  // set the LED:
  //digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

  //  if (digitalRead(powerSwitchBtn) == HIGH)
  //  {
  //    Serial.println("ON");
  //    notInUse = !notInUse;
  //  }

  if (notInUse)
  {
    digitalWrite(powerSwitchRed, HIGH);
    // digitalWrite(powerSwitchGreen, HIGH);

    digitalWrite(heater, HIGH);
    digitalWrite(pump, HIGH);


    //Serial.println("Standby");
    // set the brightness of pin 9:
    analogWrite(powerSwitchGreen, StandByBrightness);

    // change the brightness for next time through the loop:
    StandByBrightness = StandByBrightness + StandByFadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (StandByBrightness == 0 || StandByBrightness == 255) {
      StandByFadeAmount = -StandByFadeAmount ;
    }

    delay(30);
  } else {
    pullLever = !digitalRead(pullLeverHot) || !digitalRead(pullLeverCold);
    int temp = analogRead(tempSensor);
    
    if (digitalRead(pullLeverCold))
    {
      Serial.println(temp);
      delay(100);
      //Serial.println((IdealTemp + 5));
      if (temp < IdealTemp)
      {

        Serial.println("TEMP LOW");
        digitalWrite(heater, LOW);
        analogWrite(powerSwitchRed, brightness);
        analogWrite(powerSwitchGreen, brightness);
        //delay(60);
        // change the brightness for next time through the loop:
        brightness = brightness + fadeAmount;

        // reverse the direction of the fading at the ends of the fade:
        if (brightness == 0 || brightness == 240) {
          fadeAmount = -fadeAmount ;
        }

        //digitalWrite(powerSwitchGreen, LOW);
        //digitalWrite(powerSwitchRed, LOW);
      } else if (temp > (IdealTemp + 10))
      {
        Serial.println("TEMP PERFECT");
        digitalWrite(heater, HIGH);

        digitalWrite(powerSwitchGreen, LOW);
        digitalWrite(powerSwitchRed, HIGH);
      }
    }

    if (pullLever)
    {
      Serial.println("LEVEL PULL");

      //digitalWrite(powerSwitchGreen, !digitalRead(powerSwitchBtn));
      // if no pod holder flash red...
      if (digitalRead(capPodSensor))
      {
        Serial.println("CAP OPEN");
        digitalWrite(powerSwitchRed, LOW);
        digitalWrite(powerSwitchGreen, HIGH);
      } else
      {
        
        Serial.println("START PUMP");
        digitalWrite(powerSwitchRed, LOW);
        digitalWrite(powerSwitchGreen, LOW);
        digitalWrite(pump, LOW);
      }
    } else
    {
      // Serial.println("STOP PUMP");
      digitalWrite(pump, HIGH);
    }

  }
}

