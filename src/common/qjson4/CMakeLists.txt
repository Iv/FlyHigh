include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} )

set(qjson4_SRCS QJsonArray.cpp
                QJsonDocument.cpp
                QJsonObject.cpp
                QJsonParseError.cpp
                QJsonValue.cpp
                QJsonValueRef.cpp
                QJsonParser.cpp )

add_library(qjson4 ${qjson4_SRCS})
