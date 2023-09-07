pfl_add_library (
        INS
        include/ocppi/common/configure.hpp.in
        SOURCES
        src/ocppi/common/configure.cpp
        src/ocppi/common/InvalidArgumentError.cpp
        HEADERS
        include/ocppi/common/InvalidArgumentError.hpp
        COMPILE_FEATURES
        PUBLIC
        cxx_std_17
        LINK_LIBRARIES
        PUBLIC
        tl::expected
)
