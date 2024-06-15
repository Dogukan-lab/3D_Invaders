#pragma once
#include "Component.h"
#include "tigl.h"

struct Mesh : public Component {
	tigl::VBO* drawable; //TODO make this a unique ptr??

	Mesh() {
		std::cout << "Mesh has been created!" << std::endl;
		this->drawable = nullptr;
	}

    ~Mesh() {
        this->drawable = nullptr;
        delete this->drawable;
    }

	void setMesh(tigl::VBO* vbo) {
		this->drawable = vbo;
	}
};