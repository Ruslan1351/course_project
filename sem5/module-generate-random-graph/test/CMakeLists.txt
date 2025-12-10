make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
        NAME module-generate-random-graph-tests
        DEPENDS sc-machine::sc-builder-lib module-generate-random-graph
        INCLUDES ${EXAMPLE_MODULE_SRC_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(module-generate-random-graph-tests)
endif()
