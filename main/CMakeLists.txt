idf_component_register(SRCS "quirby-firmware.cpp"
                   INCLUDE_DIRS "."
                   PRIV_INCLUDE_DIRS "enumeration")

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++2a)
