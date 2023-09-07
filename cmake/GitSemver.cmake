get_property (
        GITSEMVER_INITIALIZED GLOBAL ""
        PROPERTY
                GITSEMVER_INITIALIZED
        SET
)
if(GITSEMVER_INITIALIZED)
        return ()
endif()

function(GitSemver varname)
        if(NOT PROJECT_IS_TOP_LEVEL)
                return ()
        endif()

        message (
                STATUS
                        "GitSemver: Getting version from git repository ..."
        )

        find_package (Git REQUIRED)
        find_program (
                SED_EXECUTABLE NAMES
                                       sed REQUIRED
        )
        execute_process (
                COMMAND
                        ${GIT_EXECUTABLE} describe --tags --long --dirty
                COMMAND
                        ${SED_EXECUTABLE} -e
                        s/-\\\([[:digit:]]\\+\\\)-g/+\\1\\./
                COMMAND
                        ${SED_EXECUTABLE} -e s/-dirty\$/\\.dirty/
                COMMAND
                        ${SED_EXECUTABLE} -e s/+0\\.[^\\.]\\+\\.\\?/+/
                COMMAND
                        ${SED_EXECUTABLE} -e s/^v//
                COMMAND
                        ${SED_EXECUTABLE} -e s/+\$//
                OUTPUT_VARIABLE
                        ${varname} RESULTS_VARIABLE rets
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
                COMMAND_ECHO
                STDOUT
        )

        foreach(ret ${rets})
                if(NOT
                   ret
                   EQUAL
                   0
                )
                        message (
                                STATUS
                                        "GitSemver: Failed."
                        )
                        return ()
                endif()
        endforeach()

        message (
                STATUS
                        "GitSemver: ${${varname}}"
        )

        set (
                ${varname}
                ${${varname}}
                PARENT_SCOPE
        )
endfunction()
