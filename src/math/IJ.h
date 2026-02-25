#pragma once
#include <tuple>
#include <iostream>
/* 
	Utility type for i-th row and j-th cell.
	Unlike XY is not templateable and int only.
	used to avoid confusion with XY (since order is different) and to avoid generic pair.
*/
struct IJ {
	int i{};
	int j{};

	bool operator==(const IJ& other) const;

	bool operator!=(const IJ& other) const;

	bool operator<(const IJ& other) const;
};

std::ostream& operator<<(std::ostream& os, const IJ& obj);