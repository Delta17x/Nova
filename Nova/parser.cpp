#include "parser.hpp"
#include <iostream>
#include <fstream>

std::unordered_map<std::string, double> varNumbers;
std::unordered_map<std::string, std::string> varStrings;
std::unordered_map<std::string, bool> varBools;
std::string currentLine;
size_t currentLineNumber;

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
			parsed.push_back(src.substr(last, i - last)); // remove semicolon and put on new currentLine
			last = i + 1; 
			// remove whitespace that was after semicolon
			remove_leading_whitespace(parsed[parsed.size() - 1]);
		}
	}	
	return parsed;
}
*/

std::string checkForVariableCreation() {
	if (splice(currentLine, 0, 4) == "num ") {
		size_t equalSignLocation = currentLine.find('=', 4);
		std::string varName = splice(currentLine, 4, equalSignLocation - 1);
		varNumbers[varName] = stoi(splice(currentLine, equalSignLocation + 2, currentLine.size()));
	}
	else if (splice(currentLine, 0, 7) == "string ") {
		size_t equalSignLocation = currentLine.find('=', 7);
		std::string varName = splice(currentLine, 7, equalSignLocation - 1);
		std::string spliced = splice(currentLine, equalSignLocation + 3, currentLine.size() - 1);
		if (spliced[0] != '\"' || *spliced.end() != '\"') {
			return std::string("Unidentified keyword \"" + spliced + "\" found on line " + std::to_string(currentLineNumber) + ".");
		}
		varStrings[varName] = spliced;
	}
	else if (splice(currentLine, 0, 5) == "bool ") {
		size_t equalSignLocation = currentLine.find('=', 5);
		std::string varName = splice(currentLine, 5, equalSignLocation - 1);
		std::string spliced = splice(currentLine, equalSignLocation + 2, currentLine.size());
		if (spliced != "false" && spliced != "0" && spliced != "true" && spliced != "1") {
			return std::string("Unidentified keyword \"" + spliced + "\" found on line " + std::to_string(currentLineNumber) + ".");
		}
		varBools[varName] = (spliced == "true" || spliced == "1");
	}
	return "";
}

std::string checkForFunctionCall() {
	if (splice(currentLine, 0, 6) == "print(") {
		size_t closeParenLocation = currentLine.find(')', 6);
		std::string fullMessage = splice(currentLine, 6, closeParenLocation);

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
						return std::string("Unidentified function argument \"" + fullMessage + "\" found on line " + std::to_string(currentLineNumber) + ".");
				}
					
			}
		}
	}
	return "";
}

std::pair<std::string, std::string> parseLine() {
	return std::pair<std::string, std::string>(checkForVariableCreation(), checkForFunctionCall());
}

int main() {
	std::cout << std::boolalpha;
	std::ifstream fileToRead("debug.txt");

	while (std::getline(fileToRead, currentLine)) {
		currentLineNumber++;
		auto retMessages = parseLine();
		if (!retMessages.first.empty()) {
			std::cout << "Error: " << retMessages.first << std::endl;
			break;
		}
		if (!retMessages.second.empty()) {
			std::cout << "Error: " << retMessages.second << std::endl;
			break;
		}
	}
	fileToRead.close();
}