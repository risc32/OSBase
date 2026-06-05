cmake_minimum_required(VERSION 3.20)
project(OSBase NONE)

add_library(IDE_Helper INTERFACE)
target_include_directories(IDE_Helper INTERFACE include)

add_custom_target(run
        COMMAND cmd /c comp.bat
        WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
)
