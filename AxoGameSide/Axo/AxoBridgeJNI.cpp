#include <Windows.h>
#include <gl/GL.h>
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#include "AxoBridge.h"
#include "AxoJavaTile.h"
#include "AxoJavaCrop.h"
#include "../Tile.h"
#include "../Material.h"
#include <unordered_map>
#include <Windows.h>
#include <string>
#include "include/jni.h"
#include "../TileItem.h"
#include "../Item.h"
#include "AxoTileItem.h"
#include "AxoJavaItem.h"
#include "../../Minecraft.Client/PreStitchedTextureMap.h"
#include "../../Minecraft.Client/SimpleIcon.h"
#include "../../Minecraft.Client/BufferedImage.h"
#include "../../Minecraft.Client/Texture.h"
#include "../../Minecraft.Client/Textures.h"
#include "../../Minecraft.Client/TexturePack.h"
#include "../../Minecraft.Client/Minecraft.h"
#include "../../Minecraft.World/ByteBuffer.h"

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
static jstring WStringToJString(JNIEnv* env, const wstring& wstr) {
    if (wstr.empty()) return env->NewStringUTF("");
    int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(ulen > 0 ? ulen - 1 : 0, 0);
    if (ulen > 0) {
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], ulen, nullptr, nullptr);
    }
    return env->NewStringUTF(str.c_str());
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
        jboolean isSolidRender,
        jint jDropItemId,
        jstring jRenderShape,
        jboolean jCanWalkThrough
    ) {
        InitSoundMap();
        InitMaterialMap();

        std::wstring name = JStringToWString(env, jname);
        std::wstring material = JStringToWString(env, jmaterial);
        std::wstring soundStr = JStringToWString(env, jsoundType);
        std::wstring iconName = JStringToWString(env, jiconName);
        std::wstring renderShapeStr = JStringToWString(env, jRenderShape);

        int renderShapeId = Tile::SHAPE_BLOCK;
        if (renderShapeStr == L"cross") renderShapeId = Tile::SHAPE_CROSS_TEXTURE;
        else if (renderShapeStr == L"rows") renderShapeId = Tile::SHAPE_ROWS;

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
        AxoJavaTile* tile = new AxoJavaTile(id, mat, isSolidRender, destroyTime, explosionResistance, sound, iconName, name, jDropItemId, renderShapeId, jCanWalkThrough);
        
        if (Item::items[id] == nullptr) {
            AxoTileItem* itemBlock = new AxoTileItem(id - 256, name);
            printf("[AxoJVM] Registered itemblock for tile %d\n", id);
        }
        printf("[AxoJVM] Registered tile: %ls (id=%d)\n", name.c_str(), id);
}
void JNICALL Java_axo_jvm_Bridge_registerItem(
    JNIEnv* env, jclass,
    jint id,
    jstring jname,
    jstring jiconName,
    jint maxStackSize
) {
    std::wstring name = JStringToWString(env, jname);
    std::wstring iconName = JStringToWString(env, jiconName);
    if (id < 1000 || id > 31999) {
        printf("[AxoJVM] ERROR: item id %d out of range\n", id);
        return;
    }
    AxoJavaItem* item = new AxoJavaItem(id - 256, iconName, name, maxStackSize, -1);
    printf("[AxoJVM] Registered item: %ls (id=%d)\n", name.c_str(), id);
}

void JNICALL Java_axo_jvm_Bridge_registerCrop(
    JNIEnv* env, jclass,
    jint id,
    jstring jname,
    jobjectArray jStageTextures,
    jint seedItemId,
    jint dropItemId
) {
    std::wstring name = JStringToWString(env, jname);
    jsize count = env->GetArrayLength(jStageTextures);
    std::vector<wstring> stageIconNames;
    for (int i = 0; i < count; i++) {
        jstring jTex = (jstring)env->GetObjectArrayElement(jStageTextures, i);
        stageIconNames.push_back(JStringToWString(env, jTex));
        env->DeleteLocalRef(jTex);
    }

    AxoJavaCrop* crop = new AxoJavaCrop(id, stageIconNames, seedItemId, dropItemId);
    if (Item::items[id] == nullptr) {
        AxoTileItem* itemBlock = new AxoTileItem(id - 256, name);
    }
    printf("[AxoJVM] Registered crop: %ls (id=%d)\n", name.c_str(), id);
}

