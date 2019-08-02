add_library(Catch INTERFACE)

file(GLOB TEST_SOURCES "*.cpp")
add_executable(my_test ${TEST_SOURCES})
target_link_libraries(my_test Catch)

add_test (NAME MyTest COMMAND my_test)
