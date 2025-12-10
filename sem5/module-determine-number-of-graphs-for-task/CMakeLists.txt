file(GLOB SOURCES CONFIGURE_DEPENDS
        "*.cpp" "*.hpp"
        "agent/*.cpp" "agent/*.hpp"
        "keynodes/*.hpp"
)

add_library(module-determine-number-of-graphs-for-task SHARED ${SOURCES})
target_link_libraries(module-determine-number-of-graphs-for-task
        LINK_PUBLIC sc-machine::sc-memory
)
target_include_directories(module-determine-number-of-graphs-for-task
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(module-determine-number-of-graphs-for-task PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/extensions)

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(module-determine-number-of-graphs-for-task)
endif ()

if (${SC_BUILD_TESTS})
    set(MODULE-DETERMINE-NUMBER-OF-GRAPHS-FOR-TASK_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif ()
