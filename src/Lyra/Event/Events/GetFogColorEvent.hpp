#pragma once
#include "../../SDK/Client/Core/mce.hpp"
class GetFogColorEvent {
	mce::Color colour;
public:
	mce::Color getColour() {
		return this->colour;
	}

	void setColour(mce::Color colour) {
		this->colour = colour;
	}

	GetFogColorEvent(mce::Color colour) : colour(colour) {};
};