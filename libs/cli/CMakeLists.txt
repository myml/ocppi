if(NOT TARGET Boost::process)
  add_library(Boost::process ALIAS Boost::headers)
endif()

pfl_add_library(
  INTERNAL # cli is not ready for publish
  SOVERSION
  ${OCPPI_SOVERSION}
  VERSION
  ${PROJECT_VERSION}
  TYPE
  ${OCPPI_LIBRARY_TYPE}
  HEADERS
  include/ocppi/cli/CLI.hpp
  include/ocppi/cli/CommandFailedError.hpp
  include/ocppi/cli/crun/Crun.hpp
  include/ocppi/cli/runc/Runc.hpp
  include/ocppi/cli/youki/Youki.hpp
  SOURCES
  src/ocppi/cli/CLI.cpp
  src/ocppi/cli/CommandFailedError.cpp
  src/ocppi/cli/crun/Crun.cpp
  src/ocppi/cli/runc/Runc.cpp
  src/ocppi/cli/youki/Youki.cpp
  EXAMPLES
  using-crun
  COMPILE_FEATURES
  PUBLIC
  cxx_std_17
  LINK_LIBRARIES
  PUBLIC
  ocppi::runtime
  tl::expected
  spdlog::spdlog
  fmt::fmt
  PRIVATE
  Boost::process)
