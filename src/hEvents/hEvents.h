
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

#ifndef _HEVENTS
#define _HEVENTS

#include "hSymbols.h"
#include "hObject.h"
#include "ofEvents.h"

//--------------------------------------------------------------
//							HARFANG EVENTS
//--------------------------------------------------------------

// harfang objects use ofEvents to communicate.
// A sender and a receiver don't need to know each other:
// This is the so-called publish-subscribe pattern.
// The sender has just to notify that he sends an event with specific arguments
// and the receiver has just to register to the events that he wants to use (generally done in the constuctor of the object)

// More than one object can register to the same event
// In this way, an object can easyly be replaced by another without the need to modify anything else.
// Test objects can also be implemented as "mock objects" for testing objects that don't exists yet or for debuging.

//--------------------------------------------------------------

struct hEventArgs : public ofEventArgs {
// We use a vector of doubles and a string to represent various arguments
	string objectName;		// the name of the object (empty means any object)
	string eventName;       // the name of the event
	vector<string> strings; // string parameters
	vector<double> values;  // numerical parameters
};

typedef ofEvent<hEventArgs>  hEvent;
// This is our event type

//--------------------------------------------------------------

class hEvents : public hSymbols {
// singleton class hEvents
	
private:
	static hEvents * _events;
	
	// Private constructor / destructor :
    hEvents()  {}
	~hEvents() {}
	
	std::map < unsigned int, hObject * > objectMap;
	// This map hold the objects to which to send when the corresponding string is found in it
	
	std::map < unsigned int, ofEvent<hEventArgs>* > eventMap;
	// This map hold the events to send when the corresponding string is found in it
	
public :
	// Public constructor :
	static hEvents * getInstance() {
        if(_events == 0) {
			_events = new hEvents;
        }
        return _events;
    }
		
	// ---------------------------------------------------------------

	void setup(void);
	// Initialize the event system and the default listeners
	
	void addObject(std::string objectName, hObject * obj);
	// Create a symbol that represent an object and add the event to the object map
	// Used to send messages to a specific object
	// objectName is the name of the object instance, not the class

    hObject * getObject(std::string eventName);
	// Return an object from the object map
	// Return NULL if no object with this name was found

	bool objectAvailable(std::string objectName);
	// Return true if objectName is in the "objects" dictionary

	hEvent * addEvent(std::string dictName, std::string eventName);
	// If an event does'nt exist,
	// create a symbol that represent it and add the event to the event map
	// Return the adress of the event

	hEvent * addEvent(std::string eventName);
	// Add an event using the selected dictionary

	hEvent * getEvent(std::string dictName, std::string eventName);
	// Return an event from the event map
	// Return NULL if no event with this name was found

    hEvent * getEvent(std::string eventName);
	// Return an event using the selected dictionary
	
    void disable(void);
	// Disable the sending of events
	
	void enable(void);
	// Enable the sending of events (default)

	void set_verbose(bool flag);
	// Display sent messages on the console if this flag is set
	
	//------------------------------------------

    void sendEvent(std::string dictName, std::string message, hEventArgs &args);
	// Send an event (parameters are given in &args)
	// Can be send to a specific object f.e. message = "obj.method"
	// Or to any object that has this method  f.e. message = "method"
	
	// The event has to be created before (generally with addListener), else it does nothing
	// The eventual object has to be created before too
	
	// Warning: if the listener of the event need value parameters,
	// it should ALWAYS verify the number of received parameters!
	// The listener can decide which parameters are usefull and ignore others
	
	// There are 2 kind of parameters: 
	// doubles (stored in the vector args.values)
	// strings (stored in the vector args.strings)
	
	// The listener has also to call the method of the right object
	// by checking args.objectName
	
    void sendEvent(std::string message, hEventArgs &args);
	// Send an event using the selected dictionary
	
	// ---------------------------------------------------------------
	// Sending event using different numbers of parameters:
	// ---------------------------------------------------------------
	
    void sendEvent(std::string message);
    void sendEvent(std::string message, double val);
    void sendEvent(std::string message, double val1, double val2);
    void sendEvent(std::string message, double val1, double val2, double val3);
    void sendEvent(std::string message, double val1, double val2, double val3, double val4);
	
    void sendEvent(std::string message, std::ostringstream &oss);
	// Uses a string stream argument as string parameter
	// Clear also the string stream when finished

