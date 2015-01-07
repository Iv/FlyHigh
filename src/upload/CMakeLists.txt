include(${QT_USE_FILE})

include_directories(${QT_INCLUDES} )

set(upload_SRCS Account.cpp
                IOLCUploader.cpp
                OLCUploaderFactory.cpp
                XContestUploader.cpp )

add_library(upload ${upload_SRCS})
