project(circle_routes)

find_package(Boost 1.57 EXACT)

if (Boost_FOUND)
    message(STATUS ${Boost_INCLUDE_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})


    add_executable(solver solver.cc)
    target_link_libraries(solver ${PROJECT_NAME} -lboost_system -lboost_program_options)

endif()