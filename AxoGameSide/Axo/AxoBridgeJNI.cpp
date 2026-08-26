#include "AxoBridge.h"
#include "AxoJavaTile.h"
#include "../Tile.h"
#include "../Material.h"
#include <unordered_map>
#include <Windows.h>
#include <string>
#include "include/jni.h"
#include "../TileItem.h"
#include "../Item.h"
#include "AxoTileItem.h"

static std::unordered_map<std::wstring, Tile::SoundType*> g_soundMap;
static bool g_soundMapInit = false;
// Sound map
static void InitSoundMap() {
	if (g_soundMapInit) return;
	g_soundMap[L"NORMAL"] = Tile::SOUND_NORMAL;
	g_soundMap[L"WOOD"] = Tile::SOUND_WOOD;
	g_soundMap[L"GRAVEL"] = Tile::SOUND_GRAVEL;
	g_soundMap[L"GRASS"] = Tile::SOUND_GRASS;
	g_soundMap[L"STONE"] = Tile::SOUND_STONE;
	g_soundMap[L"METAL"] = Tile::SOUND_METAL;
    g_soundMap[L"GLASS"] = Tile::SOUND_GLASS;
    g_soundMap[L"CLOTH"] = Tile::SOUND_CLOTH;
    g_soundMap[L"SAND"] = Tile::SOUND_SAND;
    g_soundMap[L"SNOW"] = Tile::SOUND_SNOW;
    g_soundMap[L"LADDER"] = Tile::SOUND_LADDER;
    g_soundMap[L"ANVIL"] = Tile::SOUND_ANVIL;
	g_soundMapInit = true;
}
static std::unordered_map<std::wstring, Material*> g_materialMap;
static bool g_materialMapInit = false;

// Material Map
static void InitMaterialMap() {
    if (g_materialMapInit) return;
    g_materialMap[L"stone"] = Material::stone;
    g_materialMap[L"wood"] = Material::wood;
    g_materialMap[L"cloth"] = Material::cloth;
    g_materialMap[L"glass"] = Material::glass;
    g_materialMap[L"sand"] = Material::sand;
    g_materialMap[L"water"] = Material::water;
    g_materialMap[L"lava"] = Material::lava;
    g_materialMap[L"air"] = Material::air;
    g_materialMap[L"leaves"] = Material::leaves;
    g_materialMapInit = true;
}

static std::wstring JStringToWString(JNIEnv* env, jstring jstr) {
    if (!jstr) return L"";
    const char* utf8 = env->GetStringUTFChars(jstr, nullptr);
    if (!utf8) return L"";
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
    std::wstring wstr(wlen > 0 ? wlen - 1 : 0, 0);
    if (wlen > 0) {
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &wstr[0], wlen);
    }
    env->ReleaseStringUTFChars(jstr, utf8);
    return wstr;
}
// Define registerTile
void JNICALL Java_axo_jvm_Bridge_registerTile(
        JNIEnv* env, jclass,
        jint id,
        jstring jname,
        jstring jmaterial,
        jfloat destroyTime,
        jfloat explosionResistance,
        jstring jsoundType,
        jstring jiconName,
        jboolean isSolidRender
    ) {
        InitSoundMap();
        InitMaterialMap();

        std::wstring name = JStringToWString(env, jname);
        std::wstring material = JStringToWString(env, jmaterial);
        std::wstring soundStr = JStringToWString(env, jsoundType);
        std::wstring iconName = JStringToWString(env, jiconName);

        std::wstring soundUpper = soundStr;
        for (auto& c : soundUpper) c = towupper(c);
        auto soundIt = g_soundMap.find(soundUpper);
        Tile::SoundType* sound = (soundIt != g_soundMap.end())
            ? soundIt->second
            : Tile::SOUND_NORMAL;

        auto matIt = g_materialMap.find(material);
        Material* mat = (matIt != g_materialMap.end())
            ? matIt->second
            : Material::stone;
        if (id < 217 || id > 254) {
            printf("[AxoJVM] ERROR: tile id %d out of range (217-254)\n", id);
            return;
        }
        if (Tile::tiles[id] != nullptr) {
            printf("[AxoJVM] ERROR: tile slot %d already occupied\n", id);
            return;
        }
        AxoJavaTile* tile = new AxoJavaTile(id, mat, isSolidRender, destroyTime, explosionResistance, sound, iconName, name);
        
        if (Item::items[id] == nullptr) {
            AxoTileItem* itemBlock = new AxoTileItem(id - 256, name);
            printf("[AxoJVM] Registered itemblock for tile %d\n", id);
        }
        printf("[AxoJVM] Registered tile: %ls (id=%d)\n", name.c_str(), id);
}