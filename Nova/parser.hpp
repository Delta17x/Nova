#pragma once

#include <string>
#include <vector>
#include <unordered_map>

typedef std::size_t size_t;

// Returns an array with each line of code on its own element.
std::vector<std::string> parse1(const std::string& src);

std::string checkForVariableCreation(std::string line);
std::string checkForFunctionCreation(std::string line);
std::string checkForFunctionCall(std::string line);

std::string parseLine(std::string line);