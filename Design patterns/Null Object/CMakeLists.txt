include_directories(.)

FILE(GLOB HEADERS_LIST *.h)
FILE(GLOB SRC_LIST *.cpp)

add_executable(NullObject 
               ${SRC_LIST}
               ${HEADERS_LIST}
              )

target_link_libraries(NullObject gmock-gtest gmock-main)
