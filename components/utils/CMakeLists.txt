
set(UTILS_COMPONENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")

if(EXISTS ${UTILS_COMPONENT_DIR}/src)
    message(STATUS "Use project component folder ${UTILS_COMPONENT_DIR}.")
endif()

set(requires esp_timer)

idf_component_register(SRCS "src/utils.cpp"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_timer
                       "")