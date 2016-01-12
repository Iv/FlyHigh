include_directories(${CMAKE_SOURCE_DIR}/src/database/sql)

set(window_SRCS MDIWindow.cpp
               PhotoView.cpp
               ProgressDlg.cpp
               TableWindow.cpp)

add_library(window ${window_SRCS})
