# Compiler features

# Choose C++ standard
set(CMAKE_CXX_STANDARD 17)

message( STATUS "CMAKE_CXX_STANDARD = " ${CMAKE_CXX_STANDARD} )

if(MSVC)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif()
