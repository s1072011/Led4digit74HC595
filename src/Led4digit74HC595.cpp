/*
  Led4digit74HC595.cpp - Library
*/

#include "Led4digit74HC595.h"
namespace {
uint8_t LED_CHAR(char c) {
  switch (c) {
    case '_' : return 0x08;
    case '^' : return 0x01; // ¯
    case '-' : return 0x40;
    case '*' : return 0x63; // °
    case ' ' : return 0x00; // space
    case 'A' : return 0x77; // upper case A
    case 'a' : return 0x5f; // lower case a
    case 'B' :              // lower case b
    case 'b' : return 0x7c; // lower case b
    case 'C' : return 0x39; // upper case C
    case 'c' : return 0x58; // lower case c
    case 'D' :              // lower case d
    case 'd' : return 0x5e; // lower case d
    case 'E' :              // upper case E
    case 'e' : return 0x79; // upper case E
    case 'F' :              // upper case F
    case 'f' : return 0x71; // upper case F
    case 'G' :              // upper case G
    case 'g' : return 0x35; // upper case G
    case 'H' : return 0x76; // upper case H
    case 'h' : return 0x74; // lower case h
    case 'I' : return 0x06; // 1
    case 'i' : return 0x04; // lower case i
    case 'J' : return 0x1e; // upper case J
    case 'j' : return 0x16; // lower case j
    case 'K' :              // upper case K
    case 'k' : return 0x75; // upper case K
    case 'L' :              // upper case L
    case 'l' : return 0x38; // upper case L
    case 'M' :              // twice tall n
    case 'm' : return 0x37; // twice tall ∩
    case 'N' :              // lower case n
    case 'n' : return 0x54; // lower case n
    case 'O' :              // lower case o
    case 'o' : return 0x5c; // lower case o
    case 'P' :              // upper case P
    case 'p' : return 0x73; // upper case P
    case 'Q' : return 0x7b; // upper case Q
    case 'q' : return 0x67; // lower case q
    case 'R' :              // lower case r
    case 'r' : return 0x50; // lower case r
    case 'S' :              // 5
    case 's' : return 0x6d; // 5
    case 'T' :              // lower case t
    case 't' : return 0x78; // lower case t
    case 'U' :              // lower case u
    case 'u' : return 0x1c; // lower case u
    case 'V' :              // twice tall u
    case 'v' : return 0x3e; // twice tall u
    case 'W' : return 0x7e; // upside down A
    case 'w' : return 0x2a; // separated w
    case 'X' :              // upper case H
    case 'x' : return 0x76; // upper case H
    case 'Y' :              // lower case y
    case 'y' : return 0x6e; // lower case y
    case 'Z' :              // separated Z
    case 'z' : return 0x1b; // separated Z
    case '0': return 0x3f;
    case '1': return 0x30;
    case '2': return 0x5b;
    case '3': return 0x4f;
    case '4': return 0x66;
    case '5': return 0x6d;
    case '6': return 0x7d;
    case '7': return 0x07;
    case '8': return 0x7f;
    case '9': return 0x6f;
  }
  return 0;
}
}

Led4digit74HC595::Led4digit74HC595(int SCLK, int RCLK, int DIO)
{
  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(DIO, OUTPUT);

  _SCLK = SCLK;
  _RCLK = RCLK;
  _DIO = DIO;

  _dp = 0;
  _char[0] = LED_CHAR(' ');
  _char[1] = LED_CHAR(' ');
  _char[2] = LED_CHAR(' ');
  _char[3] = LED_CHAR(' ');
  sleep();  //display dark
}

