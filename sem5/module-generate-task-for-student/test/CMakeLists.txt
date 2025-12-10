make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
        NAME module-generate-task-for-student-tests
        DEPENDS sc-machine::sc-builder-lib module-generate-task-for-student
        INCLUDES ${COMPARE_STRUCTURES_MODULE_SRC_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(module-generate-task-for-student-tests)
endif()
