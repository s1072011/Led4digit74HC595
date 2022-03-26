/*
  Led4digit74HC595.h - Library for use 7-segment led * 4 digits display with 2 74HC595 in multiplexing mode
  For china (ebay) arduino module 
  Created by Lubomir Hron, 3.11.2016
  Based on Kostarev V. - TM74HC595-4dig-display project	- thanks

  Show integer from -999 to +9999
  If number > 9999 display show upper ----
  If number < -999 display show ____
  Optional set decimal point on any position
  
  

      A
   -------
  |       |
 F|       |B
  |   G   |
   -------
  |       |
 E|       |C
  |       |
   -------  *DP
      D

Binary char data = DP G F E D C B A

To determine the code generator can be used at
http://www.uize.com/examples/seven-segment-display.html
The resulting code for programming must be negated
LOW bit = active lighting led segment

*/

#ifndef Led4digit74HC595_h
#define Led4digit74HC595_h
#include <Arduino.h>

class Led4digit74HC595
{
	public:
		Led4digit74HC595(int SCLK, int RCLK, int DIO);
		void setNumber(int n);								//main function = number to be show on display in periodical call show() [from -999 to +9999]
		void setTemperature(float n);
      void setString(const char *c);
      void set(char a,char b,char c,char d);
      void setScroller(byte c[],int length, unsigned int delay,byte mode);
      void setScroller(byte c[],int length, unsigned int delay, unsigned int space,byte mode);
      void rotate(unsigned int delay, byte mode);
      void loopShow();									//periodic call action in main loop of program and show muptiplexing number
		void setDecimalPoint(unsigned char position);		//set decimal poit disable (0) or enable on position (1-4)
		void sleep();										//temporary dark display if not multiplexed and wait for ending another long time operation
		//void test();
	
	private:
		void writeByte(unsigned char X);
		int _number;				//number to be show from -999 to 9999
		unsigned char _dp;			//flag of decimal poit disable (0) or enable on position (1-4)
		unsigned char _showPos;		//actual position of multiplexed number actual to be show
		int _SCLK;
		int _RCLK;
		int _DIO;
		uint8_t _char[4];				//prepared digits char index for to be show
      byte r[48]={0x01,0x01,0x01,0x01,
                  0x00,0x01,0x01,0x03,
                  0x00,0x00,0x01,0x07,
                  0x00,0x00,0x00,0x0f,
                  0x00,0x00,0x08,0x0e,
                  0x00,0x08,0x08,0x0c,
                  0x08,0x08,0x08,0x08,
                  0x18,0x08,0x08,0x00,
                  0x38,0x08,0x00,0x00,
                  0x39,0x00,0x00,0x00,
                  0x31,0x01,0x00,0x00,
                  0x21,0x01,0x01,0x00};
};
 
#endif
