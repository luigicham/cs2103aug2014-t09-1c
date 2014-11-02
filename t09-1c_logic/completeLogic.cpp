#include "stdafx.h"
#include "CompleteLogic.h"

CompleteLogic::CompleteLogic(string fileName, string date, vector<string> keywords, int displayCase) try : BaseClassLogic(fileName, date, keywords, displayCase)
{
}

catch (const exception& ex)
{
	throw runtime_error(ex.what());
}

CompleteLogic::CompleteLogic(vector<string> testVector, string date, vector<string> keywords, int displayCase) : BaseClassLogic(testVector, date, keywords, displayCase)
{
}

CompleteLogic::~CompleteLogic()
{
}

void CompleteLogic::editCompletion(int fromPosition, int toPosition, bool complete)
{
	vector<int> positions = getSortedLinePositions();
	int positionSize = positions.size();
	int startPosition = fromPosition - 1;
	int endPosition = toPosition - 1;
	try {
		checkFromToValidity(startPosition, endPosition, positionSize);
		for (int i = startPosition; i <= endPosition; ++i) {
			int filePosition = positions[i];
			oldLinePosforUndo.push(filePosition);
			string line = getLineFromPositionNumber(filePosition);
			OldLineEntriesForUndo.push(line);
			if (complete) {
				line = FileEntryFormatter::editAttributedEntryFromLineEntry(COMPLETE_ATTRIBUTE, "yes", line);
			}
			else {
				line = FileEntryFormatter::editAttributedEntryFromLineEntry(COMPLETE_ATTRIBUTE, "no", line);
			}
			editLine(filePosition, line);
		}
	}
	catch (const exception& ex) {
		throw runtime_error(ex.what());
	}
}

void CompleteLogic::checkFromToValidity(int fromPosition, int toPosition, int size)
{
	if (fromPosition >= size || fromPosition < 0 || toPosition >= size || toPosition < 0) {
		throw runtime_error(SLOT_IN_EXCESS);
	}
}

void CompleteLogic::execute(map<string, int> fromToPositions, int executionCase)
{
	try {
		if (executionCase == 1) {
			editCompletion(fromToPositions[FROM_POSITION], fromToPositions[TO_POSITION], true);
		}
		else if (executionCase == 0) {
			editCompletion(fromToPositions[FROM_POSITION], fromToPositions[TO_POSITION], false);
		}
		else {
			assert(false && "Wrong execution case inserted.");
		}
		if (isTestMode) {
			updateSortedEntries();
		}
	}
	catch (const exception& ex) {
		throw runtime_error(ex.what());
	}
}