set(AXO_WORLD_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/AxoBridge.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AxoBridgeJNI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AxoJavaTile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AxoJavaItem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AxoJavaCrop.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AxoTileItem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AxoJavaBiome.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AxoBridgeWorldGen.cpp
)

target_sources(Minecraft.World PRIVATE ${AXO_WORLD_SOURCES})
target_include_directories(Minecraft.World PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(Minecraft.World PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libs/jvm.lib
    kernel32
    user32
)
