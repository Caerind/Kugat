set(SRC_KUGAT_SERVER
	main.cpp
	Server.cpp
	Server.hpp
	ServerSocket.hpp
)
source_group("" FILES ${SRC_KUGAT_SERVER})

add_executable(KugatServer ${SRC_KUGAT_SERVER})
target_link_libraries(KugatServer PUBLIC KugatCommon)