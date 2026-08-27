To add axo to noelegacy put whole Axo folder into Minecraft.World folder and make a little patch to Minecraft.World.cpp:
add ```#include "Axo/AxoBridge.h"```,
and after ```BeaconTileEntity::staticCtor();``` (line 95) put:
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
and for shutdown methods:
in ```Minecraft.Client/Windows64/Windows64_Minecraft.cpp``` before comment ```// Free resources, unregister custom classes, and exit.```
add:
```
	// AXO_BRIDGE_MARKER_START
	AxoBridge_ShutdownJVM();
	// AXO_BRIDGE_MARKER_END
```
and for server in ```Minecraft.Server/Windows64/ServerMain.cpp``` after:
```
g_NetworkManager.Terminate();
LogDebugf("shutdown", "Network manager terminated.");
```
add:
```
	// AXO_BRIDGE_MARKER_START
	AxoBridge_ShutdownJVM();
	// AXO_BRIDGE_MARKER_END
```
in ```Minecraft.Client/PreStitchedTextureMap.cpp``` after ```loadUVs();``` (line ~ 45) add:
```
	// AXOBRIDGE_BEGIN: register_custom_icons
	AxoBridge_RegisterCustomIcons(this);
	// AXOBRIDGE_END: register_custom_icons
```
in the same file after ```BufferedImage *image = texturePack->getImageResource(filename, false, true, drive);``` (line ~ 124) add:
```
	// AXOBRIDGE_BEGIN: paint_custom_textures_on_image
	if (image) {
		AxoBridge_PaintCustomTextures(image, iconType);
	}
	// AXOBRIDGE_END: paint_custom_textures_on_image
```
and in the same file add include ```#include "../Minecraft.World/Axo/AxoBridge.h"```

In ```Minecraft.Client/PreStitchedTextureMap.h``` after:
```
class Icon;
class StitchedTexture;
class Texture;
class BufferedImage;
```
add:
``` class TexturePack; // AXO_ADDED ```
and after this:
```
class PreStitchedTextureMap : public IconRegister
{
```
add:
```
	// AXOMARKER_BEGIN
	friend void AxoBridge_RegisterCustomIcons(PreStitchedTextureMap*);
    friend void AxoBridge_BlitCustomTextures(PreStitchedTextureMap*);
	// AXOMARKER_END
```