CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(thread C)

set(THREAD_MAJOR_VERSION 0)
set(THREAD_MINOR_VERTION 1)
set(THREAD_PATCH_VERSION 0)
set(THREAD_VERSION
    "${THREAD_MAJOR_VERSION}.${THREAD_MINOR_VERTION}.${THREAD_PATCH_VERSION}")

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libs")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for bins")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for headers")

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/Thread)
endif()

set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")


# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}"   # to find libthread/thread.h
  "${PROJECT_SOURCE_DIR}/include" # to find list.h
  "${PROJECT_BINARY_DIR}")  # to find libthread/config.h

ADD_SUBDIRECTORY(libthread/)
ADD_SUBDIRECTORY(tests/)

export(TARGETS thread
    FILE "${PROJECT_BINARY_DIR}/ThreadTargets.cmake")

export(PACKAGE thread)

# Create the ThreadConfig.cmake and ThreadConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(ThreadConfig.cmake.in
  "${PROJECT_BINARY_DIR}/ThreadConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${THREAD_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(ThreadConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ThreadConfig.cmake" @ONLY)
# ... for both
configure_file(ThreadConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/ThreadConfigVersion.cmake" @ONLY)
 
# Install the ThreadConfig.cmake and ThreadConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ThreadConfig.cmake"
  "${PROJECT_BINARY_DIR}/ThreadConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Install the export set for use with the install-tree
install(EXPORT ThreadTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
