add_executable(algebra_test dummy_test.cpp)
target_include_directories(algebra_test PRIVATE ..)
add_test(AlgebraTest algebra_test)