#
# Find lwl library
# 
# Maciej Maliborski 
# Tue Nov  2 11:49:51 2010
#

FIND_PATH(LWL_INCLUDE_DIR lwl.h /usr/include/ /usr/local/include/)

FIND_LIBRARY(LWL_LIBRARY NAMES lwl PATH /usr/lib /usr/local/lib) 

IF (LWL_INCLUDE_DIR AND LWL_LIBRARY)
   SET(LWL_FOUND TRUE)
ENDIF (LWL_INCLUDE_DIR AND LWL_LIBRARY)


IF (LWL_FOUND)
  IF (NOT LWL_FIND_QUIETLY)
      MESSAGE(STATUS "Found lwl: ${LWL_LIBRARY}")
   ENDIF (NOT LWL_FIND_QUIETLY)
ELSE (LWL_FOUND)
   IF (LWL_FIND_REQUIRED)
     MESSAGE(FATAL_ERROR "Could not find lwl")
   ENDIF (LWL_FIND_REQUIRED)
ENDIF (LWL_FOUND)


# SET(LWL_EXE_LINKER_FLAGS "-llwl")