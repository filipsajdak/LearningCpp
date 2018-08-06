
include_directories(SYSTEM include)

SET(HEADERS_LIST include/tinyxml2.h)

add_library(tinyxml2 STATIC src/tinyxml2.cpp ${HEADERS_LIST})
target_include_directories (tinyxml2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
