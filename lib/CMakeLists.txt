SET(ARGCV_VERSION 1.0)
SET(ARGCV_SOVERSION 0.1)

################### UTIL EXT : GRAPH
AUX_SOURCE_DIRECTORY(. LIBARGCV_SRC)
ADD_LIBRARY(argcv SHARED ${LIBARGCV_SRC})
SET_TARGET_PROPERTIES(argcv PROPERTIES VERSION ${ARGCV_VERSION} SOVERSION ${ARGCV_SOVERSION})

ADD_LIBRARY(argcv_static STATIC ${LIBARGCV_SRC})
SET_TARGET_PROPERTIES(argcv_static PROPERTIES OUTPUT_NAME "argcv")

#ADD_DEPENDENCIES(argcv leveldb)
#ADD_DEPENDENCIES(argcv crfpp)
#ADD_DEPENDENCIES(argcv mongo_cxx_static)

#ADD_DEPENDENCIES(argcv_static leveldb_static)
#ADD_DEPENDENCIES(argcv_static crfpp_static)
#ADD_DEPENDENCIES(argcv_static mongo_cxx_static)

#TARGET_LINK_LIBRARIES(argcv leveldb)
#TARGET_LINK_LIBRARIES(argcv crfpp)
#TARGET_LINK_LIBRARIES(argcv mongo_cxx_static)
#TARGET_LINK_LIBRARIES(argcv pthread)

#TARGET_LINK_LIBRARIES(argcv_static leveldb_static)
#TARGET_LINK_LIBRARIES(argcv_static crfpp_static)
#TARGET_LINK_LIBRARIES(argcv_static mongo_cxx_static)
#TARGET_LINK_LIBRARIES(argcv_static pthread)

# INSTALL (TARGETS argcv_util argcv_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcv/util.h DESTINATION include/argcv/ )



