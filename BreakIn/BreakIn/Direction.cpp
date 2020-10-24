#include "Direction.h"

Direction::Direction(DirectionType const& type) {
	switch (type)
	{
	case DirectionType::UP:
		dir = make_pair<int, int>(0, -2);
		break;
	case DirectionType::DOWN:
		dir = make_pair<int, int>(0, 2);
		break;
	case DirectionType::LEFT:
		dir = make_pair<int, int>(-2, 0);
		break;
	case DirectionType::RIGHT:
		dir = make_pair<int, int>(2, 0);
		break;
	}
	this->type = type;
}

pair<int, int> Direction::getDir() const {
	return dir;
}