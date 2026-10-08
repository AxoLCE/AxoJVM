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
and in ```Minecraft.Client/Textures.cpp``` replace:
```
	terrain = new PreStitchedTextureMap(Icon::TYPE_TERRAIN, L"terrain", L"textures/blocks/", missingNo, true);
	items = new PreStitchedTextureMap(Icon::TYPE_ITEM, L"items", L"textures/items/", missingNo, true);
```
to:
```
	terrain = new PreStitchedTextureMap(Icon::TYPE_TERRAIN, L"terrain", L"textures/blocks/", missingNo, false); // Axo_changed
	items = new PreStitchedTextureMap(Icon::TYPE_ITEM, L"items", L"textures/items/", missingNo, false); // Axo_changed
```
and in ```Minecraft.World/RandomLevelSource.cpp``` in ```getChunk``` after ```buildSurfaces(xOffs, zOffs, blocks, blockData, biomes);```:
```
	// AXO_BRIDGE_MARKER_START
	AxoBridge_RunSurfaceGen(level, blocks, xOffs, zOffs);
	// AXO_BRIDGE_MARKER_END
```
and in the same file in ```postProcess``` after ```biome->decorate(level, pprandom, xo, zo);``` add:
```
    // AXO_BRIDGE_MARKER_START
    AxoBridge_RunDecorateGen(level, pprandom, xt, zt);
    // AXO_BRIDGE_MARKER_END
```
and at the top of the file add ```#include "Axo/AxoBridge.h"```
In ```Minecraft.World/BiomeInitLayer.cpp``` after:
```
if (levelType != LevelType::lvl_normal_1_1 && levelType == LevelType::lvl_customized)
{
    this->customSettings = (CustomizableSourceSettings*)superflatConfig; 
}
```
add:
```
    // AXO_BRIDGE_MARKER_START
    AxoBridge_ApplyBiomeSpawns(desertBiomes, warmBiomes, coolBiomes, icyBiomes);
    // AXO_BRIDGE_MARKER_END
```
and add ```#include "Axo/AxoBridge.h"```
in: ```Minecraft.World/HellRandomLevelSource.cpp``` after:
```
netherBridgeFeature->apply(this, level, xOffs, zOffs, blocks);
```
add:
```
// AXO_MARKER_START
AxoBridge_RunSurfaceGen(level, blocks, xOffs, zOffs);
// AXO_MARKER_END
```
and after:
```
LevelChunk *levelChunk = new LevelChunk(level, blocks, xOffs, zOffs);
levelChunk->setCheckAllLight();
```
add:
```
// AXO_MARKER_START
AxoBridge_ApplyDimensionBiomes(levelChunk, level, xOffs, zOffs);
// AXO_MARKER_END
```
after:
```
for (int i = 0; i < 16; i++)
{
	int x = xo + random->nextInt(16);
	int y = random->nextInt(Level::genDepth - 20) + 10;
	int z = zo + random->nextInt(16);
	HellSpringFeature hellSpringFeature(Tile::flowing_lava_Id, true);
	hellSpringFeature.place(level, random, x, y, z);
}
```
add
```
// AXO_MARKER_START
AxoBridge_RunDecorateGen(level, pprandom, xt, zt);
// AXO_MARKER_END
```
in ```Minecraft.World/TheEndLevelRandomLevelSource.cpp``` after:
```
prepareHeights(xOffs, zOffs, blocks, biomes);
buildSurfaces(xOffs, zOffs, blocks, biomes);
```
add:
```
// AXO_MARKER_START
AxoBridge_RunSurfaceGen(level, blocks, xOffs, zOffs);
// AXO_MARKER_END
```
after:
```
LevelChunk *levelChunk = new LevelChunk(level, blocks, xOffs, zOffs);
```
add:
```
// AXO_MARKER_START
AxoBridge_ApplyDimensionBiomes(levelChunk, level, xOffs, zOffs);
// AXO_MARKER_END
```
after:
```
Biome *biome = level->getBiome(xo + 16, zo + 16);
biome->decorate(level, pprandom, xo, zo);		// 4J - passing pprandom rather than level->random here to make this consistent with our parallel world generation
```
add:
```
// AXO_MARKER_START
AxoBridge_RunDecorateGen(level, pprandom, xt, zt);
// AXO_MARKER_END
```

