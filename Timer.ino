#include <IRremote.h> //VERSION 2.7.0

#define BUZZER 4

#define RECV_PIN 3

IRrecv irrecv(RECV_PIN);

const int dataPin  = 5;  // DS
const int clockPin = 7; // SHCP
const int latchPin = 6; // STCP
const int digitPins[4] = {10, 11, 12, 13};  // DIG1–DIG4

// Codici segmenti per cifre da 0 a 9
const byte digitSegments[10] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111   // 9
};

int number = 0, tempN[4], pos = 0;

int volume = 4;
unsigned long lastToggle = 0, halfPeriod = 0, lastTick = 0, lastBlink = 0, currentDigit = 0, currentMillis = 0;

bool on = false, setTime = false, going = false, buzzerActive = false, displayVisible = true;

void displayNumber(int num) {
  static uint8_t currentDigit = 0;
  static unsigned long lastRefresh = 0;

  if (micros() - lastRefresh < 1000) return; // ogni 1000µs = 1ms
  lastRefresh = micros();

  // calcola le cifre
  uint8_t digits[4];
  digits[0] = num / 1000;
  digits[1] = (num / 100) % 10;
  digits[2] = (num / 10) % 10;
  digits[3] = num % 10;

  // spegni tutte le cifre
  for(int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH);

  // byte dei segmenti
  byte seg = digitSegments[digits[currentDigit]];
  if(currentDigit == 1) seg |= 0b10000000; // accendi DP

  // FAST shiftOut
  digitalWrite(latchPin, LOW);
  for(int i = 7; i >= 0; i--) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, (seg >> i) & 1);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);

  // accendi solo la cifra attuale
  if (!setTime || (setTime && (currentDigit != pos || displayVisible))) {
    digitalWrite(digitPins[currentDigit], LOW);
  }

  currentDigit = (currentDigit + 1) % 4;
}


void startBeep() {
  halfPeriod = 1000000L / (2 * volume * 50);
  buzzerActive = true;
}

void stopBeep() {
  buzzerActive = false;
  digitalWrite(BUZZER, LOW);
}

void updateBeep() {
  if (buzzerActive && (micros() - lastToggle >= halfPeriod)) {
    digitalWrite(BUZZER, !digitalRead(BUZZER));
    lastToggle = micros();
  }
}


void onOffDisplay() {
  on = (on) ? false : true;

  if(!on) {
    for (int j = 0; j < 4; j++) digitalWrite(digitPins[j], HIGH);

    going = false;
    setTime = false;
    buzzerActive = false;
    displayVisible = true;
    number = 0;
  }
}

void pauseTime() {
  going = (going) ? false : true;

  if(number == 0) stopBeep();
}

void volUp() {
  if(volume < 30) {
    volume++;

    Serial.print("Vol: ");
    Serial.println(volume * 500);
  }
  if(buzzerActive) startBeep();
}

void volDown() {
  if(volume > 0) {
    volume--;

    Serial.print("Vol: ");
    Serial.println(volume * 500);
  }
  if(buzzerActive) startBeep();
}

void setValue(unsigned long value) {
  int n = -1;

  switch(value) {
    case 0xE916FF00: n = 0; break;
    case 0xF30CFF00: n = 1; break;
    case 0xE718FF00: n = 2; break;
    case 0xA15EFF00: n = 3; break;
    case 0xF708FF00: n = 4; break;
    case 0xE31CFF00: n = 5; break;
    case 0xA55AFF00: n = 6; break;
    case 0xBD42FF00: n = 7; break;
    case 0xAD52FF00: n = 8; break;
    case 0xB54AFF00: n = 9; break;
  }

  if(pos == 2 && n > 5) return;

  if (n != -1 && pos >= 0 && pos < 4) {
    tempN[pos] = n;
    number = tempN[0]*1000 + tempN[1]*100 + tempN[2]*10 + tempN[3];
    Serial.print("Set digit at pos "); Serial.print(pos);
    Serial.print(" to "); Serial.println(n);
    pos = (pos + 1) % 4;
    Serial.print("Next pos: "); Serial.println(pos);
  } else {
    Serial.print("Invalid value or pos: n=");
    Serial.print(n);
    Serial.print(", pos=");
    Serial.println(pos);
  }
}


void setup(){
  Serial.begin(9600);

  for(int i=0; i<4; i++) tempN[i]=0;

  for(int i=0; i<4; i++) pinMode(digitPins[i], OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  for (int j = 0; j < 4; j++) digitalWrite(digitPins[j], HIGH);

  irrecv.enableIRIn();
}

void loop(){
  if(irrecv.decode()){
    unsigned long val = irrecv.decodedIRData.decodedRawData;

    if(val != 0xFFFFFFFF && val != 0) {

      bool validCodeIR = false;
      switch(val) {
        case 0xBA45FF00: case 0xB946FF00: case 0xB847FF00: case 0xBB44FF00: case 0xBF40FF00: case 0xBC43FF00: case 0xF807FF00:
        case 0xEA15FF00: case 0xF609FF00: case 0xE916FF00: case 0xE619FF00: case 0xF20DFF00: case 0xF30CFF00: case 0xE718FF00:
        case 0xA15EFF00: case 0xF708FF00: case 0xE31CFF00: case 0xA55AFF00: case 0xBD42FF00: case 0xAD52FF00: case 0xB54AFF00:
          validCodeIR = true;

          Serial.print("Received code: ");
          Serial.println(val, HEX);
          break;
        
        default:
          Serial.print("Codice IR non valido: ");
          Serial.println(val, HEX);
          break;
      }

      if(validCodeIR) {
        if(val == 0xBA45FF00) {
         onOffDisplay();
        } else if(on) {
      
          if(setTime) {
            if(val == 0xF807FF00) { //down
              pos = (pos + 3) % 4; //-1 + 4
            } else if(val == 0xF609FF00) { //up
              pos = (pos + 1) % 4;
            } else if(!((val == 0xB946FF00) || (val == 0xBB44FF00) || (val == 0xBF40FF00) || (val == 0xBC43FF00) || (val == 0xEA15FF00) || (val == 0xE619FF00) || (val == 0xF20DFF00))) { //numeri
              setValue(val);
            }
          } else {
            if(val == 0xBF40FF00) pauseTime();
            else if(val == 0xB946FF00) volUp();
            else if(val == 0xEA15FF00) volDown();
            else if(val == 0xE619FF00) for(int i=0; i<20; i++) Serial.println();
          }

          if(val == 0xF20DFF00) {
            setTime = (setTime) ? false : true;
            pos = 0;

            if(setTime) {
              for(int i=0; i<4; i++) tempN[i]=0;
              number = 0;
              stopBeep();
            }
          }
        }
      }
    }

    irrecv.resume();
    delay(10);
  }

  if(on || going) currentMillis = millis();

  if(on) {
    if(setTime && currentMillis - lastBlink >= 500) {
        displayVisible = !displayVisible;
        lastBlink = currentMillis;
    } 
    
    displayNumber(number);
  }

  if(going) {
    if(number == 0) {
      if(buzzerActive) updateBeep();
      else startBeep();
    } else if(currentMillis - lastTick >= 1000) {
      if(number % 100 == 0) number -= 40;

      number--;
      lastTick = currentMillis;
    }
  }
}
