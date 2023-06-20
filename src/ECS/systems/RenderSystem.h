#pragma once
#include "../core/System.h"
/*
* System that draws every Mesh on the screen.
*/
class RenderSystem : public System {
public:
	RenderSystem(); //Sets its signature
	void draw(); //Draws any of the signature provided
};