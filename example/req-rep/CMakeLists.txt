cmake_minimum_required(VERSION 3.10.0)
project(examples VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
cmake_host_system_information(RESULT OSN QUERY OS_NAME)
cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")
message(STATUS " ${HOST}")
message(STATUS " ${OSN}: ${OS_VERSION}")
message(STATUS " ${PROC}")
message(STATUS "-------------------------------------")


# Library =====================================================================
# https://cmake.org/cmake/help/latest/module/ExternalProject.html
include(ExternalProject)
ExternalProject_Add(gecko-proj
  GIT_REPOSITORY    https://github.com/gecko-robotics/gecko.cpp.git
  GIT_TAG           main
  GIT_SHALLOW       TRUE
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/gecko/src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/gecko"
  # CONFIGURE_COMMAND ""
  BUILD_COMMAND     "make"
  INSTALL_COMMAND   ""
  # TEST_COMMAND      ""
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
)

SET(GECKO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gecko/src/include)
SET(GECKO_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/gecko)

# Threads ---------------------------------------------------------
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS ">> Threads found")
    if(CMAKE_USE_PTHREADS_INIT)
        message(STATUS " -> Using pthreads")
    endif()
else()
    message(WARNING "*** Could not find Threads ***")
endif()

# Binary ======================================================================
LIST(APPEND apps
    main.cpp
    imu.cpp
)

foreach(MAIN ${apps})
    add_executable(${MAIN} main.cpp)
    add_dependencies(${MAIN} gecko-proj)
    target_include_directories(${MAIN}
        PRIVATE
            ${GECKO_INCLUDE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/gecko/marko/src/include
    )
    target_link_directories(${MAIN}
        PRIVATE
            ${GECKO_LIB_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/gecko/marko
    )
    target_link_libraries(${MAIN} PRIVATE gecko marko ${CMAKE_THREAD_LIBS_INIT})
endforeach()
