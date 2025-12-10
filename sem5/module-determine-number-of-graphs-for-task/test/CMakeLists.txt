make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
        NAME module-determine-number-of-graphs-for-task-tests
        DEPENDS sc-machine::sc-builder-lib module-determine-number-of-graphs-for-task
        INCLUDES ${EXAMPLE_MODULE_SRC_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(module-determine-number-of-graphs-for-task-tests)
endif()
