cmake_minimum_required (VERSION 3.8)
project ("3D_invaders")


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GLFW_LIB ${PROJECT_SOURCE_DIR}/lib/glfw/64-bit/lib-vc2022/glfw3.lib)
    set(GLFW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/glfw/64-bit/include)
else()
    set(GLFW_LIB ${PROJECT_SOURCE_DIR}/lib/glfw/32-bit/lib-vc2022/glfw3.lib)
    set(GLFW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/glfw/32-bit/include)
endif ()

include_directories(lib/glad/include) 
include_directories(lib/tigl/include)
include_directories(lib/imgui/include)
include_directories(lib/glm)

add_subdirectory(lib/glm)
add_subdirectory(lib/tigl)
add_subdirectory(lib/glad)
add_subdirectory(lib/imgui)

link_directories(lib/glm)

add_library(glfw STATIC IMPORTED) 
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${GLFW_LIB})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "src/3D_invaders.cpp" 
"src/camera/fpsCam.cpp" 
"src/window/Window.cpp" 
"src/ECS/core/Types.h"  
"src/ECS/core/Component.h" 
"src/ECS/core/EntityManager.h" 
"src/ECS/core/Transform.h"
"src/ECS/core/Mesh.h"
"src/ECS/core/System.h"
"src/ECS/core/SystemManager.cpp" 
"src/ECS/systems/RenderSystem.cpp"
"src/ECS/core/Entity.cpp" 
"src/ECS/core/Coordinator.h" 
"src/ECS/core/LightComponent.h" 
"src/ECS/core/TextureComponent.cpp"
"src/modelLoader/ModelLoader.cpp")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 3D_invaders PROPERTY CXX_STANDARD 17)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} glfw IMGUI opengl32)
target_link_libraries(${PROJECT_NAME} glad tigl glm)

