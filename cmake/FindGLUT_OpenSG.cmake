# - try to find glut library and include files
#  GLUT_INCLUDE_DIR, where to find GL/glut.h, etc.
#  GLUT_LIBRARIES, the libraries to link against
#  GLUT_FOUND, If false, do not try to use GLUT.
# Also defined, but not for general use are:
#  GLUT_glut_LIBRARY = the full path to the glut library.
#  GLUT_Xmu_LIBRARY  = the full path to the Xmu library.
#  GLUT_Xi_LIBRARY   = the full path to the Xi Library.


IF (WIN32)

  IF(NOT GLUT_INCLUDE_DIR)
    FIND_PATH( GLUT_INCLUDE_DIR NAMES GL/glut.h 
      PATHS  
      ${GLUT_ROOT_PATH}/include
      ${GLUT_ROOT}/include )
  ENDIF(NOT GLUT_INCLUDE_DIR)

  FIND_LIBRARY( GLUT_glut_LIBRARY_RELEASE NAMES osgglut glut glut32
    PATHS
    ${GLUT_LIBRARY_DIR}
    ${GLUT_ROOT_PATH}/lib
    ${GLUT_ROOT}/lib
    ${GLUT_ROOT}/Release
    )

  FIND_LIBRARY( GLUT_glut_LIBRARY_DEBUG NAMES osgglutD glutd glut32d
    PATHS
    ${GLUT_LIBRARY_DIR}
    ${GLUT_ROOT}/lib
    ${GLUT_ROOT}/Debug
    )
    
    SET(GLUT_LIBRARIES optimized ${GLUT_glut_LIBRARY_RELEASE}
                       debug     ${GLUT_glut_LIBRARY_DEBUG})
                       
    #MESSAGE(STATUS GLUT_glut_LIBRARY_RELEASE: ${GLUT_glut_LIBRARY_RELEASE})
    #MESSAGE(STATUS GLUT_glut_LIBRARY_DEBUG:   ${GLUT_glut_LIBRARY_DEBUG})
ELSE (WIN32)

  MESSAGE(ERROR "Only to be used on WIN32 platforms")
  
  IF (APPLE)
    # These values for Apple could probably do with improvement.
    FIND_PATH( GLUT_INCLUDE_DIR glut.h
      /System/Library/Frameworks/GLUT.framework/Versions/A/Headers
      ${OPENGL_LIBRARY_DIR}
      )
    SET(GLUT_glut_LIBRARY "-framework GLUT" CACHE STRING "GLUT library for OSX") 
    SET(GLUT_cocoa_LIBRARY "-framework Cocoa" CACHE STRING "Cocoa framework for OSX")
  ELSE (APPLE)
    
    FIND_PATH( GLUT_INCLUDE_DIR GL/glut.h
      /usr/include/GL
      /usr/openwin/share/include
      /usr/openwin/include
      /opt/graphics/OpenGL/include
      /opt/graphics/OpenGL/contrib/libglut
      )
  
    FIND_LIBRARY( GLUT_glut_LIBRARY glut
      /usr/openwin/lib
      )
    
    FIND_LIBRARY( GLUT_Xi_LIBRARY Xi
      /usr/openwin/lib
      )
    
    FIND_LIBRARY( GLUT_Xmu_LIBRARY Xmu
      /usr/openwin/lib
      )
    
  ENDIF (APPLE)
  
ENDIF (WIN32)

SET( GLUT_FOUND FALSE)
IF(GLUT_INCLUDE_DIR)
  IF(GLUT_glut_LIBRARY_DEBUG OR GLUT_glut_LIBRARY_RELEASE)
    # Is -lXi and -lXmu required on all platforms that have it?
    # If not, we need some way to figure out what platform we are on.
#    SET( GLUT_LIBRARIES
#      ${GLUT_glut_LIBRARY}
#      ${GLUT_Xmu_LIBRARY}
#      ${GLUT_Xi_LIBRARY} 
#      ${GLUT_cocoa_LIBRARY}
#      )
    SET( GLUT_FOUND TRUE )
    
    #The following deprecated settings are for backwards compatibility with CMake1.4
#    SET (GLUT_LIBRARY ${GLUT_LIBRARIES})
    SET (GLUT_INCLUDE_PATH ${GLUT_INCLUDE_DIR})
    
  ENDIF(GLUT_glut_LIBRARY_DEBUG OR GLUT_glut_LIBRARY_RELEASE)
ENDIF(GLUT_INCLUDE_DIR)

MARK_AS_ADVANCED(
  GLUT_INCLUDE_DIR
  GLUT_glut_LIBRARY_RELEASE
  GLUT_glut_LIBRARY_DEBUG
#  GLUT_Xmu_LIBRARY
#  GLUT_Xi_LIBRARY
  )
