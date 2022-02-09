#include "parser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<size_t> semiLocations;
std::unordered_map<std::string, double> varNums;

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

void parse1(std::string& src) {
	for (size_t i = 0; i < src.size(); i++) {
		if (src[i] == ' ') {
			src[i] = ';';
			while (src[i + 1] == ' ') {
				src.erase(src.begin() + i + 1);
			}
			semiLocations.push_back(i);
		}
		else if (src[i] == '\n') {
			semiLocations.push_back(i);
			src.insert(src.begin() + i++, ';');
		}
	}
}

std::string parse2(std::string& src) {
	std::string ret = "";

	auto type = splice(src, 0, semiLocations[0]);
	std::string actualType = "";
	auto name = splice(src, semiLocations[0] + 1, semiLocations[1]);
	auto val = splice(src, semiLocations[2] + 1, semiLocations[3] + 1);
	if (type == "num") {
		actualType = "double ";
	}
	if (type == "str") {
		actualType = "std::string ";
	}
	if (type == "bool") {
		actualType = "bool ";
	}
	if (type == "void") {
		actualType = "void ";
	}
	ret.append(actualType + name + " = " + val);
	return ret;
}

int main() {
	std::cout << std::boolalpha;
	std::stringstream stream;
	std::ifstream fileToRead("debug.txt");
	stream << fileToRead.rdbuf();
	auto sourceCode = stream.str();
	parse1(sourceCode);
	sourceCode = parse2(sourceCode);
	fileToRead.close();

	std::ofstream fileToWrite("debug2.cpp");
	fileToWrite << sourceCode;
	fileToWrite.close();
}

