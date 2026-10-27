#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool validateAndSplit(const std::string& input, std::vector<std::string>& parts) {
	parts.clear();

	std::istringstream stream(input);
	std::string part;
	while (stream >> part) {
		parts.push_back(part);
	}

	return !parts.empty();
}

int main() {
	std::string line;
	while (std::getline(std::cin, line)) {
		std::vector<std::string> parts;
		if (validateAndSplit(line, parts)) {
			// Process the validated parts here.
		}
	}

	return 0;
}
