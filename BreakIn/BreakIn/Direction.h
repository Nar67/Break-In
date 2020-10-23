#ifndef DIRECTION_H 
#define DIRECTION_H 
#include <utility>
using namespace std;

enum class DirectionType { LEFT, RIGHT, UP, DOWN };

class Direction
{
public:

	Direction() = default;
	Direction(DirectionType const& type);
	pair<int, int> getDir() const;

private:

	DirectionType type;
	pair<int, int> dir;
};

#endif