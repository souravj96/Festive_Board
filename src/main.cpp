#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 11);

const byte latchPin = 3;
const byte dataPin = 2;
const byte clockPin = 4;

const int digitsPins[] = {
    5, 6, 7};

String receivedData = "";

const byte space = B11111111;

const byte numbers[] =
    {
        B00000010, // 0
        B10011110, // 1
        B00100101, // 2
        B00001101, // 3
        B10011001, // 4
        B01001001, // 5
        B01000001, // 6
        B00011111, // 7
        B00000000, // 8
        B00001001, // 9
};

const byte letters[] =
    {
        B00010001, // A
        B11000001, // B
        B01100011, // C
        B10000101, // D
        B01100001, // E
        B01110001, // F
        B01000011, // G
        B10010001, // H
        B11001111, // I
        B10000111, // J
        B01010001, // K
        B11100011, // L
        B00010011, // M
        B00010011, // N (same as M)
        B00000011, // O
        B00110001, // P
        B00001011, // Q
        B00110011, // R
        B01001001, // S
        B11110001, // T
        B10000011, // U
        B10001011, // V
        B01000011, // W
        B01101101, // X
        B01100101, // Y
        B00101001  // Z
};

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (int i = 0; i < sizeof(digitsPins) / sizeof(digitsPins[0]); i++)
  {
    pinMode(digitsPins[i], OUTPUT);
  }
}

void showNumber(int number)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, numbers[number]);
  digitalWrite(latchPin, HIGH);
}

void showLetter(char letter)
{
  Serial.print(letter);
  if (letter >= 'A' && letter <= 'Z')
  {
    int index = letter - 'A';
    Serial.println(index);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, letters[index]);
    digitalWrite(latchPin, HIGH);
  }
  else if (letter >= '0' && letter <= '9')
  {
    int index = letter - '0';
    Serial.println(index);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, numbers[index]);
    digitalWrite(latchPin, HIGH);
  }
  else
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, space);
    digitalWrite(latchPin, HIGH);
  }
}

void showText(String text)
{
  for (int i = 0; i < text.length(); i++)
  {
    for (int k = 0; k < 50; k++)
    {
      for (int j = 0; j < sizeof(digitsPins) / sizeof(digitsPins[0]); j++)
      {
        showLetter(text[i + j]);
        digitalWrite(digitsPins[j], HIGH);
        delay(3);
        digitalWrite(digitsPins[j], LOW);
      }
    }
  }
}

void loop()
{

  if (mySerial.available() > 0)
  {
    receivedData = "";
    while (mySerial.available() > 0)
    {
      char receivedChar = mySerial.read();
      receivedData += receivedChar;
    }
  }

  showText(String(receivedData));
}

// echo -n -e '1154' > /dev/tty.HC-05