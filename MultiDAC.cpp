/*
  MultiDAC.h - A library to control a TLC5628C, an 8 port serial DAC
  Created by Andrew Bloom May 3rd 2009.
  Released into the public domain.
*/

// Include the standard types
#include <WProgram.h>
#include <WConstants.h>
#include "MultiDAC.h"

// Constructor
// dataPin  -> DATA (4)
// clockPin -> CLK (5)
// latchPin -> LOAD (12)
MultiDAC::MultiDAC(int dataPin, int clockPin, int latchPin)
{
  // save our member data
  _dataPin = dataPin;
  _clockPin = clockPin;
  _latchPin = latchPin;

  // set the pins to output mode
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_latchPin, OUTPUT);

  // set the pins low
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_latchPin, LOW);
}

    
// Send an output signal to the DAC
//   dac  = 1 to 8
//   number = the number to write out (0-254)
void MultiDAC::writeValue(int dac, int number)
{
  digitalWrite(_latchPin, HIGH);

  _serializeValue(dac, number);

  digitalWrite(_latchPin, LOW);
  digitalWrite(_latchPin, HIGH);
}

// Write the values of an array of specified size
//   arrayNums = the array of values
//   size      = the number to DACs
void MultiDAC::writeArray(int* arrayNums, int size)
{
  int i = 0;

  for(i = size; i > 0; i--)
    writeValue(i, arrayNums[i-1]);
}

// Clear the DAC (0 output)
void MultiDAC::clear(int dac)
{  
  writeValue(dac, 0);
}

// Private function that sends a value out to the TLC5628C,
void MultiDAC::_serializeValue(int dac, int writeValue)
{
  int i = 0;
  int value = 0;
  int bitmask = 0x80; // binary 1000 0000
  
  _selectDAC(dac);

  // set data pin low
  digitalWrite(_dataPin, LOW);

  // send out the bits of the nibble MSB -> LSB
  for (i = 0; i < 8; i++)
  {
    digitalWrite(_clockPin, HIGH);
    
    // write bit to the data pin
    value = ((bitmask & writeValue) > 0 ? HIGH : LOW);
    digitalWrite(_dataPin, value);

    digitalWrite(_clockPin, LOW);
    
    bitmask = bitmask >> 1;
  }

  // set the data low
  digitalWrite(_dataPin, LOW);
}

void MultiDAC::_selectDAC(int dac)
{
  int i = 0;
  int value = 0;
  int bitmask = 0x4; // binary 0100

  // set data pin low
  digitalWrite(_dataPin, LOW);
  
  // send out the 3 control bits
  for (i = 0; i < 3; i++)
  {
    digitalWrite(_clockPin, HIGH);
    
    // write bit to the data pin
    value = ((bitmask & (dac-1)) > 0 ? HIGH : LOW);
    digitalWrite(_dataPin, value);
    
    digitalWrite(_clockPin, LOW);
    
    // shift the bitmask
    bitmask = bitmask >> 1;
  }

  digitalWrite(_clockPin, HIGH);

  // write multiplier bit to the data pin
  digitalWrite(_dataPin, HIGH);

  // set the clock high
  digitalWrite(_clockPin, LOW);
}
