To add axo to noelegacy put whole Axo folder into Minecraft.World folder and make a little patch to Minecraft.World.cpp:
after ```BeaconTileEntity::staticCtor();``` (line 95) put:
```
	// AXO_BRIDGE_MARKER_START
	AxoBridge_BootstrapJVM();
	// AXO_BRIDGE_MARKER_END
```
and compile the game