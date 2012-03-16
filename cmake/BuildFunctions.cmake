#------------------------------------------------------------------------------
# DESCRIPTION:
#
# Configures the meshlib build environment, including
#   - support libraries: freetype, xerces, png, jpeg, zlib, glew, svd
#   - meshlib library and inlcude locations
#
# PROVIDES:
#
# After execution the the following variables are provided:
#   - MESHLIB_FOUND:        TRUE, if the configuration was successful
#   - MESHLIB_INCLUDE_DIRS: list of all needed include directories
#   - MESHLIB_LIBRARIES:    list of all needed libraries
#
# CONFIGURATION:
#
# You can configure the macro in setting the following (optional) variables
# before execution.
#
#   - MESHLIB_USE_DYNAMIC_GLUT: ON/OFF
#     Enables/disables the dynamic version of the GLUT library (e.g. the
#     dynamic version is needed for Qt or OpenSG applications)
#
#   - MESHLIB_SUPPORT_ROOT: location of the support libraries
#     Folder containing the support libraries in a
#       x include/
#       x lib/
#       x bin/
#     structure. It defaults to
#       x ${CMAKE_SOURCE_DIR}/../Meshlib-External
#     which is the external library directory of the meshlib project. It does
#     not follow the include, lib, bin structure, however, that case is
#     correctly handled internally.
MACRO(CONFIGURE_MESHLIB_BUILDENV)

# A convenience variable to be able to set a different project dir, if needed
SET(MESHLIB_PROJECT_DIR ${CMAKE_SOURCE_DIR}/..)

