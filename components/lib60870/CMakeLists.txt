
message(STATUS "============== DEBUG: Using libmodbus component folder ==============")

set (src
"file-service/file_server.c"
"iec60870/apl/cpXXtime2a.c"
"iec60870/cs101/cs101_asdu.c"
"iec60870/cs101/cs101_bcr.c"
"iec60870/cs101/cs101_information_objects.c"
"iec60870/cs101/cs101_master_connection.c"
"iec60870/cs101/cs101_master.c"
"iec60870/cs101/cs101_queue.c"
"iec60870/cs101/cs101_slave.c"
"iec60870/cs104/cs104_connection.c"
"iec60870/cs104/cs104_frame.c"
"iec60870/cs104/cs104_slave.c"
"iec60870/link_layer/buffer_frame.c"
"iec60870/link_layer/link_layer.c"
"iec60870/link_layer/serial_transceiver_ft_1_2.c"
"iec60870/frame.c"
"iec60870/lib60870_common.c"

"hal/memory/lib_memory.c"
"hal/thread/esp32/thread_esp32.c"
"hal/socket/esp32/socket_esp32.c"
"hal/time/esp32/time.c"
"common/linked_list.c"
)

set(include "file-service/"
	"config/"
	"common/inc/"
	"inc/api/"
	"inc/internal/"
	"hal/inc/")

idf_component_register( SRCS ${src}
                        INCLUDE_DIRS ${include}
                        PRIV_REQUIRES driver vfs)


message(STATUS "============== END OF DEBUG ==============")