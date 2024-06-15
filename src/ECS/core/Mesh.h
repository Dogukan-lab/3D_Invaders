#pragma once
#include "Component.h"
#include "tigl.h"

struct Mesh : public Component {
	std::shared_ptr<tigl::VBO> drawable; //TODO make this a unique ptr??

	Mesh() {
		std::cout << "Mesh has been created!" << std::endl;
		this->drawable = nullptr;
	}

    ~Mesh() {
        drawable = nullptr;
    }

	void setMesh(const std::shared_ptr<tigl::VBO>& vbo) {
		this->drawable = vbo;
	}
};