IF(NOT MESHLIB_SUPPORT_ROOT)

  #~~~~~~~~~~~~~~~~~ PLATFORM TEST ~~~~~~~~~~~~~~~~~ 
  SET(MESHLIB-EXTERNAL-TMP ${MESHLIB_PROJECT_DIR}/Meshlib-External)
  #MESSAGE("MESHLIB-EXTERNAL: " ${MESHLIB-EXTERNAL-TMP})
  IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    ## Find out whether it's a 64 or 32 bit Linux
    IF(${CMAKE_SIZEOF_VOID_P} MATCHES 8)
      SET (MESHLIB-EXTERNAL-LINK-DIRECTORY ${MESHLIB-EXTERNAL-TMP}/Linux-athlon64)
    ELSE ()
      SET (MESHLIB-EXTERNAL-LINK-DIRECTORY ${MESHLIB-EXTERNAL-TMP}/Linux-gcc3)
    ENDIF ()
    ADD_DEFINITIONS (-DWITH_OPENGL -DPCCTS_USE_NAMESPACE_STD -DLINUX -DLINUX64)
    SET (CMAKE_CXX_FLAGS "-ggdb -Wfatal-errors  -fexceptions")
  ELSEIF (WIN32)
    ADD_DEFINITIONS (-DWITH_OPENGL -DPCCTS_USE_NAMESPACE_STD)
    IF(${CMAKE_SIZEOF_VOID_P} MATCHES 4)
      SET (MESHLIB-EXTERNAL-LINK-DIRECTORY ${MESHLIB-EXTERNAL-TMP}/Win32-VC90-MTDLL)
    ELSE()
      SET (MESHLIB-EXTERNAL-LINK-DIRECTORY ${MESHLIB-EXTERNAL-TMP}/Win64)
    ENDIF()
  
    IF(MSVC)
       # /Ot optimize for time
       # /Oy omit frame pointer
       # /Ob2 more inlining
       # /GS- disable buffer check
       # /arch:SSE2 use SSE2
       # /fp:fast faster and slightly non-standard floating point operations
      SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ot /Oy /Ob2 /arch:SSE2")
      SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /arch:SSE2")
      ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ENDIF(MSVC)
  ELSEIF (APPLE)
    SET (MESHLIB-EXTERNAL "${MESHLIB-EXTERNAL-TMP}/mac-snowleopard")
    SET (MESHLIB-EXTERNAL-LINK-DIRECTORY "${MESHLIB-EXTERNAL-TMP}/lib")
    ADD_DEFINITIONS (-DWITH_OPENGL -DPCCTS_USE_NAMESPACE_STD)
  ELSE (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # blabla something else
  ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

  SET(MESHLIB_SUPPORT_LIBRARY_DIR ${MESHLIB-EXTERNAL-LINK-DIRECTORY})
  SET(MESHLIB_SUPPORT_INCLUDE_DIR ${MESHLIB_PROJECT_DIR}/Meshlib-External/include)
ELSE()
  SET(MESHLIB_SUPPORT_LIBRARY_DIR ${MESHLIB_SUPPORT_ROOT}/lib)
  SET(MESHLIB_SUPPORT_INCLUDE_DIR ${MESHLIB_SUPPORT_ROOT}/include)
ENDIF()

#IF(MESHLIB_USE_DYNAMIC_GLUT AND NOT WITH_MINIGLUT)
#    # Qt needs to be linked with a dynamic lib version of glut. Otherwise
#    # OpenGL Qt applications will crash when calling glutInit(). We take
#    # the dynamic glut library from the OpenSG support libraries.
#    FIND_LIBRARY(GLUT_LIBRARY_RELEASE
#    NAMES osgglut
#    PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
#    NO_DEFAULT_PATH)
#
#    FIND_LIBRARY(GLUT_LIBRARY_DEBUG
#    NAMES osgglutD
#    PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
#    NO_DEFAULT_PATH)
#
#    IF(GLUT_LIBRARY_RELEASE AND GLUT_LIBRARY_DEBUG)
#        SET(GLUT_LIBRARY optimized ${GLUT_LIBRARY_DEBUG}
#                         debug     ${GLUT_LIBRARY_DEBUG})
#        MESSAGE(STATUS "Using OpenSG's dynamic GLUT library: " ${GLUT_LIBRARY})
#    ELSE(GLUT_LIBRARY_RELEASE AND GLUT_LIBRARY_DEBUG)
#        FIND_PACKAGE(GLUT)
#        #MESSAGE(FATAL_ERROR "No dynamic GLUT library found! Aborting...")
#    ENDIF(GLUT_LIBRARY_RELEASE AND GLUT_LIBRARY_DEBUG)
#
#ELSE(MESHLIB_USE_DYNAMIC_GLUT AND NOT WITH_MINIGLUT)
#
#    FIND_LIBRARY(GLUT_LIBRARY
#      NAMES ${GLUT_NAMES}
#      PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
#      NO_DEFAULT_PATH)
#    IF(GLUT_LIBRARY)
#      SET(GLUT_LIBRARIES ${GLUT_LIBRARY})
#        MESSAGE(STATUS "Using Meshlib's static GLUT library: " ${GLUT_LIBRARY})
#    ELSE()
#      # Fall back to system-supplied library
#      FIND_PACKAGE(GLUT)
#      MESSAGE(STATUS "Trying to use sytem-supplied GLUT library...")
#    ENDIF()
#
#ENDIF(MESHLIB_USE_DYNAMIC_GLUT AND NOT WITH_MINIGLUT)

# TODO: add a FindFLTK.cmake file
#IF(MESHLIB_ENABLE_FLTK)
#FIND_LIBRARY (FLTK_LIBRARY fltk
#  PATHS ${LINK_DIRECTORIES}
#  NO_DEFAULT_PATH)
#
## In newer versions of FLTK, OpenGL functionality is in a separate library.
#FIND_LIBRARY (FLTK_GL_LIBRARY fltk_gl
#  PATHS ${LINK_DIRECTORIES}
#  NO_DEFAULT_PATH)
#IF(NOT FLTK_GL_LIBRARY)
#  # GL support must be in the main library.
#  SET(FLTK_GL_LIBRARY "")
#ENDIF()
#ENDIF(MESHLIB_ENABLE_FLTK)

FIND_PACKAGE(OpenGL)
FIND_LIBRARY (FREETYPE_LIBRARY
  NAMES ttf freetype
  PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
  NO_DEFAULT_PATH)

FIND_LIBRARY (XERCES_LIBRARY
  NAMES xerces-c1_4 xerces-c_1_4
  PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
  NO_DEFAULT_PATH)
IF(NOT XERCES_LIBRARY)
  # If XERCES not found, ignore it.
  SET(XERCES_LIBRARY "")
ENDIF()

# For PNG, JPEG and ZLIB, we use Meshlib-External versions if present,
# default system versions otherwise.
FIND_LIBRARY (PNG_LIBRARY png libpng
  PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
  NO_DEFAULT_PATH)
IF(NOT PNG_LIBRARY)
   FIND_LIBRARY (PNG_LIBRARY png)
ENDIF()

FIND_LIBRARY (JPEG_LIBRARY jpeg
  PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
  NO_DEFAULT_PATH)
IF(NOT JPEG_LIBRARY)
  # Fall back to system-supplied library
  FIND_LIBRARY (JPEG_LIBRARY jpeg)
ENDIF()

FIND_LIBRARY (ZLIB_LIBRARY NAMES z zlib
  PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
  NO_DEFAULT_PATH)
IF(NOT ZLIB_LIBRARY)
  # Fall back to system-supplied library
  FIND_LIBRARY (ZLIB_LIBRARY NAMES z)
ENDIF()

FIND_LIBRARY (GLEW_LIBRARY glew32s GLEW
  PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
  NO_DEFAULT_PATH)
IF(NOT GLEW_LIBRARY)
   FIND_LIBRARY (GLEW_LIBRARY GLEW)
ENDIF()

FIND_LIBRARY (SVD_LIBRARY libsvd.a
  PATHS ${MESHLIB_SUPPORT_LIBRARY_DIR}
  NO_DEFAULT_PATH)

IF(APPLE)
  SET(OTHER_LIBRARIES "-framework Carbon")
ELSEIF(WIN32)
  SET(OTHER_LIBRARIES "wsock32.lib" "rpcrt4.lib" "comctl32.lib") # comctl32.lib is needed by fltk (mtdll version)
ELSE()
  SET(OTHER_LIBRARIES pthread)
  #LIST(APPEND OTHER_LIBRARIES ${SVD_LIBRARY} )
ENDIF()        

IF(MESHLIB_ROOT)
  FIND_LIBRARY (MESHLIB_LIBRARY meshlib
    PATHS  ${MESHLIB_ROOT}
    SUFFIX lib
    NO_DEFAULT_PATH)
  IF(NOT MESHLIB_LIBRARY)
     MESSAGE("Cannot find meshlib library in " ${MESHLIB_ROOT})
  ENDIF()
ELSE()
  SET(MESHLIB_LIBRARY meshlib)
ENDIF()

SET(MESHLIB_LIBRARIES ${MESHLIB_LIBRARY}
        ${FREETYPE_LIBRARY}
        ${XERCES_LIBRARY} 
        ${OPENGL_LIBRARIES}
        ${PNG_LIBRARY}
        ${ZLIB_LIBRARY}
        ${JPEG_LIBRARY}
        ${OTHER_LIBRARIES})

IF(NOT WITH_MINIGLUT)
  LIST(APPEND MESHLIB_LIBRARIES ${GLUT_LIBRARY})
ENDIF()

SET(MESHLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}
	  ${CMAKE_SOURCE_DIR}/Meshlib
	  ${CMAKE_SOURCE_DIR}/Meshlib/Loop     
        ${MESHLIB_SUPPORT_INCLUDE_DIR})

