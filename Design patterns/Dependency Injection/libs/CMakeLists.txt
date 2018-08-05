
include_directories(include)

SET(HEADERS_LIST 
	include/FileLogger.h 
	include/User.h 
	include/UsersDataBase.h 
	include/UsersFileDataBase.h )

add_library(DI-libs STATIC src/UsersFileDataBase.cpp ${HEADERS_LIST})

target_include_directories(DI-libs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)