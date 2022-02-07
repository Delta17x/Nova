#pragma once

#include <string>
#include <vector>

typedef std::size_t size_t;

// Returns an array with each line of code on its own element. Delete array after.
std::vector<std::string> parse1(const std::string& src);