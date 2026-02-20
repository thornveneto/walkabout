#pragma once
//Utility type for meaningfull cartesian coordinates instead of std::pair
template<typename T>
struct XY {
	T x{};
	T y{};
};