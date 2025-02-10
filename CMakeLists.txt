cmake_minimum_required(VERSION 3.10)

project (Conbusit VERSION 0.147)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(Conbusit
                Src/Game/game.cpp
                Src/Game/game.h
                
                Src/Window/events.h 
                Src/Window/events.cpp
                Src/Window/platform.cpp
                Src/Window/platform.h)

target_include_directories(Conbusit PUBLIC
                            "${PROJECT_SOURCE_DIR}/Src"
                            "${PROJECT_SOURCE_DIR}/VendorCode"
                            "&{PROJECT_SOURCE_DIR}/Vulkan/Include")

target_link_directories(Conbusit PUBLIC
                        "${PROJECT_SOURCE_DIR}ExtDeps/Vulkan/Lib")

#target_link_libraries(Conbusit PUBLIC 
                        #vulkan-1)               