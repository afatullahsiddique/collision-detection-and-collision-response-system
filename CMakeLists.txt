cmake_minimum_required(VERSION 3.15)
project(CollisionEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(external/glad)

add_executable(CollisionEngine
    src/main.cpp
    src/Engine.cpp
    src/physics/RigidBody.cpp
    src/physics/PhysicsWorld.cpp
    src/collision/Collision.cpp
    src/shapes/Circle.cpp
    src/shapes/Rectangle.cpp
    src/shapes/Polygon.cpp
    src/render/Renderer.cpp
    src/render/ImageWriter.cpp
)

target_include_directories(CollisionEngine PRIVATE src external/glad/include)
target_link_libraries(CollisionEngine OpenGL::GL glfw glad glm::glm)
