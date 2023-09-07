pfl_add_library (
        HEADER_ONLY
        VERSION
        ${PROJECT_VERSION}
        INS
        include/ocppi/common/configure.hpp.in
        SOURCES
        src/ocppi/common/configure.cpp
        src/ocppi/common/InvalidArgumentError.cpp
        HEADERS
        include/ocppi/common/InvalidArgumentError.hpp
        COMPILE_FEATURES
        INTERFACE
        cxx_std_17
        LINK_LIBRARIES
        INTERFACE
        tl::expected
)
