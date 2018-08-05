
include_directories(SYSTEM include)

SET(HEADERS_LIST include/gtest/gtest.h include/gmock/gmock.h)

add_library(gmock-gtest STATIC src/gmock-gtest-all.cc ${HEADERS_LIST})
target_include_directories (gmock-gtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(gmock-gtest PUBLIC -DGTEST_STDLIB_CXX11 -DGTEST_LANG_CXX11)

add_library(gmock-main STATIC src/gmock_main.cc ${HEADERS_LIST})
add_dependencies(gmock-main gmock-gtest)
target_compile_definitions(gmock-main PUBLIC -DGTEST_STDLIB_CXX11 -DGTEST_LANG_CXX11)

if (${UNIX})
	target_link_libraries(gmock-main pthread)
endif (${UNIX})