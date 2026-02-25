#include "IJ.h"

bool IJ::operator==(const IJ& other) const {
	return i == other.i && j == other.j;
}

bool IJ::operator!=(const IJ& other) const {
	return !(*this == other);
}

bool IJ::operator<(const IJ& other) const {
	// Use std::tie for a clean lexicographical comparison of all members
	return std::tie(i, j) < std::tie(other.i, other.j);
}

std::ostream& operator<<(std::ostream& os, const IJ& obj) {
	os << "{" << obj.i << "," << obj.j << "}";

	return os;
}