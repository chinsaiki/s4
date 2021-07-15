# add_subdirectory(json)

# include_directories(SYSTEM 
#   json/single_include
# )
# Module folder
# set_target_properties(json PROPERTIES FOLDER thirdparty/json)
set_property(GLOBAL APPEND PROPERTY GLOBAL_INCLUDE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib")