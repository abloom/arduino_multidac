/*
  MultiDAC.h - A library to control TLC5628C driving up to 8 Nixie Bar Graphs
  Created by Andrew Bloom May 3rd 2009.
  Released into the public domain.
*/

#ifndef MultiDAC_h
#define MultiDAC_h

// Include the standard types
#include <WProgram.h>
#include <WConstants.h>

#define _BAR_GRAPH_DELAY 1

// Define the MultiDAC class
class MultiDAC
{
  public:
    // Constructor
    // dataPin  -> DATA (4)
    // clockPin -> CLK (5)
    // latchPin -> LOAD (12)
    MultiDAC(int dataPin, int clockPin, int latchPin);
    
    // Send an output signal to the DAC
    //   dac  = 1 to 8
    //   number = the number to write out (0-254)
    void writeValue(int dac, int number);
    
    // Write the values of an array of specified size
    //   arrayNums = the array of values
    //   size      = the number to DACs
    void writeArray(int* arrayNums, int size);

    // Clear the DAC (0 output)
    void clear(int dac);


  private:
    int _dataPin;
    int _clockPin;
    int _latchPin;
    void _serializeValue(int dac, int writeValue);
    void _selectDAC(int dac);

};

#endif //MultiDAC_h

