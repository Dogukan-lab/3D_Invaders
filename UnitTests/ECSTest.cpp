#include "ECSTest.h"
#include "Transform.h"
#include "Mesh.h"
#include "../src/ECS/systems/RenderSystem.h"

TEST_F(ECSTest, CreateEntity) {
    auto entityID = this->ecsCoordinator->createEntity();

    //Test entityID comparison
    EXPECT_EQ(0, entityID);
}

TEST_F(ECSTest, AddTransform) {
    auto entityID = this->ecsCoordinator->createEntity();
    auto actualComp = this->ecsCoordinator->addComponent<Transform>(entityID);

    EXPECT_NE(actualComp, nullptr);
}

TEST_F(ECSTest, AddMesh) {
    auto entityID = this->ecsCoordinator->createEntity();
    auto mesh = this->ecsCoordinator->addComponent<Mesh>(entityID);

    EXPECT_NE(mesh, nullptr);
}

// Test case niet mogelijk, doordat static_assert dit al afvangt.
//TEST_F(ECSTest, AddIncorrectComponent) {
//    auto entityID = this->ecsCoordinator->createEntity();
//    EXPECT_COMPILE_ERROR(this->ecsCoordinator->addComponent<MockComp1>(entityID)); //Dit juist testen xD
//}

TEST_F(ECSTest, DeleteComponent) {
    auto entityID = this->ecsCoordinator->createEntity();
    this->ecsCoordinator->addComponent<Transform>(entityID);
    this->ecsCoordinator->removeComponent<Transform>(entityID);
    auto transform = this->ecsCoordinator->getEntity(entityID)->getComponent<Transform>();

    ASSERT_EQ(transform, nullptr);
}

TEST_F(ECSTest, RegisterSystem) {
    const auto& system = this->ecsCoordinator->registerSystem<RenderSystem>(); //Dit testen
    EXPECT_NE(system, nullptr);
}

TEST_F(ECSTest, AddEntityToSystem) {
    const auto& system = this->ecsCoordinator->registerSystem<RenderSystem>();
    auto entityID = this->ecsCoordinator->createEntity();
    this->ecsCoordinator->addComponent<Transform>(entityID); //Dit moet falen
    EXPECT_NE(system->systemSignature, this->ecsCoordinator->getEntity(entityID)->getSig());
    this->ecsCoordinator->addComponent<Mesh>(entityID); //Dit moet slagen
    EXPECT_EQ(this->ecsCoordinator->getEntity(entityID)->getSig(), system->systemSignature);
}

TEST_F(ECSTest, RemoveEntityFromSystem) {
    this->ecsCoordinator->registerSystem<RenderSystem>();
    auto entityID = this->ecsCoordinator->createEntity();
    this->ecsCoordinator->addComponent<Transform>(entityID);
    this->ecsCoordinator->addComponent<Mesh>(entityID);

    this->ecsCoordinator->destroyEntity(this->ecsCoordinator->getEntity(entityID)); //Dit testen
    const auto& entity = this->ecsCoordinator->getEntity(entityID);

    EXPECT_EQ(nullptr, entity);
}