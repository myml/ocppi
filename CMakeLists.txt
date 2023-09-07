cmake_minimum_required (
        VERSION
                3.23)

project (
        ocppi
        LANGUAGES
                CXX
        VERSION
                0.1.0
)

option (
        OCPPI_BUILD_EXAMPLES "Build examples of ocppi or not"
        ${PROJECT_IS_TOP_LEVEL}
)

option (
        OCPPI_ENABLE_TESTING "Enable test of ocppi or not"
        ${PROJECT_IS_TOP_LEVEL}
)

include (./cmake/external/CPM.cmake)

message (
        STATUS
                "Finding dependencies using CPM ..."
)

CPMFindPackage (
        NAME expected
        VERSION
                1.1.0
        GITHUB_REPOSITORY
                TartanLlama/expected
        GIT_TAG
                v1.1.0
        GIT_SHALLOW
                ON
        OPTIONS
                "EXPECTED_BUILD_TESTS OFF"
)

CPMFindPackage (
        NAME semver
        VERSION
                0.3.0
        GITHUB_REPOSITORY
                Neargye/Semver
        GIT_TAG
                v0.3.0
        GIT_SHALLOW
                ON
)

CPMFindPackage (
        NAME nlohmann_json
        VERSION
                3.11.2
        GITHUB_REPOSITORY
                nlohmann/json
        GIT_TAG
                v3.11.2
        GIT_SHALLOW
                ON
)

CPMFindPackage (
        NAME Catch2
        VERSION
                2.13.10
        GITHUB_REPOSITORY
                catchorg/Catch2
        GIT_TAG
                v2.13.10
        GIT_SHALLOW
                ON
)

CPMFindPackage (
        NAME Boost COMPONENTS headers
        VERSION
                1.74.0
        GITHUB_REPOSITORY
                "boostorg/boost"
        GIT_TAG
                "boost-1.74.0"
        GIT_SHALLOW
                ON
)

include (./cmake/GitSemver.cmake)

set (OCPPI_SEMVER ${PROJECT_VERSION})
ocppi_get_semver_from_git (OCPPI_SEMVER)

set (OCPPI_OCI_RUNTIME_SPEC_MIN "1.0.0")
set (OCPPI_OCI_RUNTIME_SPEC_MAX "1.1.0")

include (./cmake/PFL.cmake)

set (
        OCPPI_LIBS
        common
        utils
        runtime
        cli
)

set (PFL_ENABLE_TESTING ${OCPPI_ENABLE_TESTING})
set (PFL_BUILD_EXAMPLES ${OCPPI_BUILD_EXAMPLES})

foreach(LIB ${OCPPI_LIBS})
        pfl_add_subdirectory (TARGET_PREFIX ocppi libs/${LIB})
endforeach()
