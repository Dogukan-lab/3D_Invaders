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
    tigl::shader->use();
	tigl::shader->enableFog(true);
	tigl::shader->setFogColor({ 0.3f, 0.4f, 0.6f });
	tigl::shader->setFogExp2(.1f);

	//TODO Lighcomponent so you can move it around.
	for (const auto& entity : this->entities) {
        glm::mat4 modelM = glm::mat4(1.f);
		auto transform = entity->getComponent<Transform>();
        auto texture = entity->getComponent<TextureComponent>();
        auto mesh = entity->getComponent<Mesh>();
        auto light = entity->getComponent<LightComponent>();
		if (texture) {
			//std::cout << "Entity ID: " << entity->entityID << "Has TEXTURE!" << std::endl;
			tigl::shader->enableTexture(true);
			texture->bindTexture();
		}
        if(light) {
            tigl::shader->setLightPosition(0, light->position);
        }
        modelM = glm::translate(modelM, transform->position);
		modelM = glm::rotate(modelM, transform->rotation.z, glm::vec3(0, 0, 1));
		modelM = glm::rotate(modelM, transform->rotation.y, glm::vec3(0, 1, 0));
		modelM = glm::rotate(modelM, transform->rotation.x, glm::vec3(1, 0, 0));
		modelM = glm::scale(modelM, transform->scale);
		tigl::shader->setModelMatrix(modelM);
		tigl::drawVertices(GL_TRIANGLES, mesh->drawable.get());
		tigl::shader->enableTexture(false);
        tigl::shader->enableColorMult(false);
	}
}
