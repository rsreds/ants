add_executable(test_dummy_algebra test_dummy.cpp)
add_test(DummyTest test_dummy_algebra)

add_executable(test_vector2 test_vector2.cpp)
target_include_directories(test_vector2 PRIVATE ../include)
target_link_libraries(test_vector2 sfml-graphics)
add_test(SMFLVectorAlgebra test_vector2)

add_executable(test_theme test_theme.cpp)
target_include_directories(test_theme PRIVATE ../include)
target_link_libraries(test_theme sfml-graphics)
add_test(Theme test_theme)

add_executable(test_spawning test_spawning.cpp ../src/ant.cpp)
target_include_directories(test_spawning PRIVATE ../include)
target_link_libraries(test_spawning sfml-graphics)
add_test(Spawn test_spawning)