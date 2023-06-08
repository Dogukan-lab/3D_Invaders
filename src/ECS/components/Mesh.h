#pragma once

#include "Component.h"
#include "tigl.h"

class Mesh: public Component {
public:

	Mesh(std::vector<tigl::Vertex>& vertices) {
		model = tigl::createVbo(vertices);
	}

	~Mesh() override {
		model = nullptr;
		delete model;
	}

private:
	tigl::VBO* model;
};