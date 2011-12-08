
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

#include "hStringUtils.h"

#include "string.h"
#include <sstream>
using namespace std;

//--------------------------------------------------------------

std::string spacesToUnderscores(std::string str)
{
	int size = str.size();
    for(int i = 0; i < str.size(); ++i)
        if(str[i] == ' ') str[i] = '_';
	
    return str;
}

std::string packMessage(std::string message, std::string param)
{
    message += " ";
    message += param;
	
    return message;
}

//--------------------------------------------------------------

bool isNumeric(const char* inputStr, int numberBase)
{
	istringstream iss( inputStr );
	
	if ( numberBase == 10 )
	{
		double dTestSink;
		iss >> dTestSink;
	}
	else if ( numberBase == 8 || numberBase == 16 )
	{
		int nTestSink;
		iss >> ( ( numberBase == 8 ) ? oct : hex ) >> nTestSink;
	}
	else
		return false;
	
	// was any input successfully consumed/converted?
	if ( ! iss )
		return false;
	
	// was all the input successfully consumed/converted?
	return ( iss.rdbuf()->in_avail() == 0 );
}

bool isDecimal(const char* inputStr)
{
    return isNumeric(inputStr, 10);
}

bool isHexadecimal(const char* inputStr)
{
    bool numFlag = isNumeric(inputStr, 16);
    bool hexFlag = false;
	
    if(numFlag == true) {
        if(strlen(inputStr) > 2)
            if((inputStr[1] == 'x') || (inputStr[1] == 'X'))
                hexFlag = true;
		
        return hexFlag;
    }
    else return false;
}

//--------------------------------------------------------------
