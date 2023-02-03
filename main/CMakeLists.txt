set(SOURCES ./ devices/src)
set(PATH_INC_DIRS enumeration/inc devices/inc)

idf_component_register(SRC_DIRS ${SOURCES}
                   INCLUDE_DIRS ${PATH_INC_DIRS}
                   )

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++2a)

