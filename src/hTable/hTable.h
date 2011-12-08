
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

#ifndef _HTABLE
#define _HTABLE

#include <vector>

class hTable {
	// hTable keep a list of values stored in a vector<double> in ascending order
public:
	hTable();

    int getSize(void);
	// Return the number of values in the table

    void addValue(double value);
	// Add a new value to the table
	// Warning : the values of the table have to be entered in ascending order
	
    void changeValue(int index, double value);
	// Change an existing value
	// index starts with #0
	// Shoud be used only if there is no other choice
	// Warning : the values of the table have to stay in ascending order

    double getValue(int index);
	// Get the value at a specific index
	// index starts with #0

    double getNearestValue(double value);
	// Compare a value to the values in the table
	// and return the nearest value

    double getMinValue(void);
    double getMaxValue(void);

    void display(void);
	// Just for tests, display the table to the console

private:
    void recalculateMinMax(void);
	// Helper function: must recalculate min/max when a data value is changed

    double min, max;
	std::vector<double> data;
};

#endif // _HTABLE