void JNICALL Java_axo_jvm_Bridge_registerSeed(
    JNIEnv* env, jclass,
    jint id,
    jstring jname,
    jstring jiconName,
    jint maxStackSize,
    jint plantBlockId
) {
    std::wstring name = JStringToWString(env, jname);
    std::wstring iconName = JStringToWString(env, jiconName);
    
    if (id < 1000 || id > 31999) {
        printf("[AxoJVM] ERROR: item id %d out of range\n", id);
        return;
    }
    AxoJavaItem* item = new AxoJavaItem(id - 256, iconName, name, maxStackSize, plantBlockId);
    printf("[AxoJVM] Registered seed: %ls (id=%d, plant=%d)\n", name.c_str(), id, plantBlockId);
}
// Register Icons
void AxoBridge_RegisterCustomIcons(PreStitchedTextureMap* textureMap) {
    JNIEnv* env = Axo_GetJNIEnv();
    if (!env) return;

    jclass registryClass = env->FindClass("axo/jvm/TextureRegistry");
    if (!registryClass) return;

    jmethodID getAllMethod = env->GetStaticMethodID(registryClass, "getAll", "()Ljava/util/Map;");
    jobject map = env->CallStaticObjectMethod(registryClass, getAllMethod);
    if (!map) return;

    jclass mapClass = env->FindClass("java/util/Map");
    jmethodID entrySet = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    jobject set = env->CallObjectMethod(map, entrySet);

    jclass setClass = env->FindClass("java/util/Set");
    jmethodID iterator = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iter = env->CallObjectMethod(set, iterator);

    jclass iterClass = env->FindClass("java/util/Iterator");
    jmethodID hasNext = env->GetMethodID(iterClass, "hasNext", "()Z");
    jmethodID next = env->GetMethodID(iterClass, "next", "()Ljava/lang/Object;");

    jclass entryClass = env->FindClass("java/util/Map$Entry");
    jmethodID getValue = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

    jclass customTexClass = env->FindClass("axo/jvm/TextureRegistry$CustomTexture");
    jfieldID nameField = env->GetFieldID(customTexClass, "name", "Ljava/lang/String;");
    jfieldID u0Field = env->GetFieldID(customTexClass, "u0", "F");
    jfieldID v0Field = env->GetFieldID(customTexClass, "v0", "F");
    jfieldID u1Field = env->GetFieldID(customTexClass, "u1", "F");
    jfieldID v1Field = env->GetFieldID(customTexClass, "v1", "F");

    while (env->CallBooleanMethod(iter, hasNext)) {
        jobject entry = env->CallObjectMethod(iter, next);
        jobject customTex = env->CallObjectMethod(entry, getValue);

        jfieldID atlasTypeField = env->GetFieldID(customTexClass, "atlasType", "I");
        int atlasType = env->GetIntField(customTex, atlasTypeField);

        if (atlasType != textureMap->getIconType()) {
            env->DeleteLocalRef(entry);
            env->DeleteLocalRef(customTex);
            continue;
        }

        jstring jname = (jstring)env->GetObjectField(customTex, nameField);
        const char* nameStr = env->GetStringUTFChars(jname, nullptr);
        std::wstring wname = JStringToWString(env, jname);

        float u0 = env->GetFloatField(customTex, u0Field);
        float v0 = env->GetFloatField(customTex, v0Field);
        float u1 = env->GetFloatField(customTex, u1Field);
        float v1 = env->GetFloatField(customTex, v1Field);
        textureMap->texturesByName[wname] = new SimpleIcon(wname, wname, u0, v0, u1, v1);

        printf("[AxoJVM] Added icon to atlas: %ls (UV: %.3f,%.3f - %.3f,%.3f)\n", wname.c_str(), u0, v0, u1, v1);

        env->ReleaseStringUTFChars(jname, nameStr);
        env->DeleteLocalRef(entry);
        env->DeleteLocalRef(customTex);
        env->DeleteLocalRef(jname);
    }
    env->DeleteLocalRef(registryClass);
    env->DeleteLocalRef(map);
    env->DeleteLocalRef(mapClass);
    env->DeleteLocalRef(set);
    env->DeleteLocalRef(setClass);
    env->DeleteLocalRef(iter);
    env->DeleteLocalRef(iterClass);
    env->DeleteLocalRef(entryClass);
    env->DeleteLocalRef(customTexClass);
}
// Paint custom textures
void AxoBridge_PaintCustomTextures(BufferedImage* atlasImage, int iconType) {
    JNIEnv* env = Axo_GetJNIEnv();
    if (!env || !atlasImage) return;

    jclass registryClass = env->FindClass("axo/jvm/TextureRegistry");
    if (!registryClass || env->ExceptionCheck()) {
        if(env->ExceptionCheck())
           env->ExceptionClear();
        return;
    }

    jmethodID getAllMethod = env->GetStaticMethodID(registryClass, "getAll", "()Ljava/util/Map;");
    jobject map = env->CallStaticObjectMethod(registryClass, getAllMethod);
    if (!map) return;

    jclass mapClass = env->FindClass("java/util/Map");
    jmethodID values = env->GetMethodID(mapClass, "values", "()Ljava/util/Collection;");
    jobject collection = env->CallObjectMethod(map, values);

    jclass collClass = env->FindClass("java/util/Collection");
    jmethodID iterator = env->GetMethodID(collClass, "iterator", "()Ljava/util/Iterator;");
    jobject iter = env->CallObjectMethod(collection, iterator);

    jclass iterClass = env->FindClass("java/util/Iterator");
    jmethodID hasNext = env->GetMethodID(iterClass, "hasNext", "()Z");
    jmethodID next = env->GetMethodID(iterClass, "next", "()Ljava/lang/Object;");

    jclass customTexClass = env->FindClass("axo/jvm/TextureRegistry$CustomTexture");
    jfieldID atlasTypeField = env->GetFieldID(customTexClass, "atlasType", "I");
    jfieldID pngField = env->GetFieldID(customTexClass, "pngBytes", "[B");
    jfieldID rowField = env->GetFieldID(customTexClass, "row", "I");
    jfieldID colField = env->GetFieldID(customTexClass, "col", "I");

    int* atlasPixels = atlasImage->getData();
    int atlasWidth = atlasImage->getWidth();

    while (env->CallBooleanMethod(iter, hasNext)) {
        jobject customTex = env->CallObjectMethod(iter, next);
        
        int atlasType = env->GetIntField(customTex, atlasTypeField);

        if (atlasType != iconType) {
            env->DeleteLocalRef(customTex);
            continue;
        }

        jbyteArray jPng = (jbyteArray)env->GetObjectField(customTex, pngField);
        int row = env->GetIntField(customTex, rowField);
        int col = env->GetIntField(customTex, colField);

        if (jPng != nullptr) {
            jsize size = env->GetArrayLength(jPng);
            PBYTE pngData = new BYTE[size];
            env->GetByteArrayRegion(jPng, 0, size, (jbyte*)pngData);
            BufferedImage* img = new BufferedImage(pngData, size);
            printf("[AxoJVM] PaintCustomTextures: img loaded (%dx%d)\n", img->getWidth(), img->getHeight());
            
            if (img && img->getWidth() == 16 && img->getHeight() == 16) {
                int* customPixels = img->getData();
                int* atlasPixels = atlasImage->getData();
                
                if (customPixels && atlasPixels) {
                    int atlasWidth = atlasImage->getWidth();
                    int atlasHeight = atlasImage->getHeight();
                    
                    int x = col * 16;
                    int y = (int)(env->GetFloatField(customTex, env->GetFieldID(customTexClass, "v0", "F")) * atlasHeight);
                    
                    for (int py = 0; py < 16; py++) {
                        for (int px = 0; px < 16; px++) {
                            int srcIdx = py * 16 + px;
                            int dstIdx = (y + py) * atlasWidth + (x + px);
                            atlasPixels[dstIdx] = customPixels[srcIdx];
                        }
                    }
                }
            }
            if (img) delete img;
            delete[] pngData;
        }

        env->DeleteLocalRef(customTex);
        env->DeleteLocalRef(jPng);
    }

    env->DeleteLocalRef(registryClass);
    env->DeleteLocalRef(map);
    env->DeleteLocalRef(mapClass);
    env->DeleteLocalRef(collection);
    env->DeleteLocalRef(collClass);
    env->DeleteLocalRef(iter);
    env->DeleteLocalRef(iterClass);
    env->DeleteLocalRef(customTexClass);
}