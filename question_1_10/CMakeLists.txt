cmake_minimum_required(VERSION 3.5)
project(question_1_10)

macro(imageProcessAdd name)
    add_executable(${name} ${name}.cpp)
    target_include_directories(${name} PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${name} PRIVATE ${OpenCV_LIBS})

    # add test to a virtual project group
#    set_property(TARGET ${name} PROPERTY FOLDER "examples")
endmacro()

find_package(OpenCV)
if (OpenCV_FOUND)
    message(${OpenCV_DIR})
endif (OpenCV_FOUND)

imageProcessAdd(1_channel_swap)
