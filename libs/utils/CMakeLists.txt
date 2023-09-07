pfl_add_library (
        INTERNAL
        HEADERS
        include/ocppi/utils/ewrap/EWRAP.hpp
        SOURCES
        src/ocppi/utils/ewrap/EWRAP.cpp
        LINK_LIBRARIES
        PUBLIC
        ocppi::common
)
