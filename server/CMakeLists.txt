project(circle_routes)

find_package(Poco 1.6 EXACT REQUIRED)
include_directories(${Poco_INCLUDE_DIRS})
link_directories(${Poco_LIBRARY_DIRS})
message(STATUS ${Poco_LIBRARY_DIRS})

add_executable(server server.cc server.h)
target_link_libraries(server ${PROJECT_NAME} -lPocoFoundation -lPocoNet -lPocoUtil)