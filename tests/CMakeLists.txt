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

add_executable(tests
    test_serializer.cpp
)

target_include_directories(tests 
    PRIVATE
      ${INCLUDE_DIR}
)

target_link_libraries(tests
    PRIVATE
        List
        ListSerializer
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)