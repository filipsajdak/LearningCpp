include_directories(.)

FILE(GLOB HEADERS_LIST *.h)
FILE(GLOB SRC_LIST *.cpp)

add_executable(DI-StdFunction 
               ${SRC_LIST}
               ${HEADERS_LIST}
              )

target_link_libraries(DI-StdFunction DI-libs gmock-gtest gmock-main)
