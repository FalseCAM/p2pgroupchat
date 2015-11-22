cmake_minimum_required(VERSION 3.0.0)

project(p2pgroupchat)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Boost COMPONENTS thread system REQUIRED)
find_package(Crypto++ REQUIRED)
find_package(OpenP2P COMPONENTS Root UDP TCP IP Crypt Transport Concurrency Event Util REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${Crypto++_INCLUDE_DIR} ${OpenP2P_INCLUDE_DIRS})

set(${PROJECT_NAME}libs ${Boost_LIBRARIES} ${Crypto++_LIBRARIES} ${OpenP2P_LIBRARIES})

file (GLOB_RECURSE Sources "*.cpp")
file (GLOB_RECURSE Headers "*.h")

add_executable(${PROJECT_NAME} ${Sources} ${Headers})

target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}libs})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME}/bin)
