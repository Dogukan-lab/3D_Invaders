#include "RenderSystem.h"
#include "../core/Entity.h"
#include "../core/Mesh.h"
#include "../core/Transform.h"
#include "glm/gtc/matrix_transform.hpp"

RenderSystem::RenderSystem()
{
	this->entities = {};
	this->systemSignature.set(types::getComponentTypeID<Mesh>());
	this->systemSignature.set(types::getComponentTypeID<Transform>());
}

/*
* TODO Needs the model matrix as well as the transform for proper entity drawing!!!
*/
void RenderSystem::draw()
{
	tigl::shader->enableLighting(true);
	tigl::shader->enableColor(true);
	tigl::shader->setShinyness(32.f);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightCount(2);
	tigl::shader->setLightPosition(0, glm::vec3(0, 10, -2));
	tigl::shader->setLightDirectional(0, false);
	tigl::shader->setLightAmbient(0, { .15f, .05f, .1f });
	tigl::shader->setLightDiffuse(0, { .9f, .9f, .9f });
	tigl::shader->setLightSpecular(0, { 0, 0, 0 });

	tigl::shader->setLightPosition(1, glm::vec3(0, 10, 2));
	tigl::shader->setLightDirectional(1, false);
	tigl::shader->setLightAmbient(1, { .2f, .2f, .2f });
	tigl::shader->setLightDiffuse(1, { .8f, .8f, .8f });
	tigl::shader->setLightSpecular(1, { 0, 0, 0 });


	//TODO Lighcomponent so you can move it around n shiet.
	for (const auto& entity : this->entities) {
		auto transform = entity->getComponent<Transform>();
		glm::mat4 modelM = glm::mat4(1.f);

		modelM = glm::scale(modelM, transform->scale);
		modelM = glm::translate(modelM, transform->position);
		modelM = glm::rotate(modelM, transform->rotation.x, glm::vec3(1, 0, 0));
		modelM = glm::rotate(modelM, transform->rotation.y, glm::vec3(0, 1, 0));
		modelM = glm::rotate(modelM, transform->rotation.z, glm::vec3(0, 0, 1));
		tigl::shader->setModelMatrix(modelM);
		
		tigl::drawVertices(GL_QUADS, entity->getComponent<Mesh>()->drawable);
	}
}
