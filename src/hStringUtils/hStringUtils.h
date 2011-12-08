
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

#ifndef _HSTRING_UTILS
#define _HSTRING_UTILS

#include <string>

std::string spacesToUnderscores(std::string str);
// Transform a string to a single word.
// Spaces are eventually replaces by underscores

std::string packMessage(std::string message, std::string param);
// Concat the parameter string to a message
// Used by hEvents::unserialize

bool isNumeric(const char* inputStr, int numberBase);
bool isDecimal(const char* inputStr);	  // can be int, float or double
bool isHexadecimal(const char* inputStr); // has to be an int starting with 0x or 0X

#endif // _HSTRING_UTILS
