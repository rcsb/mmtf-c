# Adapted from http://isti.bitbucket.org/2012/06/01/cmake-check.html
enable_testing()
find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
set(LIBS ${LIBS} ${CHECK_LIBRARIES} num_converters)

include_directories(. ../src)
add_executable(test_num_converters test_num_converters.c)
add_test(test_num_converters ${CMAKE_CURRENT_BINARY_DIR}/test_num_converters)
target_link_libraries(test_num_converters ${LIBS})
