add_executable(renderer main.cc)
target_link_libraries(renderer PRIVATE ${SDL2_LIBRARIES})