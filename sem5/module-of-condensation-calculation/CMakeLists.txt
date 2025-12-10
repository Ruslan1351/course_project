file(GLOB SOURCES CONFIGURE_DEPENDS
        "*.cpp" "*.hpp"
        "agent/*.cpp" "agent/*.hpp"
        "keynodes/*.hpp"
)

add_library(module-of-condensation-calculation SHARED ${SOURCES})
target_link_libraries(module-of-condensation-calculation
        LINK_PUBLIC sc-machine::sc-memory
)
target_include_directories(module-of-condensation-calculation
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(module-of-condensation-calculation PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/extensions)

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(module-of-condensation-calculation)
endif ()

if (${SC_BUILD_TESTS})
    set(MODULE-OF-CONDENSATION-CALCULATION_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif ()
