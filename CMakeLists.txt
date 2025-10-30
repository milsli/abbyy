cmake_minimum_required(VERSION 3.5)

project(AbbyyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AbbyyProject main.cpp
    task.h task.cpp
    taskscheduler.h taskscheduler.cpp)

include(GNUInstallDirs)
install(TARGETS AbbyyProject
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
