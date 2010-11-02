#
# Find HDF5 library
# 
# Maciej Maliborski 
# Tue Nov  2 12:33:21 2010
#

FIND_PATH(HDF5_INCLUDE_DIR hdf5.h /usr/include/ /usr/local/include/)

FIND_LIBRARY(HDF5_LIBRARY NAMES hdf5 PATH /usr/lib /usr/local/lib) 

IF (HDF5_INCLUDE_DIR AND HDF5_LIBRARY)
   SET(HDF5_FOUND TRUE)
ENDIF (HDF5_INCLUDE_DIR AND HDF5_LIBRARY)


IF (HDF5_FOUND)
  IF (NOT HDF5_FIND_QUIETLY)
      MESSAGE(STATUS "Found HDF5: ${HDF5_LIBRARY}")
   ENDIF (NOT HDF5_FIND_QUIETLY)
ELSE (HDF5_FOUND)
   IF (HDF5_FIND_REQUIRED)
     MESSAGE(FATAL_ERROR "Could not find HDF5")
   ENDIF (HDF5_FIND_REQUIRED)
ENDIF (HDF5_FOUND)


# SET(HDF5_EXE_LINKER_FLAGS "-lhdf5")