FOR MODS TAB!!
Copy the files from path of Miencraft.Client\Common\UI to the same path in game files
and in Minecraft.Client\cmake\sources\Windows.cmake after:
```
  "${CMAKE_CURRENT_SOURCE_DIR}/Common/UI/UIControl_MultiList.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Common/UI/UIControl_MultiList.h"
```
add:
```
  # AXO_ADDED
  "${CMAKE_CURRENT_SOURCE_DIR}/Common/UI/UIScene_ModsMenu.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Common/UI/UIScene_ModsMenu.h"
  # AXO_ADDED
```
In UIScene_MainMenu.cpp after:
```
m_buttons[static_cast<int>(eControl_Achievements)].init( (UIString)IDS_ACHIEVEMENTS,eControl_Achievements);
```
add:
```
	// AXO_ADDED
	m_buttons[static_cast<int>(eControl_Mods)].init(L"Mods",eControl_Mods);
	// AXO_ADDED
```
in the same file after:
```
	case eControl_Achievements:
		//CD - Added for audio
		ui.PlayUISFX(eSFX_Press);

		m_eAction=eAction_RunAchievements;
		signInReturnedFunc = &UIScene_MainMenu::Achievements_SignInReturned;
		break;
```
add:
```
	// AXO_ADDED
	case eControl_Mods:
		ui.PlayUISFX(eSFX_Press);
		ui.NavigateToScene(primaryPad, eUIScene_ModsMenu);
		return;
	// AXO_ADDED
```
In UILayer.cpp after:
```
	case eUIScene_AchievementsMenu:
		newScene = new UIScene_AchievementsMenu(iPad, initData, this);
		break;
```
add:
```
	// AXO_ADDED
	case eUIScene_ModsMenu:
		newScene = new UIScene_ModsMenu(iPad, initData, this);
		break;
	// AXO_ADDED
```
and add include: ```#include "UIScene_ModsMenu.h"```
In UIEnums.h after:
```
	eUIScene_BookMenu,
	eUIScene_AchievementsMenu,
```
add:
```
	// AXO_ADDED
	eUIScene_ModsMenu,
	// AXO_ADDED
```
In UIScene_MainMenu.h after:
```
	enum EControls
	{
		eControl_PlayGame,
		eControl_Leaderboards,
		eControl_Achievements,
```
add:
```
		// AXO_CHANGED
		eControl_Mods,
		// AXO_CHANGED
```
And fragment that starts with:
```
	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT( m_buttons[(int)eControl_PlayGame], "Button1")
		UI_MAP_ELEMENT( m_buttons[(int)eControl_Leaderboards], "Button2")
		UI_MAP_ELEMENT( m_buttons[(int)eControl_Achievements], "Button3")
```
replace it with:
```
	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT( m_buttons[(int)eControl_PlayGame], "Button1")
		UI_MAP_ELEMENT( m_buttons[(int)eControl_Leaderboards], "Button2")
		UI_MAP_ELEMENT( m_buttons[(int)eControl_Achievements], "Button3")
		// AXO_CHANGED
		UI_MAP_ELEMENT(m_buttons[(int)eControl_Mods], "Button4")
		// AXO_CHANGED
		UI_MAP_ELEMENT( m_buttons[(int)eControl_HelpAndOptions], "Button5")
		UI_MAP_ELEMENT( m_buttons[(int)eControl_UnlockOrDLC], "Button9")
#ifndef _DURANGO
		UI_MAP_ELEMENT( m_buttons[(int)eControl_MiniGames], "Button7")
		UI_MAP_ELEMENT( m_buttons[(int)eControl_Exit], "Button6")
```

and compile the game