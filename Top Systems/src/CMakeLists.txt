CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(testing)

FIND_PACKAGE(glfw3 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

SET(CMAKE_CXX_STANDARD 17)

ADD_EXECUTABLE(DrawPrimitive main.cpp primitives.cpp request.cpp)
target_link_libraries(DrawPrimitive OpenGL::GL)
target_link_libraries(DrawPrimitive glfw)
