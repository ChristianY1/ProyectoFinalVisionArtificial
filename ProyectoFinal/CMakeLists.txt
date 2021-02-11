cmake_minimum_required(VERSION 2.8.12)

SET(OpenCV_DIR /usr/local/lib/)

Project(Vision)

find_package(OpenCV REQUIRED)
include_directories( $(OpenCV_INCLUDE_DIRS) )

MACRO(add_source name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    TARGET_LINK_LIBRARIES(${name} ${OpenCV_LIBS})
ENDMACRO()

#add_source(Principal)
file(GLOB SOURCES *.cpp)
add_executable(lbp ${SOURCES})
TARGET_LINK_LIBRARIES(lbp ${OpenCV_LIBS})
