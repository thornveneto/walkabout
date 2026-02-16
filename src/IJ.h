#pragma once
#include <tuple>
/* 
	Utility type for i-th row and j-th cell.
	Unlike XY is not templateable and int only.
	used to avoid confusion with XY (since order is different) and to avoid generic pair.
*/
struct IJ {
	int i{};
	int j{};

	bool operator==(const IJ& other) const {
		return i == other.i && j == other.j;
	}

	bool operator!=(const IJ& other) const {
		return !(*this == other);
	}

	bool operator<(const IJ& other) const {
		// Use std::tie for a clean lexicographical comparison of all members
		return std::tie(i, j) < std::tie(other.i, other.j);
	}
};