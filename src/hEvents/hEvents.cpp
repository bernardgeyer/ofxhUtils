
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

#include "hEvents.h"

#include "hStringUtils.h"
#include <sstream>

#include "ofUtils.h"

hEvents * hEvents::_events =  0;
// Singleton initialization

// ---------------------------------------------------------------

void hEvents::setup(void)
{
    addListener("argTest", this, &hEvents::argTest);

    addListener("setValue", this, &hEvents::setValue);
    addListener("setValue2", this, &hEvents::setValue2);

    addListener("setXY", this, &hEvents::setXY);
    addListener("setTime", this, &hEvents::setTime);
	
    addListener("setLabel", this, &hEvents::setLabel);
    addListener("clearLabel", this, &hEvents::clearLabel);
	
    addListener("setText", this, &hEvents::setText);
    addListener("addText", this, &hEvents::addText);
    addListener("clearText", this, &hEvents::clearText);

	addListener("open", this, &hEvents::open);
	addListener("close", this, &hEvents::close);

	addListener("clear", this, &hEvents::clear);
	addListener("display", this, &hEvents::display);
	addListener("displayWOcr", this, &hEvents::displayWOcr);
	

    addListener("select", this, &hEvents::select);
    addListener("unselect", this, &hEvents::unselect);
    addListener("setSelected", this, &hEvents::setSelected);
	
	addListener("selectElement", this, &hEvents::selectElement);
	addListener("unselectElement", this, &hEvents::unselectElement);
	addListener("elementSetSelected", this, &hEvents::elementSetSelected);

	addListener("selectItem", this, &hEvents::selectItem);
	addListener("unselectItem", this, &hEvents::unselectItem);
	addListener("itemSetSelected", this, &hEvents::itemSetSelected);

    addListener("bang", this, &hEvents::bang);

// ----------------------------------
    
    addListener("setValueToItem", this, &hEvents::setValueToItem);
    addListener("setValue2ToItem", this, &hEvents::setValue2ToItem);
	
    addListener("openItem", this, &hEvents::openItem);
    addListener("closeItem", this, &hEvents::closeItem);
	
    addListener("answerDialog", this, &hEvents::answerDialog);

	addListener("start", this, &hEvents::start);
	addListener("stop", this, &hEvents::stop);
	addListener("cont", this, &hEvents::cont);

// ----------------------------------

	events_disabled = false;
    verbose = false;
}

// ---------------------------------------------------------------

void hEvents::addObject(std::string objectName, hObject * obj)
{
	hSymbol symbol = getSymbol("objects", objectName);

	if(symbol.ID != 0) {
		// cout << "error in hEvents::addObject: object " << objectName << " already in dictionary" << endl;
		return;
	}

	symbol = createSymbol("objects", objectName, H_OBJECT_SYMBOL);
	objectMap[symbol.ID] = obj;
}

hObject * hEvents::getObject(std::string objectName)
{
	hSymbol symbol = getSymbol("objects", objectName);
	
	if(symbol.ID == 0) {
		cout << "error in hEvents::getObject: unknown object \"" << objectName << "\" in object dictionary" << endl;
		return NULL;
	}
	
	if(symbol.type != H_OBJECT_SYMBOL) {
		cout << "error in hEvents::getObject: symbol " << objectName << " is not an object " << endl;
		return NULL;
	}
	
	// cout << "hEvents::getObject: found object: " << objectName << endl;
	return objectMap[symbol.ID];
}

bool hEvents::objectAvailable(std::string objectName)
{
	hSymbol symbol = getSymbol("objects", objectName);

	if(symbol.ID > 0)
		 return true;
	else return false;
}

// ---------------------------------------------------------------

hEvent * hEvents::addEvent(std::string dictName, std::string eventName)
{
	hSymbol symbol = getSymbol(dictName, eventName);

	if(symbol.ID == 0) {
        symbol = createSymbol(dictName, eventName, H_EVENT_SYMBOL);
        hEvent * e = new hEvent;
        eventMap[symbol.ID] = e;
	}

    return eventMap[symbol.ID];
}

hEvent * hEvents::addEvent(std::string eventName)
{
    return addEvent(selectedDictionary->name, eventName);
}

// ---------------------------------------------------------------

