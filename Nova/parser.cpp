#include "parser.hpp"
#include <iostream>
#include <fstream>

std::unordered_map<std::string, double> varNumbers;
std::unordered_map<std::string, std::string> varStrings;
std::unordered_map<std::string, bool> varBools;

void remove_leading_whitespace(std::string& src) {
	size_t i = 0;
	while (src[i] == ' ') {
		i++;
	}
	src.erase(0, i);
}

// does not include last
std::string splice(const std::string& src, size_t first, size_t last) {
	return src.substr(first, last - first);
}

/*
std::vector<std::string> parse1(const std::string& src) {
	std::vector<std::string> parsed;
	size_t last = 0;
	for (size_t i = 0; i < src.size(); i++) {
		if (src[i] == ';') {
			parsed.push_back(src.substr(last, i - last)); // remove semicolon and put on new line
			last = i + 1; 
			// remove whitespace that was after semicolon
			remove_leading_whitespace(parsed[parsed.size() - 1]);
		}
	}	
	return parsed;
}
*/

std::string checkForVariableCreation(std::string line) {
	if (splice(line, 0, 4) == "num ") {
		size_t equalSignLocation = line.find('=', 4);
		std::string varName = splice(line, 4, equalSignLocation - 1);
		varNumbers[varName] = stoi(splice(line, equalSignLocation + 2, line.size()));
	}
	else if (splice(line, 0, 7) == "string ") {
		size_t equalSignLocation = line.find('=', 7);
		std::string varName = splice(line, 7, equalSignLocation - 1);
		varStrings[varName] = splice(line, equalSignLocation + 3, line.size() - 1);
	}
	return "";
}

std::string checkForFunctionCall(std::string line) {
	if (splice(line, 0, 6) == "print(") {
		size_t closeParenLocation = line.find(')', 6);
		std::string fullMessage = splice(line, 6, closeParenLocation);

		if (fullMessage[0] == '\"' && fullMessage[fullMessage.size() - 1] == '\"') { // Calling with literal
			std::cout << splice(fullMessage, 1, fullMessage.size() - 1);
		}
		else { // Calling with variable. Variable name is equal to fullMessage
			auto iteratorForStrings = varStrings.find(fullMessage);
			if (iteratorForStrings != varStrings.end()) {
				std::cout << iteratorForStrings->second;
			}
			else {
				auto iteratorForNumbers = varNumbers.find(fullMessage);
				if (iteratorForNumbers != varNumbers.end()) {
					std::cout << iteratorForNumbers->second;
				}
				else {
					auto iteratorForBools = varBools.find(fullMessage);
					if (iteratorForBools != varBools.end()) {
						std::cout << iteratorForBools->second;
					}
					else
						return std::string("Function parameter \"" + fullMessage + "\" not found.");
				}
					
			}
		}
	}
	return "";
}

std::string parseLine(std::string line) {
	return checkForVariableCreation(line) + "; " + checkForFunctionCall(line);
}

int main() {
	std::ifstream fileToRead("debug.txt");
	std::string currentLine;
	while (std::getline(fileToRead, currentLine)) {
		auto retMessage = parseLine(currentLine);
		if (retMessage != "; ") {
			std::cout << retMessage << std::endl;
			break;
		}
	}
	fileToRead.close();
}