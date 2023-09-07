get_property (
        PFL_INITIALIZED GLOBAL ""
        PROPERTY
                PFL_INITIALIZED
        SET
)
if(PFL_INITIALIZED)
        return ()
endif()

function(pfl_add_subdirectory)
        cmake_parse_arguments (
                PFL_ADD_SUBDIRECTORY
                ""
                "TARGET_PREFIX"
                ""
                ${ARGN}
        )
        set (PFL_PREFIX ${PFL_ADD_SUBDIRECTORY_TARGET_PREFIX})
        add_subdirectory (${PFL_ADD_SUBDIRECTORY_UNPARSED_ARGUMENTS})
endfunction()

function(pfl_configure_files)
        cmake_parse_arguments (
                PFL_CONFIGURE_FILES
                ""
                "HEADERS;SOURCES"
                "INS"
                ${ARGN}
        )

        foreach(IN_FILE ${PFL_CONFIGURE_FILES_INS})
                string (
                        REGEX
                        REPLACE
                                "\.in$" "" OUT_FILE ${IN_FILE}
                )

                configure_file (
                        ${IN_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}
                        @ONLY
                )

                if(OUT_FILE MATCHES "^include\/.*\.h(h|pp)?$")
                        list (
                                APPEND ${PFL_CONFIGURE_FILES_HEADERS}
                                ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}
                        )
                elseif(OUT_FILE MATCHES "^src\/.*\.(h(h|pp)?|cpp)$")
                        list (
                                APPEND ${PFL_CONFIGURE_FILES_SOURCES}
                                ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}
                        )
                else()
                        # TODO: add other files later
                endif()
        endforeach()

        set (
                ${PFL_CONFIGURE_FILES_HEADERS}
                ${${PFL_CONFIGURE_FILES_HEADERS}}
                PARENT_SCOPE
        )

        set (
                ${PFL_CONFIGURE_FILES_SOURCES}
                ${${PFL_CONFIGURE_FILES_SOURCES}}
                PARENT_SCOPE
        )
endfunction()

