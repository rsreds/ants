add_executable(test_dummy_algebra test_dummy.cpp)
target_include_directories(test_dummy_algebra PRIVATE ..)
add_test(DummyTest test_dummy_algebra)

add_executable(test_vector2 test_vector2.cpp)
target_include_directories(test_vector2 PRIVATE ..)
target_include_directories(test_vector2 PRIVATE ../include)
target_link_libraries(test_vector2 sfml-graphics)
add_test(SMFLVectorAlgebra test_vector2)