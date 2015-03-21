project(circle_routes)

add_executable(solver solver.cc)
target_link_libraries(solver ${PROJECT_NAME})