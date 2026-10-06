#pragma once
#include <string>
#include <iostream>

namespace sr {
	inline std::string ToLower(const std::string& str) {
		std::string l_str = str;

		for (char& c : l_str) {
			c = std::tolower(c);
		}
		return l_str;
	}

	inline std::string ToUpper(const std::string& str) {
		std::string u_str = str;

		for (char& c : u_str) {
			c = std::toupper(c);
		}
		return u_str;
	}

	inline bool EqualsIgnoreCase(const std::string& str_a, const std::string& str_b) {
		if (str_a.length() != str_b.length()) return false;

		return ToLower(str_a) == ToLower(str_b);
	}
}