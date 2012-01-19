include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/database/sql
                    ${QT_INCLUDES} )


set(window_SRCS MDIWindow.cpp
               MigrationDlg.cpp
               ProgressDlg.cpp
               TableWindow.cpp)

set(window_HDRS MDIWindow.h
              MigrationDlg.h
              ProgressDlg.h
              TableWindow.h)

QT4_WRAP_CPP(window_SRCS ${window_HDRS})

add_library(window ${window_SRCS} ${window_HDRS})
