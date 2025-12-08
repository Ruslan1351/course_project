file(GLOB SOURCES CONFIGURE_DEPENDS
        "*.cpp" "*.hpp"
        "agent/*.cpp" "agent/*.hpp"
        "keynodes/*.hpp"
)

add_library(module-generate-random-graph SHARED ${SOURCES})
target_link_libraries(module-generate-random-graph
        LINK_PUBLIC sc-machine::sc-memory
)
target_include_directories(module-generate-random-graph
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(module-generate-random-graph PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/extensions)

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(module-generate-random-graph)
endif ()

if (${SC_BUILD_TESTS})
    set(EXAMPLE_MODULE_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif ()