// Set number to be show in next step
void Led4digit74HC595::setNumber(int _n)
{
  if (_n > 9999) {
    for (char c = 0 ; c < 4; c++) {
      _char[c] = LED_CHAR('-');   //13=index for upper - char
    }
    return;
  }
  if (_n < -999) {
    for (char c = 0 ; c < 4; c++) {
      _char[c] = LED_CHAR('_');  //12=index for _ char
    }
    return;
  }

  int absNumber = abs(_n);
  int one, ten, hundred, thousand;
  one = absNumber % 10;
  ten = (absNumber / 10) % 10;
  hundred = (absNumber / 100) % 10;
  thousand = absNumber / 1000;

  _char[3] = LED_CHAR(thousand + 48);
  _char[2] = LED_CHAR(hundred + 48);
  _char[1] = LED_CHAR(ten + 48);
  _char[0] = LED_CHAR(one + 48);

  //show "nothing" on left leading positions when needed
  if (thousand == 0) {
    _char[3] = LED_CHAR(' ');        //10=index for nothing led segments
    if (hundred == 0) {
      _char[2] = LED_CHAR(' ');
      if (ten == 0) {
        _char[1] = LED_CHAR(' ');
      }
    }
  }

  //if negative number = show minus
  if (_n < 0) {
    if (absNumber > 99) {
      _char[3] = LED_CHAR('-');   //11=index for minus char -
    } else if (absNumber > 9) {
      _char[2] = LED_CHAR('-');
    } else if (absNumber > 0) {
      _char[1] = LED_CHAR('-');
    }
  }


  if (_dp == 1) _char[0] = _char[0] | B10000000;  //add decimal point
  if (_dp == 2) _char[1] = _char[1] | B10000000;
  if (_dp == 3) _char[2] = _char[2] | B10000000;
  if (_dp == 4) _char[3] = _char[3] | B10000000;
}
void Led4digit74HC595::setTemperature(float _n){
  int temp = int(_n*10);
  int one, ten, hundred;
  one = temp % 10;
  ten = (temp / 10) % 10;
  hundred = temp / 100;

  _char[3] = LED_CHAR(hundred + 48);
  _char[2] = LED_CHAR(ten + 48);
  _char[1] = LED_CHAR(one + 48);
  _char[0] = LED_CHAR('c');

  if (_dp == 1) _char[0] = _char[0] | B10000000;  //add decimal point
  if (_dp == 2) _char[1] = _char[1] | B10000000;
  if (_dp == 3) _char[2] = _char[2] | B10000000;
  if (_dp == 4) _char[3] = _char[3] | B10000000;
}
void Led4digit74HC595::setString(char *c) {
  uint16_t length = strlen(c);
  if (length <= 4) {
    char *_c;
    _c = new char[length];
    byte _length = length - 1;
    for (byte i = 0; i < length; i++) {
      _c[i] = c[_length];
      _length--;
    }
    for (byte i = 0; i < 4; i++) {
      if (i >= length) {
        _char[i] = LED_CHAR(' ');
      } else {
        _char[i] = LED_CHAR(_c[i]);
      }
    }
    delete []_c;
  } else {
    _char[3] = LED_CHAR('0');
    _char[2] = LED_CHAR('V');
    _char[1] = LED_CHAR('E');
    _char[0] = LED_CHAR('R');
  }

  if (_dp == 1) _char[0] = _char[0] | B10000000;  //add decimal point
  if (_dp == 2) _char[1] = _char[1] | B10000000;
  if (_dp == 3) _char[2] = _char[2] | B10000000;
  if (_dp == 4) _char[3] = _char[3] | B10000000;
}
void Led4digit74HC595::setString(char a,char b,char c,char d){
    _char[3] = a;
    _char[2] = b;
    _char[1] = c;
    _char[0] = d;
	if (_dp == 1) _char[0] = _char[0] | B10000000;  //add decimal point
  	if (_dp == 2) _char[1] = _char[1] | B10000000;
  	if (_dp == 3) _char[2] = _char[2] | B10000000;
  	if (_dp == 4) _char[3] = _char[3] | B10000000;
}
// Periodic call action in main loop of program
void Led4digit74HC595::loopShow()
{

  byte port = B0001 << _showPos;
  //writeByte(~_LED_CHAR[_char[_showPos]]);
  writeByte(~_char[_showPos]);    //Led bits for module must be negated (~)
  writeByte(port);
  digitalWrite(_RCLK, LOW);
  digitalWrite(_RCLK, HIGH);


  //move actual digit position
  if (_showPos < 3)
    _showPos++;
  else
    _showPos = 0;
}


