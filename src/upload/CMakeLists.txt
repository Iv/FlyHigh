include_directories( ${CMAKE_SOURCE_DIR}/src/common/form
                     ${CMAKE_SOURCE_DIR}/src/common/qjson4)

set(upload_SRCS Account.cpp
                AccountWindow.cpp
                IOLCUploader.cpp
                OLCUploaderFactory.cpp
                XContestUploader.cpp
                XContestAdditionalInfoDlg.cpp
                XContestAdditionalInfoWidgetFactory.cpp)

add_library(upload ${upload_SRCS})
