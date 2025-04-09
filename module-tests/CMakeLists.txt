# CMakeList.txt : CMake project for logfmtxx tests with modules support.

# Now test the modules version
add_executable(logfmtxx-test-runner main.cpp logger.spec.cpp serialization.spec.cpp)
target_link_libraries(logfmtxx-test-runner PRIVATE logfmtxxModule)

add_test(NAME logfmtxx-test-runner COMMAND logfmtxx-test-runner)