function(pfl_add_library)
        message (
                STATUS
                        "PFL: Building library at"
                        "${CMAKE_CURRENT_SOURCE_DIR} ..."
        )

        cmake_parse_arguments (
                PFL_ADD_LIBRARY
                "HEADER_ONLY;INTERNAL;NO_INSTALL"
                "OUTPUT_NAME;SOVERSION;VERSION;TYPE"
                "INS;SOURCES;HEADERS;LINK_LIBRARIES;COMPILE_FEATURES;APPS;EXAMPLES"
                ${ARGN}
        )

        cmake_path (
                GET
                CMAKE_CURRENT_SOURCE_DIR
                FILENAME
                TARGET_DIR_NAME
        )

        string (
                REPLACE
                        " " "_" TARGET_NAME "${TARGET_DIR_NAME}"
        )

        string (
                REPLACE
                        "::" "__" TARGET_PREFIX "${PFL_PREFIX}"
        )

        if(TARGET_PREFIX)
                set (TARGET_NAME "${TARGET_PREFIX}__${TARGET_NAME}")
        endif()

        string (
                REPLACE
                        "__" "::" TARGET_EXPORT_NAME "${TARGET_NAME}"
        )

        message (
                STATUS
                        "PFL: Adding library ${TARGET_EXPORT_NAME} ..."
        )

        pfl_configure_files (
                INS
                ${PFL_ADD_LIBRARY_INS}
                HEADERS
                PFL_ADD_LIBRARY_HEADERS
                SOURCES
                PFL_ADD_LIBRARY_SOURCES
        )

        if(PFL_ADD_LIBRARY_HEADER_ONLY)
                add_library ("${TARGET_NAME}" INTERFACE)
                add_library ("${TARGET_NAME}__BUILD" ${PFL_ADD_LIBRARY_SOURCES})
                target_link_libraries (
                        "${TARGET_NAME}__BUILD" PUBLIC
                                                        "${TARGET_NAME}"
                )
        else()
                add_library (
                        "${TARGET_NAME}" ${PFL_ADD_LIBRARY_TYPE}
                                         ${PFL_ADD_LIBRARY_SOURCES}
                )
        endif()

        if(PFL_ADD_LIBRARY_SOVERSION AND NOT PFL_ADD_LIBRARY_HEADER_ONLY)
                set_target_properties (
                        "${TARGET_NAME}"
                        PROPERTIES
                                SOVERSION ${PFL_ADD_LIBRARY_SOVERSION}
                )
        endif()

        add_library ("${TARGET_EXPORT_NAME}" ALIAS "${TARGET_NAME}")

        set_target_properties (
                "${TARGET_NAME}" PROPERTIES
                                         EXPORT_NAME ${TARGET_DIR_NAME}
        )

        if(PFL_ADD_LIBRARY_OUTPUT_NAME)
                set_target_properties (
                        "${TARGET_NAME}"
                        PROPERTIES
                                OUTPUT_NAME ${PFL_ADD_LIBRARY_OUTPUT_NAME}
                )
        endif()

        target_sources (
                ${TARGET_NAME}
                PUBLIC
                        FILE_SET HEADERS BASE_DIRS include
                        ${CMAKE_CURRENT_BINARY_DIR}/include FILES
                        ${PFL_ADD_LIBRARY_HEADERS}
        )

        if(NOT PFL_ADD_LIBRARY_HEADER_ONLY)
                target_include_directories (
                        ${TARGET_NAME}
                        PRIVATE
                                src ${CMAKE_CURRENT_BINARY_DIR}/src
                )
        endif()

        if(PFL_ADD_LIBRARY_LINK_LIBRARIES)
                target_link_libraries (
                        ${TARGET_NAME} ${PFL_ADD_LIBRARY_LINK_LIBRARIES}
                )
        endif()

        if(PFL_ADD_LIBRARY_COMPILE_FEATURES)
                target_compile_features (
                        ${TARGET_NAME} ${PFL_ADD_LIBRARY_COMPILE_FEATURES}
                )
        endif()

        if(NOT PFL_ADD_LIBRARY_NO_INSTALL AND NOT PFL_ADD_LIBRARY_INTERNAL)
                include (GNUInstallDirs)

                install (
                        TARGETS
                                "${TARGET_NAME}"
                        EXPORT
                                "${TARGET_NAME}"
                        FILE_SET HEADERS
                )

                install (
                        EXPORT
                                "${TARGET_NAME}"
                        DESTINATION
                                ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}
                        FILE "${TARGET_NAME}.cmake"
                        NAMESPACE
                                ${PFL_PREFIX}::
                )

                if(EXISTS
                   ${CMAKE_CURRENT_SOURCE_DIR}/misc/cmake/${TARGET_NAME}-config.cmake.in
                )
                        include (CMakePackageConfigHelpers)
                        # This will be used to replace @PACKAGE_cmakeModulesDir@
                        set (cmakeModulesDir cmake)

                        configure_package_config_file (
                                misc/cmake/${TARGET_NAME}-config.cmake.in
                                misc/cmake/${TARGET_NAME}-config.cmake
                                INSTALL_DESTINATION
                                        ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}
                                PATH_VARS
                                        cmakeModulesDir
                                NO_SET_AND_CHECK_MACRO
                                NO_CHECK_REQUIRED_COMPONENTS_MACRO
                        )

                        write_basic_package_version_file (
                                ${CMAKE_CURRENT_BINARY_DIR}/misc/cmake/${TARGET_NAME}-config-version.cmake
                                VERSION
                                        ${PFL_ADD_LIBRARY_VERSION}
                                COMPATIBILITY
                                        SameMajorVersion
                        )

                        install (
                                FILES
                                        ${CMAKE_CURRENT_BINARY_DIR}/misc/cmake/${TARGET_NAME}-config.cmake
                                        ${CMAKE_CURRENT_BINARY_DIR}/misc/cmake/${TARGET_NAME}-config-version.cmake
                                DESTINATION
                                        ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}
                        )
                endif()
        endif()

        if(EXISTS tests AND PFL_ENABLE_TESTING)
                add_subdirectory (tests)
        endif()

        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples"
           AND PFL_ADD_LIBRARY_EXAMPLES
           AND PFL_BUILD_EXAMPLES
        )
                foreach(EXAMPLE ${PFL_ADD_LIBRARY_EXAMPLES})
                        add_subdirectory ("examples/${EXAMPLE}")
                endforeach()
        endif()

        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/apps" AND PFL_ADD_LIBRARY_APPS)
                foreach(APP ${PFL_ADD_LIBRARY_APP})
                        add_subdirectory ("${APP}")
                endforeach()
        endif()