hEvent * hEvents::getEvent(std::string dictName, std::string eventName)
{
	hSymbol symbol = getSymbol(dictName, eventName);

	if(symbol.ID == 0) {
		cout << "error in hEvents::getEvent: unknown event \"" << eventName << "\" in " << dictName << " dictionary" << endl;
		return NULL;
	}

	if(symbol.type != H_EVENT_SYMBOL) {
		cout << "error in hEvents::getEvent: symbol " << eventName << " is not an event " << endl;
		return NULL;
	}

	// cout << "hEvents::getEvent: found event: " << eventName << endl;
	return eventMap[symbol.ID];
}

hEvent * hEvents::getEvent(std::string eventName)
{
    return getEvent(selectedDictionary->name, eventName);
}


// ---------------------------------------------------------------

void hEvents::disable(void)
{
    events_disabled = true;
}

void hEvents::enable(void)
{
    events_disabled = false;
}

// ---------------------------------------------------------------

void hEvents::set_verbose(bool flag)
{
    verbose = flag;
}

// ---------------------------------------------------------------

void hEvents::sendEvent(std::string dictName, std::string message, hEventArgs &args)
// The object is merely a kind of filter
// It is the role of the listener to check if the messages are for them
{
	if(events_disabled == true) return;

	// Try to split "message" into 2 strings for object and event
	string objectName, eventName;
	int splitPoint = message.find_first_of(".");
	
	if(splitPoint == -1) { // no dot found -> message is the name of the event
		objectName = "";
		eventName = message;
	}
	else {	// dot found -> name of the event = chars after dot
		objectName = message.substr(0, splitPoint);
		eventName = message.substr(splitPoint+1);
	}
	
	// cout << "message    = " << message    << ", splitPoint = " << splitPoint << endl;
	// cout << "objectName = " << objectName << ", eventName = "  << eventName << endl;

	// ----------------------------------------------------------------------
	
	// Check if a valid event exist. Return if not.
	hSymbol eventSymbol = getSymbol(dictName, eventName);
	
	if(eventSymbol.ID == 0) {
		// cout << "error in hEvents::sendEvent: unknown event \"" << eventName << "\" in " << dictName << " dictionary" << endl;
		return;
	}
	
	if(eventSymbol.type != H_EVENT_SYMBOL) {
		// cout << "error in hEvents::sendEvent: symbol " << eventName << " is not an event " << endl;
		return;
	}
	
	// ----------------------------------------------------------------------
	
	// Check if a valid object exist. Return if not.
	if(objectName.size() > 0) {
		hSymbol objectSymbol = getSymbol("objects", objectName);
		
		if(objectSymbol.ID == 0) {
			// cout << "error in hEvents::sendEvent: unknown object " << objectName << endl;
			return;
		}
		
		if(objectSymbol.type != H_OBJECT_SYMBOL) {
			// cout << "error in hEvents::sendEvent: " << objectName << " is not an object " << endl;
			return;
		}
	}
	
	// ----------------------------------------------------------------------
	
	// Store the object where to sent the even
	args.objectName = objectName;

	// Store the name of the event to be sent
	args.eventName = eventName;
	
	// Send the event
	ofNotifyEvent(*eventMap[eventSymbol.ID], args, this);
	
    if(verbose == true) {
        cout << "sending event ";
        cout << serialize(args, false, 15);
		if(objectName.size() > 0) cout << "to object " << objectName;
		cout << endl;
	}
}

void hEvents::sendEvent(std::string message, hEventArgs &args)
{
    sendEvent(selectedDictionary->name,message,args);
}

// ---------------------------------------------------------------
//           Sending event using different numbers of parameters:
// ---------------------------------------------------------------

void hEvents::sendEvent(std::string message){
	// No argument
    hEventArgs args;
    sendEvent(selectedDictionary->name, message, args);
}

// ------------------------------

void hEvents::sendEvent(std::string message, double val){
	// 1 number arg
    hEventArgs args;
    args.values.push_back(val);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, double val1, double val2){
	// 2 numbers args
    hEventArgs args;
    args.values.push_back(val1); args.values.push_back(val2);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, double val1, double val2, double val3){
	// 3 numbers args
    hEventArgs args;
    args.values.push_back(val1); args.values.push_back(val2); args.values.push_back(val3);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, double val1, double val2, double val3, double val4){
	// 4 numbers args
    hEventArgs args;
	args.values.push_back(val1); args.values.push_back(val2); args.values.push_back(val3); args.values.push_back(val4);
    sendEvent(selectedDictionary->name, message, args);
}

// ------------------------------

void hEvents::sendEvent(std::string message, std::ostringstream &oss){
	// Uses a string stream argument as string parameter
    hEventArgs args;
    args.strings.push_back(oss.str());
    sendEvent(selectedDictionary->name, message, args);
    oss.str("");
}

