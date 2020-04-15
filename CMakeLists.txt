cmake_minimum_required(VERSION 3.9)

project(daemoncpp VERSION 1.0.1 DESCRIPTION "daemon description")

include(GNUInstallDirs)

add_library(daemoncpp SHARED src/daemoncpp.cpp)

set_target_properties(daemoncpp PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/daemoncpp.h)
    
configure_file(daemoncpp.pc.in daemoncpp.pc @ONLY)
target_include_directories(daemoncpp PRIVATE .)

install(TARGETS daemoncpp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    
install(FILES ${CMAKE_BINARY_DIR}/daemoncpp.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