#MESSAGE(STATUS "[CONFIGURE_MESHLIB_BUILDENV] MESHLIB_LIBRARIES:     " ${MESHLIB_LIBRARIES})
#MESSAGE(STATUS "[CONFIGURE_MESHLIB_BUILDENV] MESHLIB_INCLUDE_DIRS: " ${MESHLIB_INCLUDE_DIRS})

MARK_AS_ADVANCED(FLTK_GL_LIBRARY 
                 FLTK_LIBRARY
                 FREETYPE_LIBRARY
                 GLEW_LIBRARY
                 GLUT_LIBRARY
                 JPEG_LIBRARY
                 PNG_LIBRARY
                 SVD_LIBRARY
                 XERCES_LIBRARY
                 ZLIB_LIBRARY
                 OTHER_LIBRARIES)
      
# where are those coming from?           
#MARK_AS_ADVANCED(TSVN
#                 TSVNREV)

ENDMACRO(CONFIGURE_MESHLIB_BUILDENV)


#------------------------------------------------------------------------------
# DESCRIPTION:
#
# Configures the OpenSG build environment, including
#   - support libraries: freetype, xerces, png, jpeg, zlib, glew, svd
#   - meshlib library and inlcude locations
#
# PROVIDES:
#
# After execution the the following variables are provided:
#   - OPENSG_FOUND:        TRUE, if the configuration was successful
#   - OPENSG_INCLUDE_DIRS: list of all needed include directories
#   - OPENSG_LIBRARIES:    list of all needed libraries
MACRO(CONFIGURE_OPENSG_BUILDENV)

