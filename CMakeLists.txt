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
include_directories(lib/glm)
include_directories(lib/imgui/include)

add_subdirectory(lib/glm)
add_subdirectory(lib/tigl)
add_subdirectory(lib/glad)
add_subdirectory(lib/imgui)
link_directories(lib/glm)

add_library(glfw STATIC IMPORTED) 
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${GLFW_LIB})

# Add source to this project's executable.
# TODO: Clean the add_exec up please for the love of god!
add_executable (${PROJECT_NAME} 
"src/3D_invaders.cpp"  
"src/ECS/entities/Entity.h" 
"src/ECS/components/Component.h" 
"src/ECS/components/Transform.h"  
"src/ECS/managers/EntityManager.h" 
"src/camera/fpsCam.cpp"
"src/window/Window.cpp" 
"src/ECS/system/MoveSystem.cpp" 
"src/ECS/managers/SystemManager.cpp" 
"src/ECS/Coordinator.h" 
"src/ECS/components/Mesh.h" 
"src/ECS/system/RenderSystem.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 3D_invaders PROPERTY CXX_STANDARD 17)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} glfw opengl32)
target_link_libraries(${PROJECT_NAME} glad tigl glm IMGUI)

# TODO: Add tests and install targets if needed.
