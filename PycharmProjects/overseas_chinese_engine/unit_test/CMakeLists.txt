aux_source_directory(. UNITTEST_SOURCE_LIST)
add_executable(unit_test ${UNITTEST_SOURCE_LIST})

target_link_libraries(unit_test gtest gtest_main overseasCore)