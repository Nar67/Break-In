#include "Direction.h"

Direction::Direction(DirectionType const& type) {
	switch (type)
	{
	case DirectionType::UP:
		dir = make_pair<int, int>(0, -1);
		break;
	case DirectionType::DOWN:
		dir = make_pair<int, int>(0, 1);
		break;
	case DirectionType::LEFT:
		dir = make_pair<int, int>(-1, 0);
		break;
	case DirectionType::RIGHT:
		dir = make_pair<int, int>(1, 0);
		break;
	}
	this->type = type;
}