# Add directory containing FindXXX.cmake files to the search path for the
# FIND_PACKAGE() macro
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

#IF(OPENSG_ROOT)
# MESSAGE(STATUS "Searching for OpenSG in: " ${OPENSG_ROOT})
#ELSE()
# MESSAGE(STATUS "Searching for OpenSG libs in:     " ${OPENSG_LIBRARY_DIR})
# MESSAGE(STATUS "Searching for OpenSG includes in: " ${OPENSG_INCLUDE_DIR})
#ENDIF()

# the list of OpenSG libraries needed
SET(OSG_COMPONENTS OSGBase
                   OSGDrawable
                   OSGGroup
                   OSGState
                   OSGSystem
                   OSGUtil
                   OSGWindow
                   OSGCluster
                   OSGDynamics)

# append the platform window library
IF(UNIX)
    IF(APPLE)
        LIST(APPEND OSG_COMPONENTS OSGWindowCarbon)
    ELSE(APPLE)
        LIST(APPEND OSG_COMPONENTS OSGWindowX)
    ENDIF(APPLE)
ENDIF(UNIX)

IF(WIN32)
    LIST(APPEND OSG_COMPONENTS OSGWindowWIN32)
ENDIF(WIN32)

#MESSAGE(STATUS "[Test] OPENSG_ROOT:        " ${OPENSG_ROOT})
#MESSAGE(STATUS "[Test] OPENSG_INCLUDE_DIR: " ${OPENSG_INCLUDE_DIR})

FIND_PACKAGE(OpenSG REQUIRED COMPONENTS ${OSG_COMPONENTS})
IF(OpenSG_FOUND)
  INCLUDE_DIRECTORIES(${OpenSG_INCLUDE_DIRS})
  LIST(APPEND OTHER_LIBRARIES  ${OpenSG_LIBRARIES})

  #MESSAGE(STATUS "Found OpenSG libs: " ${OpenSG_LIBRARIES})
  #MESSAGE(STATUS "Found OpenSG includes: " ${OpenSG_INCLUDE_DIRS})
ENDIF()

# Restore OPENSG_ROOT variable in case
#IF(OPENSG_TMP_ROOT)
#    SET(OPENSG_ROOT ${OPENSG_TMP_ROOT}
#        CACHE PATH "Directory of the OpenSG installation" FORCE)
#ENDIF()

ENDMACRO(CONFIGURE_OPENSG_BUILDENV)

#------------------------------------------------------------------------------
# DESCRIPTION:
#
# Configures the OSGrimaldo library
MACRO(CONFIGURE_OSGRIMALDO_BUILDENV)

SET(OSGRIMALDO_LIBRARIES   OSGrimaldo)
SET(OSGRIMALDO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Contrib/OSGrimaldo)

ENDMACRO(CONFIGURE_OSGRIMALDO_BUILDENV)
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# DESCRIPTION:
#
#------------------------------------------------------------------------------
# DESCRIPTION:
#
# Configures the QGrimaldoWidget library
MACRO(CONFIGURE_QGRIMALDOWIDGET_BUILDENV)

SET(QGRIMALDOWIDGET_LIBRARIES   QGrimaldoWidget)
SET(QGRIMALDOWIDGET_INCLUDE_DIR ${Meshlib_SOURCE_DIR}/Contrib/QGrimaldoWidget/src)

ENDMACRO(CONFIGURE_QGRIMALDOWIDGET_BUILDENV)
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# DESCRIPTION:
#
# Sets the warning level for the MSVC compiler
MACRO(MSVC_WARNING_LEVEL LEVEL)
  STRING(REPLACE "W3"  "W${LEVEL}" CMAKE_CXX_FLAGS       ${CMAKE_CXX_FLAGS})
  STRING(REPLACE "W3"  "W${LEVEL}" CMAKE_C_FLAGS         ${CMAKE_C_FLAGS})
ENDMACRO(MSVC_WARNING_LEVEL)
