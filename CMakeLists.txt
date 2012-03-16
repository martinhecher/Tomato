#-------------------------------------------------------------------------------
# CMakeLists.txt for testAnimation
#
# Author:        Martin Hecher <martin.hecher@fraunhofer.at>
# Last modified: 2012-03-16
#-------------------------------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Tomato)

#-------------------------------------------------------------------------------
# CMake settings:
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/BuildFunctions.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/CMakeConfig.txt)
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Project settings:
SET(TARGET        testSimple)
SET(LABEL         ${TARGET})
SET(DEBUG_POSTFIX D)

SET(MSVC_IGNORE_LIBRARY_FLAGS "/NODEFAULTLIB:libc
                               /NODEFAULTLIB:libcmt
                               /NODEFAULTLIB:freeglut")
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Define source files:
FILE(GLOB SRCFILES src/*.cpp 
                   src/*.hpp     
                   src/*.inl
		           src/*.fcd
                   src/*.h)     
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Compiler flags:
IF(MSVC)
  ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
ENDIF()
#-------------------------------------------------------------------------------

SET(DEP_INCLUDE_DIRS)
SET(DEP_LIBRARIES)

#-------------------------------------------------------------------------------
# Add project internal dependencies:
#LIST(APPEND DEP_INCLUDE_DIRS ${OSGrimaldo_SOURCE_DIR})
#LIST(APPEND DEP_LIBRARIES ${DEPENDENCIES})
LINK_DIRECTORIES(${BOOST_ROOT}/lib)
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Configure the OpenSG build environment:
SET(OSG_COMPONENTS OSGBase
                   OSGCluster
                   OSGDrawable
                   OSGFileIO
                   OSGGroup
                   OSGImageFileIO
                   OSGEffectGroups
                   OSGState
                   OSGSystem
                   OSGUtil
                   OSGText
                   OSGWindow                   
		   OSGWindowGLUT)

FIND_PACKAGE(OpenSG REQUIRED COMPONENTS ${OSG_COMPONENTS})
IF(OpenSG_FOUND)
  LIST(APPEND DEP_INCLUDE_DIRS ${OpenSG_INCLUDE_DIRS})
  LIST(APPEND DEP_LIBRARIES    ${OpenSG_LIBRARIES})
ENDIF()
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Configure Boost:
SET(Boost_USE_MULTITHREAD ON)
SET(Boost_STATIC_LIBS ON)

FIND_PACKAGE(Boost REQUIRED)

IF(Boost_FOUND AND Boost_MINOR_VERSION GREATER "34")
    FIND_PACKAGE(Boost COMPONENTS ${Boost_COMPONENTS} REQUIRED)
ENDIF(Boost_FOUND AND Boost_MINOR_VERSION GREATER "34")

LIST(APPEND DEP_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
LIST(APPEND DEP_LIBRARIES    ${Boost_LIBRARIES})
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Configure GLUT:
FIND_PACKAGE(GLUT_OpenSG REQUIRED)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
LIST(APPEND DEP_LIBRARIES ${GLUT_LIBRARIES})
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Setup compiler:
INCLUDE_DIRECTORIES(${DEP_INCLUDE_DIRS})

ADD_EXECUTABLE(${TARGET} ${SRCFILES})

TARGET_LINK_LIBRARIES(${TARGET} ${DEP_LIBRARIES})
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Target properties:
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES DEBUG_POSTFIX ${DEBUG_POSTFIX})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES PROJECT_LABEL ${LABEL})

SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS_DEBUG          ${MSVC_IGNORE_LIBRARY_FLAGS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS_RELEASE        ${MSVC_IGNORE_LIBRARY_FLAGS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS_MINSIZEREL     ${MSVC_IGNORE_LIBRARY_FLAGS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS_RELWITHDEBINFO ${MSVC_IGNORE_LIBRARY_FLAGS})
#------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Setup library installation:
INSTALL(TARGETS ${TARGET}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Set Visual Studio compiler warning level:
IF(MSVC)
  MSVC_WARNING_LEVEL(1)
ENDIF()
#-------------------------------------------------------------------------------
