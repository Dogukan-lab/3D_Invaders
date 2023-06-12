#include "RenderSystem.h"
#include "../core/Entity.h"
#include "../core/Mesh.h"
#include "../core/Transform.h"
#include "../core/TextureComponent.h"
#include "glm/gtc/matrix_transform.hpp"

RenderSystem::RenderSystem()
{
	this->entities = {};
	this->systemSignature.set(types::getComponentTypeID<Mesh>());
	this->systemSignature.set(types::getComponentTypeID<Transform>());
}

/*
* TODO Find a way to make the light position adjustable!
* Renders every entity that matches with its signature.
*/
void RenderSystem::draw()
{
	tigl::shader->enableLighting(true);
	tigl::shader->enableColor(true);
	tigl::shader->setShinyness(20.f);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->setLightPosition(0, { 0, 10, 2 });
	tigl::shader->setLightAmbient(0, { .15f, .05f, .1f });
	tigl::shader->setLightDiffuse(0, { .9f, .9f, .9f });
	tigl::shader->setLightSpecular(0, { 0.4f, 0.6f, 0.1f });

	tigl::shader->enableFog(true);
	tigl::shader->setFogColor({ 0.3f, 0.4f, 0.6f });
	tigl::shader->setFogExp2(.25f);
	//tigl::shader->enableTexture(true);

	//TODO Lighcomponent so you can move it around n shiet.
	for (const auto& entity : this->entities) {
		auto transform = entity->getComponent<Transform>();
		glm::mat4 modelM = glm::mat4(1.f);
		auto texture = entity->getComponent<TextureComponent>();
		if (texture) {
			//std::cout << "Entity ID: " << entity->entityID << "Has TEXTURE!" << std::endl;
			tigl::shader->enableTexture(true);
			texture->bindTexture();
		}

		modelM = glm::translate(modelM, transform->position);
		modelM = glm::rotate(modelM, transform->rotation.x, glm::vec3(1, 0, 0));
		modelM = glm::rotate(modelM, transform->rotation.y, glm::vec3(0, 1, 0));
		modelM = glm::rotate(modelM, transform->rotation.z, glm::vec3(0, 0, 1));
		modelM = glm::scale(modelM, transform->scale);
		tigl::shader->setModelMatrix(modelM);
		tigl::drawVertices(GL_TRIANGLES, entity->getComponent<Mesh>()->drawable);
		tigl::shader->enableTexture(false);
	}
}
