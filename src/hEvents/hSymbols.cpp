
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

#include "hSymbols.h"

#include <iostream>
using namespace std;

hSymbols::hSymbols()
{
	lastID = 0;
	addDictionary("objects");		// Hold the objects to which events are sent
	addDictionary("english");		// Hold the events in the default language
	selectDictionary("english");	// Select the default dictionary (used to send events)
}

hDictionary * hSymbols::getDictionary(std::string name)
{
	hDictionary * dict = NULL;

	int size = dictionaries.size();
    for(int i = 0; i < size; ++i) {
        if(dictionaries[i]->name == name) {
            dict = dictionaries[i];
            break;
        }
    }

	return dict;
}

hDictionary * hSymbols::getSelectedDictionary(void)
{
	return selectedDictionary;
}

void hSymbols::addDictionary(std::string name)
{
	if(getDictionary(name) != NULL) return;

	hDictionary * dict = new hDictionary;
	dict->name = name;

	if(dict != NULL) {
		dictionaries.push_back(dict);
	}
}

void hSymbols::selectDictionary(std::string name)
{
	hDictionary * dict = getDictionary(name);
	if(dict != NULL) {
		selectedDictionary = dict;
	}
}

void hSymbols::listDictionaries(void)
{
	cout << "dictionaries: (" << dictionaries.size() << ")" << endl;

	int size = dictionaries.size();
    for(int i = 0; i < size; ++i) {
        cout << dictionaries[i]->name << endl;
    }
	cout << "selected dictionary: " << selectedDictionary->name << endl;
}


hSymbol hSymbols::getSymbol(std::string dictName, std::string symName)
{
	hSymbol symbol; symbol.ID = 0; symbol.type = 0; // empty symbol

	hDictionary * dict = getDictionary(dictName);
    if(dict == NULL) return symbol;

	if(dict->symbols.find(symName) != dict->symbols.end()) {
		symbol = dict->symbols[symName];
	}

	return symbol;
}


hSymbol hSymbols::aliasSymbol(std::string newDictName, std::string newSymName, std::string oldDictName, std::string oldSymName)
{
	hSymbol symbol; symbol.ID = 0; symbol.type = 0; // empty symbol

	hDictionary * oldDict = getDictionary(oldDictName); if(oldDict == NULL) return symbol;
	hDictionary * newDict = getDictionary(newDictName); if(newDict == NULL) return symbol;
	symbol = getSymbol(oldDictName, oldSymName); if(symbol.ID == 0) return symbol;

	newDict->symbols[newSymName] = symbol;
	// cout << "hSymbols::aliasSymbol: create new " << newDictName << " symbol " << newSymName << " = " << symbol.ID << endl;

	return symbol;
}

hSymbol hSymbols::genSym(std::string dictName, std::string symName)
{
	hSymbol symbol; symbol.ID = 0; symbol.type = 0; // empty symbol

	hDictionary * dict = getDictionary(dictName);
	if(dict == NULL) return symbol;

	if(dict->symbols.find(symName) != dict->symbols.end()) {
		symbol = dict->symbols[symName];
		// cout << "hSymbols::genSym: found "     << dictName << " symbol: " << symName << " = " << symbol.ID << endl;
	}
	else {
		++lastID; symbol.ID = lastID; dict->symbols[symName] = symbol;
		// cout << "hSymbols::genSym: create new " << dictName << " symbol " << symName << " = " << symbol.ID << endl;
	}

	return symbol;
}

std::string hSymbols::symbolName(std::string dictName, unsigned int ID)
{
	std::string name = ""; // undefined default name

	hDictionary * dict = getDictionary(dictName);
	if(dict == NULL) return name;

	std::map <std::string, hSymbol>::const_iterator it;
	for(it = dict->symbols.begin(); it != dict->symbols.end(); ++it) {
		if(it->second.ID == ID) {
			name = it->first;
			break;
		}
	}
	// cout << "hSymbols::symbolName: " << ID << " = " << name << endl;

	return name;
}

void hSymbols::listSymbols(std::string dictName)
{
	hDictionary * dict = getDictionary(dictName);
	if(dict == NULL) {
		cout << dictName << " dictionary not defined" << endl;
		return;
	}

	if(dict->symbols.size() == 0) {
		cout << "no " << dictName << " symbols defined" << endl;
		return;
	}

	cout << dictName << " symbols:" << endl;
	std::map <std::string, hSymbol>::const_iterator it;
	for(it = dict->symbols.begin(); it != dict->symbols.end(); ++it) {
		cout << it->first << " = " << it->second.ID << endl;
	}

}

//--------------------------------------------------------------
//							Helper function
//--------------------------------------------------------------


hSymbol hSymbols::createSymbol(std::string dictName, std::string symName, unsigned int type)
{
	hSymbol symbol; symbol.ID = 0; symbol.type = 0; // empty symbol
	
	hDictionary * dict = getDictionary(dictName);
	if(dict == NULL) return symbol;
	
	++lastID; symbol.ID = lastID; symbol.type = type;
	dict->symbols[symName] = symbol;
	// cout << "hSymbols::createSymbol: create new " << dictName << " symbol " << symName << " = " << symbol.ID << endl;
	
	return symbol;
}

// ---------------------------------------------------------------
