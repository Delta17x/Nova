#include "parser.hpp"
#include <iostream>

void remove_leading_whitespace(std::string& src) {
	size_t i = 0;
	while (src[i] == ' ') {
		i++;
	}
	src.erase(0, i);
}

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

int main() {
	auto p = parse1("jeff; foo; bar; asd;");
	for (int i = 0; i < p.size(); i++) {
		std::cout << p[i] << "\n";
	}
}