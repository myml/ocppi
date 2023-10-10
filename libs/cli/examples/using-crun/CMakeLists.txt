find_package(
  PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET systemd)

pfl_add_executable(
  INTERNAL
  SOURCES
  src/main.cpp
  LINK_LIBRARIES
  PUBLIC
  ocppi::cli
  PRIVATE
  systemd)
