project(circle_routes)

find_package(Boost 1.57 EXACT REQUIRED)

add_executable(solver solver.cc)
target_link_libraries(solver ${PROJECT_NAME} -lboost_system -lboost_program_options)