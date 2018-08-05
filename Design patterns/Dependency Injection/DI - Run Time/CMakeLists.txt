include_directories(.)

FILE(GLOB HEADERS_LIST *.h)
FILE(GLOB SRC_LIST *.cpp)

add_executable(DI-RunTime 
               ${SRC_LIST}
               ${HEADERS_LIST}
              )

target_link_libraries(DI-RunTime DI-libs gmock-gtest gmock-main)
