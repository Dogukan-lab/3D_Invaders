#pragma once
#include "Component.h"
#include "tigl.h"
#include "Material.h"

struct Mesh : public Component {
	std::shared_ptr<tigl::VBO> drawable; //TODO make this a unique ptr??
    Material objMaterial;

	Mesh() {
		std::cout << "Mesh has been created!" << std::endl;
		this->drawable = nullptr;
        this->objMaterial;
	}

    Mesh(const Material& newMat) {
        this->drawable = nullptr;
        this->objMaterial = newMat;
    }

    ~Mesh() {
        drawable = nullptr;
    }

	void setMesh(const std::shared_ptr<tigl::VBO>& vbo) {
		this->drawable = vbo;
	}
};