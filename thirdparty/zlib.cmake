add_subdirectory(zlib)

include_directories(
  zlib
)
# Module folder
set_target_properties(zlib PROPERTIES FOLDER thirdparty/zlib)

set_property(GLOBAL APPEND PROPERTY GLOBAL_LIBS_LIST zlibstatic)
set_property(GLOBAL APPEND PROPERTY GLOBAL_INCLUDE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/zlib)
