#----------------------------------------------------------------
# Generated CMake target import file for configuration "".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "thread" for configuration ""
set_property(TARGET thread APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(thread PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "/usr/local/lib/libthread.so"
  IMPORTED_SONAME_NOCONFIG "libthread.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS thread )
list(APPEND _IMPORT_CHECK_FILES_FOR_thread "/usr/local/lib/libthread.so" )

# Import target "test1" for configuration ""
set_property(TARGET test1 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(test1 PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "/usr/local/bin/bin/test1"
  )

list(APPEND _IMPORT_CHECK_TARGETS test1 )
list(APPEND _IMPORT_CHECK_FILES_FOR_test1 "/usr/local/bin/bin/test1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
