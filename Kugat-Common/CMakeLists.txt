set(SRC_KUGAT_COMMON
	Common.cpp
	Common.hpp
	Endpoint.cpp
	Endpoint.hpp
	GameData.cpp
	GameData.hpp
)
source_group("" FILES ${SRC_KUGAT_COMMON})

add_library(KugatCommon STATIC ${SRC_KUGAT_COMMON})
target_include_directories(KugatCommon PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(KugatCommon PUBLIC Enlivengine)