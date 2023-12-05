#define XSTEP 9
#define XDIR 8
#define XSTOP 7
#define photoDiodePin A0

int fromPC = 0;
unsigned int sRange = 700, sRes = 4, delay_home = 3, delay_sense = 500;

void stepper(unsigned char rot_rsln)
{
  for (int i = 0; i < rot_rsln; i++)
  {
    digitalWrite(XSTEP, LOW);
    delayMicroseconds(delay_sense);
    digitalWrite(XSTEP, HIGH);
    delayMicroseconds(delay_sense);
  }
}
void homing(void)
{
  digitalWrite(XDIR, HIGH); //towards Limit Switch
  delay(10);
  while (digitalRead(XSTOP) == 1)
  {
    digitalWrite(XSTEP, LOW);
    delay(delay_home);
    digitalWrite(XSTEP, HIGH);
    delay(delay_home);
  }
}
void analyZinator(void)
{
  delay(500);
  int stp_tot = 0;
  while (stp_tot <= sRange)
  {
    digitalWrite(XDIR, LOW); //away from Limit Switch
    delay(10);
    stepper(sRes);
    stp_tot = stp_tot + sRes;

    int inten = 0; int sumInten = 0; int avgInten = 0;
    delay(20);
    for (int i = 1; i <= 200; i++) {
      delay(3);
      inten = 1024 - analogRead(photoDiodePin);
      sumInten = sumInten + inten;
    }
    avgInten = sumInten / 20;
    Serial.println(avgInten + 600);
    delay(20);
  }
}

void setup() {
  pinMode(XSTEP, OUTPUT);
  pinMode(XDIR, OUTPUT);
  pinMode(XSTOP, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.flush();
  homing();
  delay(100);
}

void loop() {
  delay(100);
  if (Serial.available() > 0) {
    delay(300);
    fromPC = Serial.read();

    if (fromPC == '1') {
      for (int k = 0; k < 15; k++) {
        delay(100);
        analyZinator();
        delay(500);
        Serial.println("exit_15");
        delay(200);
        homing();
      }
    }

    else if (fromPC == '2') {
      for (int k = 0; k < 1; k++) {
        delay(100);
        analyZinator();
        delay(500);
        Serial.println("exit_1");
        delay(200);
        homing();
      }
    }

  }
}
