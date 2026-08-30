#include <windows.h>
#include <vector>
#include "include/jni.h"
#include "AxoBridge.h"
#include "../Level.h"
#include "../Random.h"
#include "../Biome.h"
#include "../Tile.h"
#include "../BiomeInitLayer.h"

struct AxoBiomeSpawn {
    Biome* biome;
    int weight;
};

static std::vector<AxoBiomeSpawn> g_axoBiomeSpawns[4];

static thread_local byte* t_surfaceBlocks = nullptr;
static thread_local int t_surfaceOriginX = 0;
static thread_local int t_surfaceOriginZ = 0;

static jclass g_worldGenRegistryClass = nullptr;
static jmethodID g_runSurfaceMethod = nullptr;
static jmethodID g_runDecorateMethod = nullptr;

JNIEnv* Axo_WorldGenEnv() {
    if (!Axo_GetJNIEnv()) {
        JavaVM* jvm = Axo_GetJavaVM();
        if (!jvm) return nullptr;
        JNIEnv* env = nullptr;
        JavaVMAttachArgs args = {};
        args.version = JNI_VERSION_21;
        if (jvm->AttachCurrentThreadAsDaemon((void**)&env, &args) != JNI_OK) return nullptr;
        return env;
    }
    return Axo_GetJNIEnv();
}

static void AxoWorldGen_CacheClasses(JNIEnv* env) {
    if (g_worldGenRegistryClass) return;
    jclass local = env->FindClass("axo/jvm/WorldGenRegistry");
    if (!local) {
        if (env->ExceptionCheck()) env->ExceptionClear();
        return;
    }
    g_worldGenRegistryClass = (jclass)env->NewGlobalRef(local);
    env->DeleteLocalRef(local);
    g_runSurfaceMethod = env->GetStaticMethodID(g_worldGenRegistryClass, "runSurface", "(JIIJI)V");
    g_runDecorateMethod = env->GetStaticMethodID(g_worldGenRegistryClass, "runDecorate", "(JIIJI)V");
    if (env->ExceptionCheck()) env->ExceptionClear();
}

void AxoBridge_RunSurfaceGen(Level* level, byteArray blocks, int xOffs, int zOffs) {
    JNIEnv* env = Axo_WorldGenEnv();
    if (!env || !g_axoBiomeSpawns && false) return;
    if (!env) return;
    AxoWorldGen_CacheClasses(env);
    if (!g_worldGenRegistryClass || !g_runSurfaceMethod) return;

    t_surfaceBlocks = blocks.data;
    t_surfaceOriginX = xOffs * 16;
    t_surfaceOriginZ = zOffs * 16;

    Biome* biome = level->getBiome(t_surfaceOriginX + 8, t_surfaceOriginZ + 8);
    int biomeId = biome ? biome->id : -1;

    env->CallStaticVoidMethod(g_worldGenRegistryClass, g_runSurfaceMethod,
        (jlong)(intptr_t)level, (jint)t_surfaceOriginX, (jint)t_surfaceOriginZ,
        (jlong)(intptr_t)level->getBiomeSource(), (jint)biomeId);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }

    t_surfaceBlocks = nullptr;
}

