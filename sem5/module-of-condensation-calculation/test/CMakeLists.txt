make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
        NAME module-of-condensation-calculation-tests
        DEPENDS sc-machine::sc-builder-lib module-of-condensation-calculation
        INCLUDES ${EXAMPLE_MODULE_SRC_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(module-of-condensation-calculation-tests)
endif()
