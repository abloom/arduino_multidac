/*
  BarGraph.h - A library to control TLC5628C driving up to 8 Nixie Bar Graphs
  Created by Andrew Bloom May 3rd 2009.
  Released into the public domain.
*/

#ifndef BarGraph_h
#define BarGraph_h

// Include the standard types
#include <WProgram.h>
#include <WConstants.h>

// Define the BarGraph class
class BarGraph
{
  public:
    // Constructor
    // dataPin  -> DATA (4)
    // clockPin -> CLK (5)
    // latchPin -> LOAD (12)
    BarGraph(int dataPin, int clockPin, int latchPin);
    
    // Write a full number, left justified, pushing digits to the right
    //   graph  = 1 to 8
    //   number = the number to write out
    void writeValue(int graph, int number);
    
    // Write the values of an array of specified size
    //   arrayNums = the array of values
    //   size      = the number to graphs
    void writeArray(int* arrayNums, int size);

    // Clear the graph
    void clear(int graph);


  private:
    int _dataPin;
    int _clockPin;
    int _latchPin;
    void _serializeValue(int graph, int writeValue);
    void _selectGraph(int graph);

};

#endif //BarGraph_h

