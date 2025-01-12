find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_MIKES_OOT gnuradio-mikes_oot)

FIND_PATH(
    GR_MIKES_OOT_INCLUDE_DIRS
    NAMES gnuradio/mikes_oot/api.h
    HINTS $ENV{MIKES_OOT_DIR}/include
        ${PC_MIKES_OOT_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_MIKES_OOT_LIBRARIES
    NAMES gnuradio-mikes_oot
    HINTS $ENV{MIKES_OOT_DIR}/lib
        ${PC_MIKES_OOT_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-mikes_ootTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_MIKES_OOT DEFAULT_MSG GR_MIKES_OOT_LIBRARIES GR_MIKES_OOT_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_MIKES_OOT_LIBRARIES GR_MIKES_OOT_INCLUDE_DIRS)
