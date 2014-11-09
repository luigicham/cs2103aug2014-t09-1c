#include "stdafx.h"
#include "CommandAndArgumentParser.h"
#include <algorithm>

//@ERIC A0111718M
CommandAndArgumentParser::CommandAndArgumentParser(string input)
{
	setCommand(input);
	setArguments(input);
}

//@ERIC A0111718M
CommandAndArgumentParser::~CommandAndArgumentParser()
{
}

//@ERIC A0111718M
void CommandAndArgumentParser::setCommand(string input)
{
	string command;
	stringstream takeCommand(input);
	takeCommand >> command;
	this->command = command;
	std::transform(this->command.begin(), this->command.end(), this->command.begin(), ::tolower);
}

//@ERIC A0111718M
void CommandAndArgumentParser::setArguments(string input)
{
	string arguments;
	std::string first;
	std::istringstream commandRemover{ input };
	commandRemover >> arguments;

	std::ostringstream oss;
	oss << commandRemover.rdbuf();

	arguments = argumentStringParser(oss.str());

	this->arguments = arguments;
}

//@ERIC A0111718M
string CommandAndArgumentParser::getCommand()
{
	return command;
}

//@ERIC A0111718M
string CommandAndArgumentParser::getArguments()
{
	return arguments;
}

//@ERIC A0111718M
string CommandAndArgumentParser::argumentStringParser(string argument)
{
	if (isParameterStringEmpty(argument)) {
		argument = EMPTY_STRING;
	}
	else {
		argument.erase(0, 1); // remove leading whitespace
	}

	return argument;
}

bool CommandAndArgumentParser::isParameterStringEmpty(string parameter)
{
	return parameter.find_first_not_of(' ') == std::string::npos;
}