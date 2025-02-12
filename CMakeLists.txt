cmake_minimum_required(VERSION 3.10)

project (Conbusit VERSION 0)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(Conbusit
                Src/Game/game.cpp
                Src/Game/game.h
                Src/Game/control.h 
                Src/Game/control.cpp
                
                Src/Window/events.h 
                Src/Window/events.cpp
                Src/Window/platform.cpp
                Src/Window/platform.h
                
                Src/Rendering/renderer.h
                Src/Rendering/renderer.cpp
                
                Src/ConVulkan/vkInit.cpp 
                Src/ConVulkan/vkRenderer.h)

target_include_directories(Conbusit PUBLIC
                            "${PROJECT_SOURCE_DIR}/Src"
                            "${PROJECT_SOURCE_DIR}/Src/VendorCode"
                            "${PROJECT_SOURCE_DIR}/ExtDeps/Vulkan/Include")

target_link_directories(Conbusit PUBLIC
                        "${PROJECT_SOURCE_DIR}/ExtDeps/Vulkan/Lib")

target_link_libraries(Conbusit PUBLIC 
                        vulkan-1)               