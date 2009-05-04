/*
  BarGraph.h - A library to control TLC5628C driving up to 8 Nixie Bar Graphs
  Created by Andrew Bloom May 3rd 2009.
  Released into the public domain.
*/

// Include the standard types
#include <WProgram.h>
#include <WConstants.h>
#include "BarGraph.h"

// Constructor
// dataPin  -> DATA (4)
// clockPin -> CLK (5)
// latchPin -> LOAD (12)
BarGraph::BarGraph(int dataPin, int clockPin, int latchPin)
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
  digitalWrite(_dataPin, 0);
  digitalWrite(_clockPin, 0);
  digitalWrite(_latchPin, 0);
}

    
// Light up a bar graph with value/255
//   graph  = 1 to 8
//   number = the number to write out
void BarGraph::writeValue(int graph, int number)
{
  // Set the latch pin low
  digitalWrite(_latchPin, 0);

  _serializeValue(graph, number);

  // Set the latch pin high
  digitalWrite(_latchPin, 1);

  // Set the latch pin low
  digitalWrite(_latchPin, 0);
}

// Write the values of an array of specified size
//   arrayNums = the array of values
//   size      = the number to graphs
void BarGraph::writeArray(int* arrayNums, int size)
{
  int i = 0;
  
  // Set the latch pin low
  digitalWrite(_latchPin, 0);

  for(i = size; i > 0; i--)
    _serializeValue(i, arrayNums[i-1]);

  // Set the latch pin high
  digitalWrite(_latchPin, 1);

  // Set the latch pin low
  digitalWrite(_latchPin, 0);
}

// Clear the graph
void BarGraph::clear(int graph)
{  
  // Set the latch pin low
  digitalWrite(_latchPin, 0);

  _serializeValue(graph, 0);

  // Set the latch pin high
  digitalWrite(_latchPin, 1);

  // Set the latch pin low
  digitalWrite(_latchPin, 0);
}

// Private function that sends a value out to the TLC5628C,
// writeValue must be 
void BarGraph::_serializeValue(int graph, int writeValue)
{
  int i = 0;
  int bitmask = 0x80; // binary 1000 0000
  
  _selectGraph(graph);

  // set data pin low
  digitalWrite(_dataPin, 0);
  Serial.println("Data Bits: ");

  // send out the bits of the nibble MSB -> LSB
  for (i = 0; i < 8; i++)
  {
    // set clock low
    digitalWrite(_clockPin, 0);

    // write bit to the data pin
    digitalWrite(_dataPin, (bitmask & writeValue));
    Serial.print(bitmask & writeValue, BIN);
    
    // set the clock high
    digitalWrite(_clockPin, 1);
    
    // shift the bitmask
    bitmask = bitmask >> 1;
  }

  // set the data low
  digitalWrite(_dataPin, 0);

  // set the clock low
  digitalWrite(_clockPin, 0);
}

void BarGraph::_selectGraph(int graph)
{
  int i = 0;
  int bitmask = 0x4; // binary 0100

  // set data pin low
  digitalWrite(_dataPin, 0);
  Serial.println("control bits: ");
  
  // send out the 3 control bits
  for (i = 0; i < 3; i++)
  {
    // set clock low
    digitalWrite(_clockPin, 0);

    // write bit to the data pin
    digitalWrite(_dataPin, (bitmask & graph));
    Serial.print(bitmask & graph, BIN);
    
    // set the clock high
    digitalWrite(_clockPin, 1);
    
    // shift the bitmask
    bitmask = bitmask >> 1;
  }
  
  // write multiplier bit
  // set clock low
  digitalWrite(_clockPin, 0);

  // write bit to the data pin
  digitalWrite(_dataPin, LOW);
  Serial.println(LOW, BIN);
  
  // set the clock high
  digitalWrite(_clockPin, 1);
}
