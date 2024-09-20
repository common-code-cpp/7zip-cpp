cmake_minimum_required(VERSION 3.27)
project(7zpp)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_definitions(-D_UNICODE -DUNICODE)

# 根据架构设置不同输出路径
if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(ARCH_PATH "x64")
elseif("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "ARM64")
    set(ARCH_PATH "arm64")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_GENERATOR_PLATFORM}")
endif()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${ARCH_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${ARCH_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${ARCH_PATH})

file(GLOB 7ZPP_SOURCES  ${PROJECT_SOURCE_DIR}/7zpp/*.cpp )
# 设置动态库
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
if(BUILD_SHARED_LIBS)
	add_library(7zpp SHARED ${7ZPP_SOURCES})
	target_compile_definitions(7zpp PRIVATE SHARED_LIBRARAY_7ZPP)
	target_compile_definitions(7zpp PRIVATE EXPORT_7ZPP)
else()
	add_library(7zpp ${7ZPP_SOURCES})
endif()
target_include_directories(7zpp PRIVATE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/7zpp ${PROJECT_SOURCE_DIR}/7z/C  ${PROJECT_SOURCE_DIR}/7z/CPP)
target_include_directories(7zpp PRIVATE ${PROJECT_SOURCE_DIR}/Include/7zpp)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
find_package(Boost COMPONENTS filesystem ) 
# Setup testing
IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Debug)
ENDIF()
if(Boost_FOUND)
    message("Building and running tests")
    enable_testing()


    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/ ${CMAKE_SOURCE_DIR}/googletest/include ${CMAKE_SOURCE_DIR}/googletest/googletest ${CMAKE_SOURCE_DIR}/googletest/googletest/include)
    #find_package(GTest REQUIRED)

    add_executable(TestApp  ${PROJECT_SOURCE_DIR}/7zpp-TestApp/7zpp-TestApp.cpp)

    target_include_directories(TestApp PRIVATE 
    ${PROJECT_SOURCE_DIR}/7zpp-TestApp/ 
    ${PROJECT_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/7zpp 
    ${PROJECT_SOURCE_DIR}/7z/C  
    ${PROJECT_SOURCE_DIR}/7z/CPP
    ${Boost_INCLUDE_DIRS} 
    )


    add_dependencies(TestApp 7zpp)
    set(GTEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc)
    add_library(gtest STATIC ${GTEST_SRC}) 

    target_link_libraries(TestApp 7zpp gtest ${Boost_LIBRARIES})


    add_test(
        NAME runUnitTests
        COMMAND TestApp --gtest_output=xml:tests.xml
    )


    file(COPY ${PROJECT_SOURCE_DIR}/7zpp-TestApp/TestFiles DESTINATION  ${CMAKE_SOURCE_DIR}/build)

    set(UNIT_TEST TestApp)
else()
    message("Not running tests:Make sure you have boost installed.")
endif()

