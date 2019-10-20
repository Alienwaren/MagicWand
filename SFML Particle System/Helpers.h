#ifndef HELPERS_H
#define HELPERS_H
#include <tuple>

inline bool is_outside_tolerance(std::tuple<int, int> first_coords, std::tuple<int, int> second_coords, const int epsilon = 50)
{
	const bool similar_x = abs(std::get<0>(first_coords) - std::get<0>(second_coords)) > epsilon;
	const bool similar_y = abs(std::get<1>(first_coords) - std::get<1>(second_coords)) > epsilon;

	return similar_x && similar_y;
}

#endif //HELPERS_H