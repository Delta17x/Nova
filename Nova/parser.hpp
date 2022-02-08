#pragma once

#include <string>
#include <vector>
#include <unordered_map>

typedef std::size_t size_t;

std::string checkForVariableCreation();
std::string checkForFunctionCreation(std::string line);
std::string checkForFunctionCall();

std::pair<std::string, std::string> parseLine(std::string line);