endfunction()

function(pfl_add_executable)
        message (
                STATUS
                        "PFL: Building executable at"
                        "${CMAKE_CURRENT_SOURCE_DIR} ..."
        )

        cmake_parse_arguments (
                PFL_ADD_EXECUTABLE
                "LIBEXEC;INTERNAL;NO_INSTALL"
                "OUTPUT_NAME"
                "INS;SOURCES;HEADERS;LINK_LIBRARIES;COMPILE_FEATURES"
                ${ARGN}
        )

        cmake_path (
                GET
                CMAKE_CURRENT_SOURCE_DIR
                FILENAME
                TARGET_DIR_NAME
        )

        string (
                REPLACE
                        " " "_" TARGET_NAME "${TARGET_DIR_NAME}"
        )

        string (
                REPLACE
                        "::" "__" TARGET_PREFIX "${PFL_PREFIX}"
        )

        if(TARGET_PREFIX)
                set (TARGET_NAME "${TARGET_PREFIX}__${TARGET_NAME}")
        endif()

        message (
                STATUS
                        "PFL: Adding executable ${TARGET_NAME} ..."
        )

        pfl_configure_files (
                INS
                ${PFL_ADD_EXECUTABLE_INS}
                HEADERS
                PFL_ADD_EXECUTABLE_HEADERS
                SOURCES
                PFL_ADD_EXECUTABLE_SOURCES
        )

        add_executable ("${TARGET_NAME}" ${PFL_ADD_EXECUTABLE_SOURCES})

        if(PFL_ADD_EXECUTABLE_OUTPUT_NAME)
                set_target_properties (
                        "${TARGET_NAME}"
                        PROPERTIES
                                OUTPUT_NAME ${PFL_ADD_EXECUTABLE_OUTPUT_NAME}
                )
        endif()

        target_sources (
                ${TARGET_NAME}
                PUBLIC
                        FILE_SET HEADERS BASE_DIRS include
                        ${CMAKE_CURRENT_BINARY_DIR}/include FILES
                        ${PFL_ADD_EXECUTABLE_HEADERS}
        )

        target_include_directories (
                ${TARGET_NAME} PRIVATE
                                       src ${CMAKE_CURRENT_BINARY_DIR}/src
        )

        if(PFL_ADD_EXECUTABLE_LINK_LIBRARIES)
                target_link_libraries (
                        ${TARGET_NAME} ${PFL_ADD_EXECUTABLE_LINK_LIBRARIES}
                )
        endif()

        if(PFL_ADD_EXECUTABLE_COMPILE_FEATURES)
                target_compile_features (
                        ${TARGET_NAME} ${PFL_ADD_EXECUTABLE_COMPILE_FEATURES}
                )
        endif()

        if(PFL_ADD_EXECUTABLE_NO_INSTALL OR PFL_ADD_EXECUTABLE_INTERNAL)
                return ()
        endif()

        if(PFL_ADD_EXECUTABLE_LIBEXEC)
                include (GNUInstallDirs)

                install (
                        ${TARGET_NAME} DESTINATION
                        ${CMAKE_INSTALL_LIBEXECDIR}/${TARGET_PREFIX}
                )
        endif()
endfunction()
