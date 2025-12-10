file(GLOB SOURCES CONFIGURE_DEPENDS
        "*.cpp" "*.hpp"
        "agent/*.cpp" "agent/*.hpp"
        "keynodes/*.hpp"
)

add_library(module-generate-task-for-student SHARED ${SOURCES})
target_link_libraries(module-generate-task-for-student
        LINK_PUBLIC sc-machine::sc-memory
)
target_include_directories(module-generate-task-for-student
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(module-generate-task-for-student PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/extensions)

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(module-generate-task-for-student)
endif ()

if (${SC_BUILD_TESTS})
    set(MODULE-GENERATE-TASK-FOR-STUDENT_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif ()
