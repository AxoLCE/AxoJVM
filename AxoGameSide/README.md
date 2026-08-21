To add axo to noelegacy put whole Axo folder into Minecraft.World folder and make a little patch to Minecraft.World.cpp:
after ```BeaconTileEntity::staticCtor();``` (line 95) put:
```
	// AXO_BRIDGE_MARKER_START
	AxoBridge_BootstrapJVM();
	// AXO_BRIDGE_MARKER_END
```
and a small patch to CMakeLists.txt:
after
```
add_subdirectory(Minecraft.World)
add_subdirectory(Minecraft.Client)
```
add 
```include(${CMAKE_SOURCE_DIR}/Minecraft.World/Axo/AxoSources_World.cmake)```
and compile the game
