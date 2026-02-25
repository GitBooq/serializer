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