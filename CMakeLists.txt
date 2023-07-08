cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(NPNewLibrary 
  VERSION 0.1
  LANGUAGES CXX
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(NPLibrary SHARED dummy.cpp)
if(WIN32)
  target_link_libraries(NPLibrary PRIVATE ${CMAKE_SOURCE_DIR}/lib/NPLibrary.lib)
else()
  add_library(NPLibraryBackend SHARED IMPORTED)
  set_target_properties(NPLibraryBackend PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libNPLibrary.so)
  target_link_libraries(NPLibrary PRIVATE NPLibraryBackend)
endif()

