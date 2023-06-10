#include "RenderSystem.h"
#include "../core/Entity.h"
#include "../core/Mesh.h"
#include "../core/Transform.h"
#include "glm/gtc/matrix_transform.hpp"

RenderSystem::RenderSystem()
{
	this->entities.reserve(10);
	this->systemSignature.set(types::getComponentTypeID<Mesh>());
	this->systemSignature.set(types::getComponentTypeID<Transform>());
}

/*
* TODO Needs the model matrix as well as the transform for proper entity drawing!!!
*/
void RenderSystem::draw()
{
	for (const auto& entity : this->entities) {
		auto transform = entity->getComponent<Transform>();
		glm::mat4 modelM = glm::mat4(1.f);

		modelM = glm::scale(modelM, transform->scale);
		modelM = glm::translate(modelM, transform->position);
		modelM = glm::rotate(modelM, transform->rotation.x, glm::vec3(1, 0, 0));
		modelM = glm::rotate(modelM, transform->rotation.y, glm::vec3(0, 1, 0));
		modelM = glm::rotate(modelM, transform->rotation.z, glm::vec3(0, 0, 1));
		tigl::shader->setModelMatrix(modelM);
		tigl::shader->enableColor(true);
		tigl::drawVertices(GL_QUADS, entity->getComponent<Mesh>()->drawable);
		tigl::shader->enableColor(true);
	}
}