void AxoBridge_RunDecorateGen(Level* level, Random* random, int xt, int zt) {
    JNIEnv* env = Axo_WorldGenEnv();
    if (!env) return;
    AxoWorldGen_CacheClasses(env);
    if (!g_worldGenRegistryClass || !g_runDecorateMethod) return;

    int xo = xt * 16;
    int zo = zt * 16;
    Biome* biome = level->getBiome(xo + 16, zo + 16);
    int biomeId = biome ? biome->id : -1;

    env->CallStaticVoidMethod(g_worldGenRegistryClass, g_runDecorateMethod,
        (jlong)(intptr_t)level, (jint)xo, (jint)zo, (jlong)(intptr_t)random, (jint)biomeId);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

static int Axo_GetSurfaceBlock(int x, int y, int z) {
    int lx = x - t_surfaceOriginX;
    int lz = z - t_surfaceOriginZ;
    if (lx < 0 || lx > 15 || lz < 0 || lz > 15 || y < 0 || y >= Level::genDepth) return 0;
    return t_surfaceBlocks[(lx * 16 + lz) * Level::genDepth + y];
}

static void Axo_SetSurfaceBlock(int x, int y, int z, int tile, int data) {
    int lx = x - t_surfaceOriginX;
    int lz = z - t_surfaceOriginZ;
    if (lx < 0 || lx > 15 || lz < 0 || lz > 15 || y < 0 || y >= Level::genDepth) return;
    t_surfaceBlocks[(lx * 16 + lz) * Level::genDepth + y] = (byte)(tile & 0xFF);
}
jint JNICALL Java_axo_jvm_Bridge_randomInt(JNIEnv*, jclass, jlong randomPtr, jint bound) {
    return ((Random*)randomPtr)->nextInt(bound);
}

jint JNICALL Java_axo_jvm_Bridge_randomIntImpl(JNIEnv*, jclass, jlong randomPtr, jint bound);

jint Axo_RandomInt(jlong randomPtr, jint bound) {
    return ((Random*)randomPtr)->nextInt(bound);
}

void JNICALL Java_axo_jvm_Bridge_setRandomSeed(JNIEnv*, jclass, jlong randomPtr, jlong seed) {
    ((Random*)randomPtr)->setSeed(seed);
}

jint JNICALL Java_axo_jvm_Bridge_getBlock(JNIEnv*, jclass, jlong levelPtr, jint x, jint y, jint z) {
    if (t_surfaceBlocks) return Axo_GetSurfaceBlock(x, y, z);
    return ((Level*)levelPtr)->getTile(x, y, z);
}

void JNICALL Java_axo_jvm_Bridge_setBlock(JNIEnv*, jclass, jlong levelPtr, jint x, jint y, jint z, jint tile) {
    if (t_surfaceBlocks) {
        Axo_SetSurfaceBlock(x, y, z, tile, 0);
        return;
    }
    ((Level*)levelPtr)->setTileAndData(x, y, z, tile, 0, Tile::UPDATE_CLIENTS);
}

void JNICALL Java_axo_jvm_Bridge_setBlockWithData(JNIEnv*, jclass, jlong levelPtr, jint x, jint y, jint z, jint tile, jint data) {
    if (t_surfaceBlocks) {
        Axo_SetSurfaceBlock(x, y, z, tile, data);
        return;
    }
    ((Level*)levelPtr)->setTileAndData(x, y, z, tile, data, Tile::UPDATE_CLIENTS);
}

jint JNICALL Java_axo_jvm_Bridge_getBiomeAt(JNIEnv*, jclass, jlong levelPtr, jint x, jint z) {
    Level* level = (Level*)levelPtr;
    if (t_surfaceBlocks && level == nullptr) {
        return -1;
    }
    Biome* biome = level->getBiome(x, z);
    return biome ? biome->id : -1;
}

jint JNICALL Java_axo_jvm_Bridge_getSeaLevel(JNIEnv*, jclass, jlong levelPtr) {
    return ((Level*)levelPtr)->seaLevel;
}

void JNICALL Java_axo_jvm_Bridge_registerBiomeSpawn(JNIEnv*, jclass, jint id, jint category, jint weight) {
    if (id < 0 || id > 255 || category < 0 || category > 3 || weight <= 0) return;
    Biome* biome = Biome::biomes[id];
    if (!biome) return;
    g_axoBiomeSpawns[category].push_back({biome, weight});
}

static void Axo_AppendSpawnList(BiomeArray& list, std::vector<AxoBiomeSpawn>& spawns) {
    if (spawns.empty()) return;
    unsigned int oldLen = list.length;
    unsigned int add = 0;
    for (size_t i = 0; i < spawns.size(); i++) add += spawns[i].weight;
    list.resize(oldLen + add);
    unsigned int idx = oldLen;
    for (size_t i = 0; i < spawns.size(); i++) {
        for (int w = 0; w < spawns[i].weight; w++) {
            list.data[idx++] = spawns[i].biome;
        }
    }
}

void AxoBridge_ApplyBiomeSpawns(BiomeArray& desertBiomes, BiomeArray& warmBiomes, BiomeArray& coolBiomes, BiomeArray& icyBiomes) {
    Axo_AppendSpawnList(desertBiomes, g_axoBiomeSpawns[0]);
    Axo_AppendSpawnList(warmBiomes, g_axoBiomeSpawns[1]);
    Axo_AppendSpawnList(coolBiomes, g_axoBiomeSpawns[2]);
    Axo_AppendSpawnList(icyBiomes, g_axoBiomeSpawns[3]);
}