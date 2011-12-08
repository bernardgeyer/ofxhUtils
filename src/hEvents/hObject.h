
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

#ifndef _HOBJECT
#define _HOBJECT

#include <string>
#include <vector>
#include <map>

//--------------------------------------------------------------
//			BASE OBJECT  for the most used message listeners
//--------------------------------------------------------------

class hObject
{
public:
	virtual void setValue(double val){}
	virtual void setValue2(double val){}
	
	virtual void setXY(double x, double y){}
	virtual void setTime(double val){}
	
	virtual void setLabel(std::string label){}
	virtual void clearLabel(void){}
	
	virtual void setText(std::string text){}
	virtual void addText(std::string text){}
	virtual void clearText(void){}
	
	virtual void open(void){}
	virtual void close(void){}
	
	virtual void clear(void){}
	virtual void display(std::string str){}
	virtual void displayWOcr(std::string str){}

	virtual void select(void){}
	virtual void unselect(void){}
	virtual void setSelected(bool selFlag){}

	virtual void selectElement(int item){}
	virtual void unselectElement(int item){}
    virtual void elementSetSelected(int item, bool flag){}

	virtual void selectItem(int item){}
	virtual void unselectItem(int item){}
    virtual void itemSetSelected(int item, bool flag){}
	
	virtual void bang(void){}

//------------------------------------
	
	virtual void setValueToItem(double val,  int index){}
	virtual void setValue2ToItem(double val, int index){}

	virtual void openItem(int index){}
	virtual void closeItem(int index){}
	
	virtual void answerDialog(int buttonID){}

	virtual void start(void){}
	virtual void stop(void){}
	virtual void cont(void){}
	
};

#endif // _HOBJECT
