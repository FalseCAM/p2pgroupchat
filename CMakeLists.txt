cmake_minimum_required(VERSION 3.0.0)

project(p2pgroupchat)

set(PROJECT_NAME "p2pgroupchat")
set(PROJECT_LONGNAME "Peer to Peer Group Chat")
set(PROJECT_DESCRIPTION "Serverless unencrypted group chat.")
set(VERSION "0.1.0")

add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

if(WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
elseif(UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
else(WIN32)
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
endif(WIN32)

#############################################################
# Enable packaging
include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Hypha Runner")
set(CPACK_PACKAGE_VENDOR "falsecam")
set(CPACK_PACKAGE_CONTACT "falsecam")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "/usr/local")
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-all-dev")

set (CPACK_PACKAGE_FILE_NAME   "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_DEBIAN_ARCHITECTURE}")
include(CPack)
#############################################################

add_subdirectory( src )
add_subdirectory( doc )
