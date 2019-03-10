

byte first = 1;
int last[7];

void setupSpectrum() {
  //Setup pins to drive the spectrum analyzer. It needs RESET and STROBE pins.
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);

  //Init spectrum analyzer
  digitalWrite(4, LOW); //pin 4 is strobe on shield
  digitalWrite(5, HIGH); //pin 5 is RESET on the shield
  digitalWrite(4, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  setupSpectrum();

  Serial.begin(9600);
  Serial.println("Hello Rock'n Bot");
  test();
  test();
  test();
}

void test() {
  digitalWrite(10, HIGH);
  delay(250);
  digitalWrite(10, LOW);
  delay(250);
  digitalWrite(9, HIGH);
  delay(250);
  digitalWrite(9, LOW);
  delay(250);
  digitalWrite(8, HIGH);
  delay(500);
  digitalWrite(8, LOW);
  delay(500);
}

int getLoudest(int spectrum[7]) {
  byte loudestBand = 0;
  int  loudestValue = -1;

  for (byte band = 0; band < 7; band++) {
    if (spectrum[band] > loudestValue) {
      loudestBand = band;
      loudestValue = spectrum[band];
    }
  }
  return loudestBand;
}

void move(int loudest) {
  if (loudest == 0)
  {
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
  }
  else if (loudest == 1)
  {
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
  }
  else if (loudest == 2)
  {
    digitalWrite(8, HIGH);
  }
  else if (loudest == 3)
  {
    digitalWrite(9, HIGH);
  }
  else if (loudest == 4)
  {
    digitalWrite(10, HIGH);
  }
  else if (loudest == 5)
  {
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
  }
  else if (loudest == 6)
  {
    digitalWrite(8, HIGH);
    digitalWrite(10, HIGH);
  }
  delay(150);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}


// Function to read 7 band equalizers
void dance()
{
  // Spectrum analyzer read values will be kept here.
  int spectrum[7]; // Band 0 = Lowest Frequencies.

  // Get the reading from each of 7 channels each (left and right)
  byte Band;
  for (Band = 0; Band < 7; Band++)
  {
    spectrum[Band] = analogRead(0); //left
    + analogRead(1); //right
    digitalWrite(4, HIGH); //Strobe pin on the shield
    delay(2);
    digitalWrite(4, LOW);
  }

  int delta[7];
  if (first == 0) { // not the first time through
    // Figure out the change in values since last sample
    for (byte i = 0; i < 7; i++) {
      delta[i] = spectrum[i] - last[i];
    }

    int loudest = getLoudest(delta); // get channel with loudest change
    if (delta[loudest] > 400) {
      for (int i = 0; i < 7; i++) {
        Serial.print(delta[i]);
        Serial.print("\t");
      }
      Serial.print("Move: ");
      Serial.print(loudest);
      move(loudest); // move based on loudest
      Serial.println("");
    }
  }

  // remember the last spectrum
  for (byte i = 0; i < 7; i++) {
    last[i] = spectrum[i];
  }
  first = 0;
}

void loop() {
  dance();
}