void Led4digit74HC595::writeByte(unsigned char X)
{
  for (char i = 0; i < 8; i++)
  {
    //select left one bit and check if is 1 or 0
    if (X & B10000000) {
      digitalWrite(_DIO, HIGH);
    } else {
      digitalWrite(_DIO, LOW);
    }
    X <<= 1;
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);
  }
}


// Position of decimal point (1 - 4) or disable (0)
void Led4digit74HC595::setDecimalPoint(unsigned char position)
{
  if (position > 4) position = 4;
  _dp = position;
  for (char c = 0; c < 4; c++) {
    _char[c] = _char[c] & B01111111;    //remove decimal point
  }
  if (_dp == 1) _char[0] = _char[0] | B10000000;  //add decimal point
  if (_dp == 2) _char[1] = _char[1] | B10000000;
  if (_dp == 3) _char[2] = _char[2] | B10000000;
  if (_dp == 4) _char[3] = _char[3] | B10000000;
}


// All characters temporary off when program processing another long time function in multiplexing display characters
void Led4digit74HC595::sleep()
{
  char c;

  digitalWrite(_DIO, HIGH);   //segments off
  for (c = 0; c < 8; c++)
  {
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);
  }

  digitalWrite(_DIO, LOW);    //positions
  for (c = 0; c < 8; c++)
  {
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);
  }

  digitalWrite(_RCLK, LOW);   //write and active
  digitalWrite(_RCLK, HIGH);


  // temporary shot decimal point while display is darking
  digitalWrite(_DIO, LOW);    // DP segment, active = LOW
  digitalWrite(_SCLK, LOW);
  digitalWrite(_SCLK, HIGH);
  for (char i = 0; i < 7; i++) {
    digitalWrite(_DIO, HIGH);
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);
  }
  for (char i = 0; i < 4; i++) {  // non exist first 4 positions
    digitalWrite(_DIO, LOW);
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);
  }

  if (_dp == 4) {
    digitalWrite(_DIO, HIGH);   // 1 position = most left, active = HIGH
  } else {
    digitalWrite(_DIO, LOW);
  }
  digitalWrite(_SCLK, LOW);
  digitalWrite(_SCLK, HIGH);

  if (_dp == 3) {
    digitalWrite(_DIO, HIGH);   // 1 position = most left, active = HIGH
  } else {
    digitalWrite(_DIO, LOW);
  }
  digitalWrite(_SCLK, LOW);
  digitalWrite(_SCLK, HIGH);

  if (_dp == 2) {
    digitalWrite(_DIO, HIGH);   // 1 position = most left, active = HIGH
  } else {
    digitalWrite(_DIO, LOW);
  }
  digitalWrite(_SCLK, LOW);
  digitalWrite(_SCLK, HIGH);

  if (_dp < 2) {
    digitalWrite(_DIO, HIGH);   // 1 position = most left, active = HIGH
  } else {
    digitalWrite(_DIO, LOW);
  }
  digitalWrite(_SCLK, LOW);
  digitalWrite(_SCLK, HIGH);

  //--------------- write and show
  digitalWrite(_RCLK, LOW);
  digitalWrite(_RCLK, HIGH);
}


/* Test function for basic education
  void Led4digit74HC595::test()
  {
    // first byte (bit 1 - 8) = active segments when LOW
    // second byte (bit 9 -16) = active position when HIGH

    digitalWrite(_DIO, LOW);    // DP segment, active = LOW
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    // G segment
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    // F segment
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, HIGH);   // E segment
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, HIGH);   // D segment
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, HIGH);   // C segment
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    // B segment
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    // A segment
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    //-------
    digitalWrite(_DIO, LOW);    //
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    //
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    //
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    //
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, HIGH);   // 1 position = most left, active = HIGH
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    // 2 position
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, LOW);    // 3 position
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    digitalWrite(_DIO, HIGH);   // 4 position = most right
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);

    //--------------- write and show
    digitalWrite(_RCLK, LOW);
    digitalWrite(_RCLK, HIGH);
  }
*/
