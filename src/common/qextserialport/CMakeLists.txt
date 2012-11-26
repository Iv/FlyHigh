include(${QT_USE_FILE})

# ${CMAKE_CURRENT_BINARY_DIR} needs to be in the includ path
# because of the mox_*.cxx files are #include'd in source
include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${QT_INCLUDES})

set(qextserialport_HDRS
    qextserialport.h
    qextserialport_p.h
    qextserialenumerator.h
    qextserialenumerator_p.h
    qextserialport_global.h)

set(qextserialport_SRCS
    qextserialport.cpp
    qextserialenumerator.cpp)

IF(WIN32)
  # append windoze specific source files
  set(qextserialport_SRCS
      ${qextserialport_SRCS}
      qextserialport_win.cpp
      qextserialenumerator_win.cpp)

  # For Windows user who doesn't have Qt4's Private files
  IF(${QT_VERSION_MAJOR} MATCHES "4")
    IF(NOT EXISTS "${QT_HEADERS_DIR}/QtCore/private/qwineventnotifier_p.h")
      set(qextserialport_SRCS
          ${qextserialport_SRCS}
          qextwineventnotifier_p.cpp)

      set(qextserialport_HDRS
          ${qextserialport_HDRS}
          qextwineventnotifier_p.h)

      # this header needs to be moc'ed, compiled AND included in
      # the library (see below)
      QT4_WRAP_CPP(qextserialport_WIN_MOC_SRCS qextwineventnotifier_p.h)

      add_definitions(-DQESP_NO_QT4_PRIVATE)
     ENDIF()
  ENDIF()

  # add windoze libraries
  find_library(SETUPAPI_LIB setupapi)
  find_library(ADVAPI32_LIB advapi32)
  find_library(USER32_LIB user32)
  # somehow, cmake refuses to add "-lsetupapi" to the link
  # command, so we'll do a workaround here
  set(LIBS_FOR_QESP
      ${LIBS_FOR_QESP}
      ${SETUPAPI_LIB}
      ${ADVAPI32_LIB}
      ${USER32_LIB}
      setupapi)

ELSEIF(UNIX)
  # unix-like systems
  set(qextserialport_SRCS
      ${qextserialport_SRCS}
      qextserialport_unix.cpp)
  # add os depending enumerator sources
  IF(${CMAKE_SYSTEM} MATCHES "Linux")
    # linux
    set(qextserialport_SRCS
        ${qextserialport_SRCS}
        qextserialenumerator_linux.cpp)

    # looking for libudev
    # there's no official cmake udev module yet, we're providing our own
    # ask cmake to look for FindUDev.cmake in the current directory:
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
    find_package(UDev REQUIRED)

    # UDEV_LIBS is now defined - add to list
    set(LIBS_FOR_QESP
        ${LIBS_FOR_QESP}
        ${UDEV_LIBS})

  ELSEIF(APPLE)
    # osx
    set(qextserialport_SRCS
        ${qextserialport_SRCS}
        qextserialenumerator_osx.cpp)

    # todo: adding apple libraries
  ELSE()
    # other unixes
    set(qextserialport_SRCS
        ${qextserialport_SRCS}
        qextserialenumerator_unix.cpp)
  ENDIF()
ENDIF()

# This is qesp specific: moc needs to be called on qextserialport.h, but
# the resulting moc_qextserialport.cxx is NOT compiled.
# Instead, it is included in qextserialport.cpp. The same applies
# to qextserialenumerator.h.
# ${CMAKE_CURRENT_BINARY_DIR} needs to be added to the include path
# in order to make the generated moc files available to the compiler.
set(qextserialport_MOC_HDRS
    qextserialport.h
    qextserialenumerator.h)
QT4_WRAP_CPP(qextserialport_MOC_SRCS ${qextserialport_MOC_HDRS})
# prevent compilation of the moc generated files. It would fail anyway.
SET_SOURCE_FILES_PROPERTIES(${qextserialport_MOC_SRCS}
                            PROPERTIES
                            HEADER_FILE_ONLY true)


add_library(qextserialport ${qextserialport_SRCS} ${qextserialport_HDRS} ${qextserialport_MOC_SRCS} ${qextserialport_WIN_MOC_SRCS})

# add dependent libraries
target_link_libraries(qextserialport ${LIBS_FOR_QESP})
