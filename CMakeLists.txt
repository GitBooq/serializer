cmake_minimum_required(VERSION 3.14)

project(Serializer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_compile_options(-Wall -Wextra)

include_directories(${INCLUDE_DIR})

add_library(List
    ${SRC_DIR}/List.cpp
)

add_library(ListSerializer
    ${SRC_DIR}/ListSerializer.cpp
)

add_executable(main
	${SRC_DIR}/main.cpp
)
target_link_libraries(main 
PRIVATE 
  List 
  ListSerializer
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/inlet.in)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inlet.in 
                   ${CMAKE_CURRENT_BINARY_DIR}/inlet.in COPYONLY)
endif()

# GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(tests)