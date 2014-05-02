# This script locates the STP C++ Library
# Based on the FindSFML.cmake module
# ========================================
#
# Usage
# -----
#
# STP's libraries use the same configuration (static/dynamic linking) as SFML's.
# If the variable SFML_STATIC_LIBRARIES is set to TRUE before calling find_package(STP),
# the static libraries of STP are found. Otherwise, the dynamic ones are found.
#
# If STP is not installed in a standard path, you can use the STP_ROOT CMake (or environment) variable
# to tell CMake where STP is.
#
# Output
# ------
#
# This script defines the following variables:
# - STP_LIBRARY_DEBUG:   Name of the debug library (set to STP_LIBRARY_RELEASE if no debug version is found)
# - STP_LIBRARY_RELEASE: Name of the release library (set to STP_LIBRARY_DEBUG if no release version is found)
# - STP_LIBRARY:         Name of the library to link to (includes both debug and release names if necessary)
# - STP_INCLUDE_DIR:     The path where STP headers are located (the directory containing the STP/Config.hpp file)
# - STP_FOUND:           Is TRUE if at least one of the debug or release library is found
#
# Usage example:
# 
#   find_package(STP 2 REQUIRED)
#   include_directories(${STP_INCLUDE_DIR})
#   target_link_libraries(MyProject ${STP_LIBRARY})


# Deduce the libraries suffix from the options
set(FINDSTP_LIB_SUFFIX "")
if(SFML_STATIC_LIBRARIES)
    set(FINDSTP_LIB_SUFFIX "-s")
endif()

# Find the STP include directory
find_path(STP_INCLUDE_DIR STP/Config.hpp
          PATH_SUFFIXES include
          PATHS
          ${STP_ROOT}
          $ENV{STP_ROOT}
          /usr/local/
          /usr/
          /sw          # Fink
          /opt/local/  # DarwinPorts
          /opt/csw/    # Blastwave
          /opt/)

# Find the requested modules
set(FINDSTP_LIB_PATHS
    ${STP_ROOT}
    $ENV{STP_ROOT}
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# Debug library
find_library(STP_LIBRARY_DEBUG
             NAMES STP${FINDSTP_LIB_SUFFIX}-d
             PATH_SUFFIXES lib64 lib
             PATHS ${FINDSTP_LIB_PATHS})

# Release library
find_library(STP_LIBRARY_RELEASE
             NAMES STP${FINDSTP_LIB_SUFFIX}
             PATH_SUFFIXES lib64 lib
             PATHS ${FINDSTP_LIB_PATHS})

if(STP_LIBRARY_DEBUG OR STP_LIBRARY_RELEASE)
    # If both are found, set STP_LIBRARY to contain both
    if(STP_LIBRARY_DEBUG AND STP_LIBRARY_RELEASE)
        set(STP_LIBRARY debug   ${STP_LIBRARY_DEBUG}
                         optimized ${STP_LIBRARY_RELEASE})
    endif()

    # If only one debug/release variant is found, set the other to be equal to the found one
    if(STP_LIBRARY_DEBUG AND NOT STP_LIBRARY_RELEASE)
        # debug and not release
        set(STP_LIBRARY_RELEASE ${STP_LIBRARY_DEBUG})
        set(STP_LIBRARY         ${STP_LIBRARY_DEBUG})
    endif()
    if(STP_LIBRARY_RELEASE AND NOT STP_LIBRARY_DEBUG)
        # release and not debug
        set(STP_LIBRARY_DEBUG ${STP_LIBRARY_RELEASE})
        set(STP_LIBRARY       ${STP_LIBRARY_RELEASE})
    endif()
endif()

# handle the QUIETLY and REQUIRED arguments and set STP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(STP DEFAULT_MSG
                                  STP_LIBRARY STP_INCLUDE_DIR)

# Mark variables as advanced
mark_as_advanced(STP_INCLUDE_DIR
                 STP_LIBRARY
                 STP_LIBRARY_RELEASE
                 STP_LIBRARY_DEBUG)
