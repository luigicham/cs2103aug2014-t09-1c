#pragma once
#ifndef TIME_LOGIC
#define TIME_LOGIC

#include <time.h>
#include <ctime>
#include <iostream>
#include <list>

using namespace std;

class TimeLogic
{
private:
	bool timeFormatCheck = true;
	string stringDate;
	string stringTime;
	
	//Set-Up TimeLogic Functions
	void convertStringToTimeLogic(string dateString, string timeString);
	void convertAndInsertDate(string date);
	void convertAndInsertTime(string time);
	bool isDateValid(int day, int mon, int year);
	bool isTimeValid(int hour, int min);

public:
	int year, month, day, hour, min;
	TimeLogic(string date, string time); //time should be in "dd/mm/yyyy" & "HH:MM" format. NO EXCEPTIONS.
	~TimeLogic();
	bool getTimeFormatCheck();
	void declareTimeFormatError();

	//These are functions which controller and logic can access staticlly
	static bool isFirstEarlierThanSecond(TimeLogic time1, TimeLogic time2);
	static bool isTimeBeforeToday(TimeLogic time1);
	static string getTimeNowInString();
};

#endif

