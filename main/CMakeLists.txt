FILE(GLOB_RECURSE app_sources *.cpp *.c)
set(COMPONENT_SRCS "${app_sources} main.cpp")
set(COMPONENT_ADD_INCLUDEDIRS "." "../components/")

register_component()
