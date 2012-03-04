
/*****************************************************************************
 
 Copyright (C) 2011 by Bernard Geyer
 
 http://bernardgeyer.com/
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *****************************************************************************/

#include "hTable.h"

#include <iostream>
using namespace std;

#if !defined( __APPLE_CC__ )
#include <float.h>
#endif

hTable::hTable()
{
    min = FLT_MAX;
    max = FLT_MIN;
}

int hTable::getSize(void)
{
    return data.size();
}

//--------------------------------------------------------------

void hTable::addValue(double value)
{
    if(value < min) min = value;
    if(value > max) max = value;

    data.push_back(value);
}

void hTable::changeValue(int index, double value)
{
    int maxIndex = data.size() - 1;
    if(maxIndex < 0) return;

    if(index < 0)             index = 0;
    else if(index > maxIndex) index = maxIndex;

    data[index] = value;
    recalculateMinMax();
}

//--------------------------------------------------------------

double hTable::getValue(int index)
{
    int maxIndex = data.size() - 1;
    if(maxIndex < 0) return 0.0;

    if(index < 0)             index = 0;
    else if(index > maxIndex) index = maxIndex;

    return data[index];
}

double hTable::getNearestValue(double value)
// Compare a value to the values in the table
// and return the nearest value
{	
	int size = data.size();		// actual size of the table
	double nearestValue = 0.0;   // default value = 0.0, used when the table is empty
	
    if(size > 0) {
		if(value >= data[size -1]) {
			nearestValue = data[size -1];
		}
		
        else {
			for(int i = 0; i < size; ++i) {
				
				if(value == data[i]) {
					nearestValue = value;
					break;
				}
				
				else if(value < data[i]){
					if(i == 0) nearestValue = data[0];
					else       nearestValue = data[i-1];
					break;
				}
				
			} // <- for ...
			
		} // <- first else
		
    } // <- if(size > 0)
	
	// cout << "value = " << value << " nearestValue = " << nearestValue << endl;
	
	return nearestValue;
}


//--------------------------------------------------------------

double hTable::getMinValue(void)
{
    return min;
}

double hTable::getMaxValue(void)
{
    return max;
}

//--------------------------------------------------------------

void hTable::display(void)
{
    cout << "Table: " << getSize() << " elements" << endl;
    cout << "min value = " << min << ", ";
    cout << "max value = " << max << endl;

    int size = getSize();
    for(int i = 0; i < size; ++i) {
        cout << i << ": " << getValue(i) << endl;
    }
}

//--------------------------------------------------------------
//							Helper function
//--------------------------------------------------------------

void hTable::recalculateMinMax(void)
{
    min = FLT_MAX;
    max = FLT_MIN;

	int size = data.size();
    for(int i = 0; i < size; ++i) {
        if(data[i] < min) min = data[i];
        if(data[i] > max) max = data[i];
    }
}

//--------------------------------------------------------------
