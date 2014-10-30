#include "stdafx.h"
#include "SearchLogic.h"
#include "ArrangeLogic.h"
#include "FileEntryFormatter.h"
#include "FileLogic.h"
#include "TimeLogic.h"
#include "DL_Algorithm.h"


SearchLogic::SearchLogic(string fileName) : fileHandler(fileName)
{
}


SearchLogic::~SearchLogic()
{
}
		
vector<string> SearchLogic::createKeywords(string input)
{
	DL_Algorithm diffCost;
	transform(input.begin(), input.end(), input.begin(), ::tolower);

	vector<string> suggestionsList;
	vector<int> suggestionPriority;
	if (input.length() >= MIMUMUM_LENGTH) {
		int numberOfEvents = fileHandler.getSize();
		for (int i = 0; i < numberOfEvents; ++i) {
			string line = fileHandler.getLineFromPositionNumber(i);
			
			string lineCategory = FileEntryFormatter::getAttributeEntry("category", line);
			int categoryDifference = diffCost.findDLCost(input, lineCategory);
			if (categoryDifference <= NONDATETIMEMAXCOST)
			{
				string categoryChars = lineCategory;
				transform(categoryChars.begin(), categoryChars.end(), categoryChars.begin(), ::tolower);
				categoryDifference = (categoryChars.find(input) != string::npos) ? -1 : categoryDifference;
				pair<vector<string>, vector<int>> newSuggestions = determinePriority(suggestionsList, suggestionPriority, lineCategory, categoryDifference);
				suggestionsList = newSuggestions.first;
				suggestionPriority = newSuggestions.second;
			}

			string lineName = FileEntryFormatter::getAttributeEntry("name", line);
			int nameDifference = diffCost.findDLCost(input, lineName);
			if (nameDifference <= NONDATETIMEMAXCOST)
			{
				string nameChars = lineName;
				transform(nameChars.begin(), nameChars.end(), nameChars.begin(), ::tolower);
				nameDifference = (nameChars.find(input) != string::npos) ? -1 : nameDifference;
				pair<vector<string>, vector<int>> newSuggestions = determinePriority(suggestionsList, suggestionPriority, lineName, nameDifference);
				suggestionsList = newSuggestions.first;
				suggestionPriority = newSuggestions.second;
			}

			string lineDate = FileEntryFormatter::getAttributeEntry("date", line);
			string lineEnd = FileEntryFormatter::getAttributeEntry("end", line);
			TimeLogic endDateTime(lineDate, lineEnd);
			string endDate = endDateTime.getStringDate();
			
			int lineDateDifference = diffCost.findDLCost(input, lineDate);
			int endDateDifference = diffCost.findDLCost(input, endDate);

			if (lineDateDifference <= DATETIMEMAXCOST || endDateDifference <= DATETIMEMAXCOST) {
				TimeLogic dateCheck(lineDate, "00:00");
				if (dateCheck.getTimeFormatCheck() && endDateTime.getTimeFormatCheck()) {
					pair<vector<string>, vector<int>> newSuggestions = determinePriority(suggestionsList, suggestionPriority, lineDate, lineDateDifference);
					suggestionsList = newSuggestions.first;
					suggestionPriority = newSuggestions.second;
					newSuggestions = determinePriority(suggestionsList, suggestionPriority, endDate, endDateDifference);
					suggestionsList = newSuggestions.first;
					suggestionPriority = newSuggestions.second;
				}
			}
			
			string endTime = endDateTime.getStringTime();
			int endDifference = diffCost.findDLCost(input, endTime);

			if (endDifference <= DATETIMEMAXCOST)
			{
				if (endDateTime.getTimeFormatCheck()) {
					pair<vector<string>, vector<int>> newSuggestions = determinePriority(suggestionsList, suggestionPriority, endTime, endDifference);
					suggestionsList = newSuggestions.first;
					suggestionPriority = newSuggestions.second;
				}
			}

			if (FileEntryFormatter::getAttributeEntry("type", line) == "timed") {
				if (checkTimedTaskEligibility(input, line)) {
					pair<vector<string>, vector<int>> newSuggestions = determinePriority(suggestionsList, suggestionPriority, input, 0);
					suggestionsList = newSuggestions.first;
					suggestionPriority = newSuggestions.second;
				}
			}
		}
		
	}
	return suggestionsList;
}

