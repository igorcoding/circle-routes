project(circle_routes)

set(SOURCE_FILES circle_routes.cc)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ../include/nn_alg.h
                                                   ../include/algorithm.h ../include/ndata.h ../include/answer.h ../include/net_bond.h ../include/prim_euler_alg.h)