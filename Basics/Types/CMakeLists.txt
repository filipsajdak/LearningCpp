include_directories(.)

FILE(GLOB HEADERS_LIST *.h)
FILE(GLOB SRC_LIST *.cpp) 


add_executable(Types 
               ${SRC_LIST}
               ${HEADERS_LIST}
              )

target_link_libraries(Types gmock-gtest gmock-main)