pair<vector<string>, vector<int>> SearchLogic::determinePriority(vector<string> list, vector<int> priority, string keyword, int diffCost) 
{
	vector<string> suggestionsList = list;
	vector<int> priorityList = priority;
	int size = suggestionsList.size();

	bool isCopy = false;
	for (int i = 0; i < size; ++i) {
		string suggestion = suggestionsList[i];
		if (keyword == suggestion) {
			isCopy = true;
			break;
		}
	}

	if (!isCopy) {
		if (suggestionsList.size() == 0) {
			suggestionsList.push_back(keyword);
			priorityList.push_back(diffCost);
		}
		else {
			for (int i = 0; i < size; ++i) {
				if (priorityList[i] >= diffCost) {
					vector<string>::iterator it1;
					it1 = suggestionsList.begin() + i;
					suggestionsList.insert(it1, keyword);

					vector<int>::iterator it2;
					it2 = priorityList.begin() + i;
					priorityList.insert(it2, diffCost);

					if (suggestionsList.size() > SUGGESTIONS_LIMIT) {
						suggestionsList.resize(SUGGESTIONS_LIMIT);
					}
					break;
				}
			}
		}
	}
	pair<vector<string>, vector<int>> newLists(suggestionsList, priorityList);
	return newLists;
}

bool SearchLogic::checkTimedTaskEligibility(string input, string line)
{
	bool isEligible;

	string date = FileEntryFormatter::getAttributeEntry("date", line);
	string start = FileEntryFormatter::getAttributeEntry("start", line);
	string end = FileEntryFormatter::getAttributeEntry("end", line);

	TimeLogic startTime(date, start);
	TimeLogic endTime(date, end);
	TimeLogic inputTime(date, input);

	isEligible = TimeLogic::isFirstEarlierThanSecond(startTime, inputTime) && TimeLogic::isFirstEarlierThanSecond(inputTime, endTime);
	if (!isEligible && end.substr(5, 2) == "+1") {
		TimeLogic inputTimePlusOne(date, input + "+1");
		isEligible = TimeLogic::isFirstEarlierThanSecond(startTime, inputTimePlusOne) && TimeLogic::isFirstEarlierThanSecond(inputTimePlusOne, endTime);
	}
	return isEligible;
}

vector<string> SearchLogic::getFreeSlots(string date)
{
	vector<string> freeSlots;
	TimeLogic checkDate(date, "00:00");
	if (checkDate.getTimeFormatCheck()) {
		vector<string> keyword;
		keyword.push_back(date);
		ArrangeLogic arranger(fileHandler);
		vector<string> eventList = arranger.getListOfEventsWithKeywords(keyword).first;
		int eventListSize = eventList.size();
		string currTime = "00:00";
		for (int i = 0; i < eventListSize; ++i) {
			string line = eventList[i];
			if (FileEntryFormatter::getAttributeEntry("type", line) == "timed") {
				string start = FileEntryFormatter::getAttributeEntry("start", line);
				string end = FileEntryFormatter::getAttributeEntry("end", line);
				TimeLogic startTime(date, start);
				TimeLogic endTime(date, end);
				TimeLogic currentTime(date, currTime);
				if (startTime.getTimeFormatCheck() && endTime.getTimeFormatCheck()) {
					if (currTime != start && TimeLogic::isFirstEarlierThanSecond(currentTime, startTime)) {
						string freeSlot = currTime + ":" + start;
						freeSlots.push_back(freeSlot);
						currTime = end;
					}
				}
			}
		}
	}
	return freeSlots;
}