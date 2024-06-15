#include "RenderSystem.h"
#include "../core/Entity.h"
#include "../core/Mesh.h"
#include "../core/Transform.h"
#include "../core/LightComponent.h"
#include "../core/TextureComponent.h"
#include "glm/gtc/matrix_transform.hpp"

//TODO Make shader compile function for setting up the shader necessary.
//TODO Cache created shader programs???
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
	tigl::shader->setLightCount(2);
	tigl::shader->setLightDirectional(0, false);
	tigl::shader->setLightPosition(0, { 0, 5, 10 });
	tigl::shader->setLightAmbient(0, { .15f, .05f, .1f });
	tigl::shader->setLightDiffuse(0, { .9f, .9f, .9f });
	tigl::shader->setLightSpecular(0, { 0.4f, 0.6f, 0.1f });

	tigl::shader->setLightDirectional(1, false);
	tigl::shader->setLightPosition(1, { -10, 5, -10 });
	tigl::shader->setLightAmbient(1, { .15f, .05f, .1f });
	tigl::shader->setLightDiffuse(1, { .9f, .9f, .9f });
	tigl::shader->setLightSpecular(1, { 0.4f, 0.6f, 0.1f });


	tigl::shader->enableFog(true);
	tigl::shader->setFogColor({ 0.3f, 0.4f, 0.6f });
	tigl::shader->setFogExp2(.25f);
	//tigl::shader->enableTexture(true);

	//TODO Lighcomponent so you can move it around.
	for (const auto& entity : this->entities) {
		auto transform = entity->getComponent<Transform>().lock();
		glm::mat4 modelM = glm::mat4(1.f);
        auto texture = entity->getComponent<TextureComponent>().lock();
        auto light = entity->getComponent<LightComponent>().lock();
		if (texture) {
			//std::cout << "Entity ID: " << entity->entityID << "Has TEXTURE!" << std::endl;
			tigl::shader->enableTexture(true);
			texture->bindTexture();
		}
        if(light) {
            tigl::shader->setLightPosition(0, light->position);
        }

		modelM = glm::translate(modelM, transform->position);
		modelM = glm::rotate(modelM, transform->rotation.x, glm::vec3(1, 0, 0));
		modelM = glm::rotate(modelM, transform->rotation.y, glm::vec3(0, 1, 0));
		modelM = glm::rotate(modelM, transform->rotation.z, glm::vec3(0, 0, 1));
		modelM = glm::scale(modelM, transform->scale);
		tigl::shader->setModelMatrix(modelM);
		tigl::drawVertices(GL_TRIANGLES, entity->getComponent<Mesh>().lock()->drawable.get());
		tigl::shader->enableTexture(false);
	}
}
