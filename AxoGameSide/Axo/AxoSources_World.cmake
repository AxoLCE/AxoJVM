set(AXO_WORLD_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/AxoBridge.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/jni.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jni_md.h
)
target_sources(Minecraft.World PRIVATE ${AXO_WORLD_SOURCES})
target_include_directories(Minecraft.World PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(Minecraft.World PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libs/jvm.lib)