void hEvents::sendEvent(std::string message, std::string s){
	// String argument
    hEventArgs args;
    args.strings.push_back(s);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, std::string s, double val){
	// String and 1 number arg
    hEventArgs args;
    args.strings.push_back(s);
    args.values.push_back(val);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, std::string s, double val1, double val2){
	// String and 2 numbers args
    hEventArgs args;
    args.strings.push_back(s);
    args.values.push_back(val1); args.values.push_back(val2);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, std::string s, double val1, double val2, double val3){
	// String and 3 numbers args
    hEventArgs args;
    args.strings.push_back(s);
    args.values.push_back(val1); args.values.push_back(val2); args.values.push_back(val3);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, std::string s, double val1, double val2, double val3, double val4){
	// String and 4 numbers args
    hEventArgs args;
    args.strings.push_back(s);
	args.values.push_back(val1); args.values.push_back(val2); args.values.push_back(val3); args.values.push_back(val4);
    sendEvent(selectedDictionary->name, message, args);
}

// ---------------------------------------------------------------

void hEvents::sendEvent(std::string message, std::string s1, std::string s2){
	// String argument
    hEventArgs args;
    args.strings.push_back(s1);  args.strings.push_back(s2);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, std::string s1, std::string s2, double val){
	// String and 1 number arg
    hEventArgs args;
    args.strings.push_back(s1);  args.strings.push_back(s2);
    args.values.push_back(val);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, std::string s1, std::string s2, double val1, double val2){
	// String and 2 numbers args
    hEventArgs args;
    args.strings.push_back(s1);  args.strings.push_back(s2);
    args.values.push_back(val1); args.values.push_back(val2);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, std::string s1, std::string s2, double val1, double val2, double val3){
	// String and 3 numbers args
    hEventArgs args;
    args.strings.push_back(s1);  args.strings.push_back(s2);
    args.values.push_back(val1); args.values.push_back(val2); args.values.push_back(val3);
    sendEvent(selectedDictionary->name, message, args);
}

void hEvents::sendEvent(std::string message, std::string s1, std::string s2, double val1, double val2, double val3, double val4){
	// String and 4 numbers args
    hEventArgs args;
    args.strings.push_back(s1);  args.strings.push_back(s2);
	args.values.push_back(val1); args.values.push_back(val2); args.values.push_back(val3); args.values.push_back(val4);
    sendEvent(selectedDictionary->name, message, args);
}

// ---------------------------------------------------------------

std::string hEvents::serialize(hEventArgs& args, bool fixedNotation, int digits)
{
 	stringstream sstr;

 	sstr << args.eventName << " ";

    int str_size = args.strings.size();
    for(int i = 0; i < str_size; ++i) {
        sstr << args.strings[i] << " ";
	}

    int size = args.values.size();
    for(int i = 0; i < size; ++i) {
        if(fixedNotation == true)
                sstr << fixed << setprecision(digits) << args.values[i] << " " ;
        else sstr << setprecision(digits) << args.values[i] << " " ;
    }

    return sstr.str();
}

std::string hEvents::unserialize(std::string fullMessage, hEventArgs&args)
{
    vector <string> words = ofSplitString(fullMessage, " ");

    string message, word; bool numFlag, hexFlag;
    int number; double fnumber;

    int numWords = words.size();
    if(numWords > 0) {
      message = words[0];
        if(message[0] != 0) { // empty line hold 0
        args.eventName = message;
        // cout << "message: " << message << endl;

        if(numWords > 1) {
           for(int i = 1; i < numWords; ++i) {
                numFlag = false;
                hexFlag = false;
                word = words[i];

                if(isDecimal(word.c_str()))
                    numFlag = true;
                else if (isHexadecimal(word.c_str())) {
                    numFlag = true;
                    hexFlag = true;
               }

                if(numFlag == false) {
					args.strings.push_back(word);
                    // cout << "word: " << word << endl;
                }
                else {
                    istringstream iss(word);

                    if(hexFlag == false) {
                        iss >> dec >> fnumber;
                        // cout << "number: " << fnumber << endl;
                        args.values.push_back(fnumber);
                   }
                    else {
                        iss >> hex >> number;
                        // cout << "number: " << number << endl;
                        args.values.push_back((double)number);
                   }
                }
            }
        }
      } // if(message[0] != 0)
    } // numWords > 0

    return message;
}

