
set(cJSON_COMPONENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cJSON")
if(EXISTS ${cJSON_COMPONENT_DIR})
    #list(APPEND EXTRA_COMPONENT_DIRS ${cJSON_COMPONENT_DIR})
    #set(cJSON_COMPONENT_DIR "$ENV{IDF_PATH}/components/cJSON")
    message(STATUS "Use project component folder ${cJSON_COMPONENT_DIR}.")
endif()

set(include_dirs ${cJSON_COMPONENT_DIR})

idf_component_register(SRCS "cJSON/cJSON.c"
                            "cJSON/cJSON_Utils.c"
                            INCLUDE_DIRS "${include_dirs}"
                    INCLUDE_DIRS cJSON)
