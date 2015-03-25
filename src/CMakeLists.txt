project(circle_routes)

set(SOURCE_FILES circle_routes.cc)
set(HEADER_FILES ../include/nn_alg.h
                ../include/algorithm.h
                ../include/ndata.h
                ../include/answer.h
                ../include/net_bond.h
                ../include/prim_euler_alg.h
                ../include/little_alg.h)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ../include/nearest_alg.h)