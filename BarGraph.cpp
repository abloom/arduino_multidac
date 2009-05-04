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
// ldacPin  -> LDAC (13)
// BarGraph::BarGraph(int dataPin, int clockPin, int latchPin, int ldacPin)
BarGraph::BarGraph(int dataPin, int clockPin, int latchPin)
{
  // save our member data
  _dataPin = dataPin;
  _clockPin = clockPin;
  _latchPin = latchPin;
  // _ldacPin = ldacPin;

  // set the pins to output mode
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_latchPin, OUTPUT);
  // pinMode(_ldacPin, OUTPUT);

  // set the pins low
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_latchPin, LOW);
  // digitalWrite(_ldacPin, LOW);
}

    
// Light up a bar graph with value/255
//   graph  = 1 to 8
//   number = the number to write out
void BarGraph::writeValue(int graph, int number)
{
  digitalWrite(_latchPin, HIGH);
  // digitalWrite(_ldacPin, HIGH);
  // delay(_BAR_GRAPH_DELAY);

  _serializeValue(graph, number);

  digitalWrite(_latchPin, LOW);
  // delay(_BAR_GRAPH_DELAY);
  digitalWrite(_latchPin, HIGH);
  // digitalWrite(_ldacPin, LOW);
  // digitalWrite(_ldacPin, HIGH);
}

// Write the values of an array of specified size
//   arrayNums = the array of values
//   size      = the number to graphs
void BarGraph::writeArray(int* arrayNums, int size)
{
  int i = 0;

  for(i = size; i > 0; i--)
    writeValue(i, arrayNums[i-1]);
}

// Clear the graph
void BarGraph::clear(int graph)
{  
  digitalWrite(_latchPin, HIGH);
  // delay(_BAR_GRAPH_DELAY);
  
  _serializeValue(graph, 0);

  digitalWrite(_latchPin, LOW);
  // delay(_BAR_GRAPH_DELAY);
  digitalWrite(_latchPin, HIGH);
}

// Private function that sends a value out to the TLC5628C,
// writeValue must be 
void BarGraph::_serializeValue(int graph, int writeValue)
{
  int i = 0;
  int value = 0;
  int bitmask = 0x80; // binary 1000 0000
  
  _selectGraph(graph);

  // set data pin low
  digitalWrite(_dataPin, LOW);
  // Serial.println("data Bits: ");

  // send out the bits of the nibble MSB -> LSB
  for (i = 0; i < 8; i++)
  {
    digitalWrite(_clockPin, HIGH);
    // delay(_BAR_GRAPH_DELAY);
    
    // write bit to the data pin
    value = ((bitmask & writeValue) > 0 ? HIGH : LOW);
    digitalWrite(_dataPin, value);
    // Serial.print(value, BIN);
    // delay(_BAR_GRAPH_DELAY);
    
    digitalWrite(_clockPin, LOW);
    // delay(_BAR_GRAPH_DELAY);
    
    bitmask = bitmask >> 1;
  }

  // set the data low
  digitalWrite(_dataPin, LOW);
  // Serial.println("");
}

void BarGraph::_selectGraph(int graph)
{
  int i = 0;
  int value = 0;
  int bitmask = 0x4; // binary 0100

  // set data pin low
  digitalWrite(_dataPin, LOW);
  // Serial.println("control bits: ");
  
  // send out the 3 control bits
  for (i = 0; i < 3; i++)
  {
    digitalWrite(_clockPin, HIGH);
    // delay(_BAR_GRAPH_DELAY);
    
    // write bit to the data pin
    value = ((bitmask & (graph-1)) > 0 ? HIGH : LOW);
    digitalWrite(_dataPin, value);
    // Serial.print(value, BIN);
    // delay(_BAR_GRAPH_DELAY);
    
    digitalWrite(_clockPin, LOW);
    // delay(_BAR_GRAPH_DELAY);
    
    // shift the bitmask
    bitmask = bitmask >> 1;
  }

  digitalWrite(_clockPin, HIGH);
  // delay(_BAR_GRAPH_DELAY);

  // write multiplier bit to the data pin
  digitalWrite(_dataPin, HIGH);
  // Serial.println(HIGH, BIN);
  // delay(_BAR_GRAPH_DELAY);
  
  // set the clock high
  digitalWrite(_clockPin, LOW);
  // delay(_BAR_GRAPH_DELAY);
}
