set(SRC_KUGAT_CLIENT
	ClientSocket.hpp
	GameSingleton.cpp
	GameSingleton.hpp
	GameState.cpp
	GameState.hpp
	main.cpp
)
source_group("" FILES ${SRC_KUGAT_CLIENT})

add_executable(KugatClient ${SRC_KUGAT_CLIENT})
target_link_libraries(KugatClient PUBLIC KugatCommon)