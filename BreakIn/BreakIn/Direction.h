#ifndef DIRECTION_H 
#define DIRECTION_H 
#include <utility>
using namespace std;

enum class DirectionType { LEFT, RIGHT, UP, DOWN };

class Direction
{
public:

	Direction();
	Direction(DirectionType const& type);

private:

	DirectionType type;
	pair<int, int> dir;
};

#endif