// ---------------------------------------------------------------
//                          Standard Listeners:
// (used by the gui, but can be used by other objects too)
// ---------------------------------------------------------------

void hEvents::argTest(hEventArgs& args)
// Test Listener: display the arguments when the message "arg.test" is received
//
{
     cout << serialize(args, false, 15) << endl;
}

// ---------------------------------------------------------------

void hEvents::setValue(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->setValue(args.values[0]);
	}
}

void hEvents::setValue2(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->setValue2(args.values[0]);
	}
}

void hEvents::setXY(hEventArgs& args)
{	
	if(args.values.size() > 1) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->setXY(args.values[0], args.values[1]);
	}
}

void hEvents::setTime(hEventArgs& args)
{
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->setTime(args.values[0]);
	}
}

// ---------------------------------------------------------------

void hEvents::setLabel(hEventArgs& args)
{	
	if(args.strings.size() > 0) {
		string label = args.strings[0];
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->setLabel(label);
	}
}

void hEvents::clearLabel(hEventArgs& args)
{	
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->clearLabel();
}

// ---------------------------------------------------------------

void hEvents::setText(hEventArgs& args)
{	
	if(args.strings.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) {
			object->setText(args.strings[0]);
		}
	}
}

void hEvents::addText(hEventArgs& args)
{	
	if(args.strings.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) {
			object->addText(args.strings[0]);
		}
	}
}

void hEvents::clearText(hEventArgs& args)
{
	hObject * object = getObject(args.objectName);
	if(object != NULL) {
			 object->clearText();
	}
}

// ---------------------------------------------------------------

void hEvents::open(hEventArgs& args)
{		
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->open();
}


void hEvents::close(hEventArgs& args)
{		
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->close();
}


// ---------------------------------------------------------------

void hEvents::clear(hEventArgs& args)
{		
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->clear();
}


void hEvents::display(hEventArgs& args)
{
	if(args.strings.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) {
			object->display(args.strings[0]);
		}
	}
}

void hEvents::displayWOcr(hEventArgs& args)
{	
	if(args.strings.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) {
			object->displayWOcr(args.strings[0]);
		}
	}
}

// ---------------------------------------------------------------

void  hEvents::select(hEventArgs& args)
{
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->select();
}

void  hEvents::unselect(hEventArgs& args)
{
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->unselect();
}

void  hEvents::setSelected(hEventArgs& args)
{
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->setSelected(args.values[0]);
	}
}

// ---------------------------------------------------------------

void hEvents::selectElement(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->selectElement(args.values[0]);
	}
}

void  hEvents::unselectElement(hEventArgs& args)
{
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->unselectElement(args.values[0]);
}

void hEvents::elementSetSelected(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->elementSetSelected(args.values[0], args.values[1]);
	}
}

// ---------------------------------------------------------------

void hEvents::selectItem(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->selectItem(args.values[0]);
	}
}

void  hEvents::unselectItem(hEventArgs& args)
{
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->unselectItem(args.values[0]);
}

void hEvents::itemSetSelected(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->itemSetSelected(args.values[0], args.values[1]);
	}
}

// ---------------------------------------------------------------

void hEvents::bang(hEventArgs& args)
{	
	// cout << "hEvents::bang" << endl;
	
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->bang();
}


// ---------------------------------------------------------------
//                 Other Usefull Listeners:
// ---------------------------------------------------------------

void hEvents::setValueToItem(hEventArgs& args)
{	
	if(args.values.size() > 1) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->setValueToItem(args.values[0], args.values[1]);
	}
}

void hEvents::setValue2ToItem(hEventArgs& args)
{	
	if(args.values.size() > 1) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->setValue2ToItem(args.values[0], args.values[1]);
	}
}

// ---------------------------------------------------------------

void hEvents::openItem(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->openItem(args.values[0]);
	}
}

void hEvents::closeItem(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->closeItem(args.values[0]);
	}
}

void hEvents::answerDialog(hEventArgs& args)
{	
	if(args.values.size() > 0) {
		hObject * object = getObject(args.objectName);
		if(object != NULL) object->answerDialog(args.values[0]);
	}
}

// ---------------------------------------------------------------

void hEvents::start(hEventArgs& args)
{		
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->start();
}


void hEvents::stop(hEventArgs& args)
{		
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->stop();
}


void hEvents::cont(hEventArgs& args)
{		
	hObject * object = getObject(args.objectName);
	if(object != NULL) object->cont();
}


// ---------------------------------------------------------------

