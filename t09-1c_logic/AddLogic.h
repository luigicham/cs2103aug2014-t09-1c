#pragma once
#ifndef ADD_LOGIC
#define ADD_LOGIC

#include "BaseClassLogic.h"

class AddLogic :
	public BaseClassLogic
{
private:
	string lineEntry;

	FileLogic fileHandler;

public:
	AddLogic(FileLogic fileHandler);
	~AddLogic();
	string getLineEntry();
	void setLineEntry(string newLineEntry);
	bool isDateAndTimeCorrect();
	bool isSlotFree();
	void appendToLineEntry(string attribute, string entry);
	void determineType();
	bool isEntryValid();
	void commitAdd();

};

#endif