    void sendEvent(std::string message, std::string s);
    void sendEvent(std::string message, std::string s, double val);
    void sendEvent(std::string message, std::string s, double val1, double val2);
    void sendEvent(std::string message, std::string s, double val1, double val2, double val3);
    void sendEvent(std::string message, std::string s, double val1, double val2, double val3, double val4);
	
    void sendEvent(std::string message, std::string s1, std::string s2);
    void sendEvent(std::string message, std::string s1, std::string s2, double val);
    void sendEvent(std::string message, std::string s1, std::string s2, double val1, double val2);
    void sendEvent(std::string message, std::string s1, std::string s2, double val1, double val2, double val3);
    void sendEvent(std::string message, std::string s1, std::string s2, double val1, double val2, double val3, double val4);
	
	// ---------------------------------------------------------------

	template <typename TArgumentsType, class TListenerClass>
    void addListener(std::string dictName, std::string eventName, TListenerClass  * listener, void (TListenerClass::*listenerMethod)(TArgumentsType&)){
        hEvent * event = addEvent(dictName, eventName);
        if(event != NULL) *event += Poco::delegate(listener, listenerMethod);
    }
    // Ask a method of an object to respond to an event
    // If the event does not exist, create it first
    // An event can be connected to more than one responder
	
	template <typename TArgumentsType, class TListenerClass>
    void addListener(std::string eventName, TListenerClass  * listener, void (TListenerClass::*listenerMethod)(TArgumentsType&)){
        hEvent * event = addEvent("english", eventName);
        if(event != NULL) *event += Poco::delegate(listener, listenerMethod);
    }
    // // Ask a method of an object to respond to an event using the default dictionary

	// ---------------------------------------------------------------

    std::string serialize(hEventArgs& args, bool fixedNotation, int digits);
	// Transform the arguments of an event to a string (including the message at the beginning)
	
	// If 'fixedNotation' = false, 'digits' specifies the maximum number of meaningful digits to display
	// in total counting both those before and those after the decimal point
	
	// If 'fixedNotation' = false, 'digits' specifies exactly how many digits to display after the decimal point,
	// even if this includes trailing decimal zeros.
	
	// Since we use doubles, digits can be up to 15, more will display junk.

    std::string unserialize(std::string fullMessage, hEventArgs&args);
	// Transform a string to event arguments (find words, decimal numbers and hexadecimal numbers)
	// Store all parameters in &args (including the message at the beginning)
	// Return also the message
	
	// ---------------------------------------------------------------
	// Standard Listeners: (used by the gui, but can be used by other objects too)
	// ---------------------------------------------------------------
	
    void argTest(hEventArgs& args);
	// Test Listener: display the arguments when the message "arg.test" is received

	void setValue(hEventArgs& args);
	void setValue2(hEventArgs& args);
	
	void setXY(hEventArgs& args);
	void setTime(hEventArgs& args);
	
	void setLabel(hEventArgs& args);
	void clearLabel(hEventArgs& args);
	
	void setText(hEventArgs& args);
	void addText(hEventArgs& args);
	void clearText(hEventArgs& args);
	
	void open(hEventArgs& args);
	void close(hEventArgs& args);

	void clear(hEventArgs& args);
	void display(hEventArgs& args);
	void displayWOcr(hEventArgs& args);

	void select(hEventArgs& args);
	void unselect(hEventArgs& args);
	void setSelected(hEventArgs& args);

	void selectElement(hEventArgs& args);
	void unselectElement(hEventArgs& args);
	void elementSetSelected(hEventArgs& args);
	
	void selectItem(hEventArgs& args);
	void unselectItem(hEventArgs& args);
	void itemSetSelected(hEventArgs& args);

	void bang(hEventArgs& args);
	
	// ---------------------------------------------------------------
	// Other Usefull Listeners:
	// ---------------------------------------------------------------
		
	void setValueToItem(hEventArgs& args);
	void setValue2ToItem(hEventArgs& args);

	void openItem(hEventArgs& args);
	void closeItem(hEventArgs& args);
	void answerDialog(hEventArgs& args);
	

	void start(hEventArgs& args);
	void stop(hEventArgs& args);
	void cont(hEventArgs& args);
	
	// ---------------------------------------------------------------

	bool events_disabled;
	bool verbose;
};


// ---------------------------------------------------------------

#endif // _HEVENTS
