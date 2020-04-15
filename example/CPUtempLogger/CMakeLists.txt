cmake_minimum_required(VERSION 2.6)
project(CPUtempLogger)

add_executable(cputemplogger main.cpp)

target_link_libraries(cputemplogger ${catkin_LIBRARIES} libdaemoncpp.so)

# Directory with systemd unit files
set (SYSTEMD_UNIT_DIR "/usr/lib/systemd/system/")

# Macro for installing configuration files
function(install_conf src dest)
  if(NOT IS_ABSOLUTE "${src}")
    set(src "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
  endif()
  get_filename_component(src_name "${src}" NAME)
  if (NOT IS_ABSOLUTE "${dest}")
    set(dest "${CMAKE_INSTALL_PREFIX}/${dest}")
  endif()
  install(CODE "
    if(NOT EXISTS \"\$ENV{DESTDIR}${dest}/${src_name}\")
      #file(INSTALL \"${src}\" DESTINATION \"${dest}\")
      message(STATUS \"Installing: \$ENV{DESTDIR}${dest}/${src_name}\")
      execute_process(COMMAND \${CMAKE_COMMAND} -E copy \"${src}\"
                      \"\$ENV{DESTDIR}${dest}/${src_name}\"
                      RESULT_VARIABLE copy_result
                      ERROR_VARIABLE error_output)
      if(copy_result)
        message(FATAL_ERROR \${error_output})
      endif()
    else()
      message(STATUS \"Skipping  : \$ENV{DESTDIR}${dest}/${src_name}\")
    endif()
  ")
endfunction(install_conf)

# Install systemd unit files 
install_conf (./cputemplogger.service ${SYSTEMD_UNIT_DIR})

install (TARGETS cputemplogger RUNTIME DESTINATION bin)
