# - Try to find STP
# Once done this will define
#
# JANSSON_FOUND - system has Jansson
# JANSSON_INCLUDE_DIRS - the Jansson include directory
# JANSSON_LIBRARIES - Link these to use Jansson

find_package(PkgConfig)
pkg_check_modules(PC_JANSSON QUIET jansson)

find_path(JANSSON_INCLUDE_DIR jansson.h
          HINTS ${PC_JANSSON_INCLUDEDIR} ${PC_JANSSON_INCLUDE_DIRS}
          PATH_SUFFIXES JANSSON)

find_library(JANSSON_LIBRARY NAMES jansson
             HINTS ${PC_JANSSON_LIBDIR} ${PC_JANSSON_LIBRARY_DIRS})

set(JANSSON_LIBRARIES ${JANSSON_LIBRARY})
set(JANSSON_INCLUDE_DIRS ${JANSSON_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set JANSSON_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(JANSSON DEFAULT_MSG
                                  JANSSON_LIBRARY JANSSON_INCLUDE_DIR)

mark_as_advanced(JANSSON_INCLUDE_DIR JANSSON_LIBRARY)
