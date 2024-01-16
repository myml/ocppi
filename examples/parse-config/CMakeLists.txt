find_package(
  PkgConfig REQUIRED)
pkg_check_modules(systemd REQUIRED IMPORTED_TARGET libsystemd)

pfl_add_executable(
  INTERNAL
  SOURCES
  ./src/main.cpp
  COMPILE_FEATURES
  PUBLIC
  cxx_std_17
  LINK_LIBRARIES
  PUBLIC
  ocppi::ocppi
  PkgConfig::systemd
  spdlog::spdlog)
