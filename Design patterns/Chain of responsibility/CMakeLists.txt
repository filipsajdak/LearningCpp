include_directories(.)

FILE(GLOB HEADERS_LIST *.h)
FILE(GLOB SRC_LIST *.cpp)

add_executable(ChainOfResponsibility 
               ${SRC_LIST}
               ${HEADERS_LIST}
              )

target_link_libraries(ChainOfResponsibility gmock-gtest gmock-main)
