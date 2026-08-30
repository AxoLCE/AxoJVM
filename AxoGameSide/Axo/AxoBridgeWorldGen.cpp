#include <windows.h>
#include <vector>
#include <stdio.h>
#include "include/jni.h"
#include "AxoBridge.h"
#include "../Level.h"
#include "../Random.h"
#include "../Biome.h"
#include "../Tile.h"
#include "../BiomeInitLayer.h"
#include "../LevelChunk.h"
#include "../Dimension.h"

struct AxoBiomeSpawn {
    Biome* biome;
    int weight;
};

struct AxoDimensionBiomeSpawn {
    jint biomeId;
    jint weight;
};

static std::vector<AxoBiomeSpawn> g_axoBiomeSpawns[4];
static std::vector<AxoDimensionBiomeSpawn> g_axoDimensionBiomeSpawns[3];

static Biome* g_dimensionVanillaBiome[3] = { nullptr, nullptr, nullptr };
static bool g_dimensionBiomeLogged[3] = { false, false, false };

static thread_local byte* t_surfaceBlocks = nullptr;
static thread_local int t_surfaceOriginX = 0;
static thread_local int t_surfaceOriginZ = 0;

static jclass g_worldGenRegistryClass = nullptr;
static jmethodID g_runSurfaceMethod = nullptr;
static jmethodID g_runDecorateMethod = nullptr;

static jint Axo_NormalizeDimension(Level* level) {
    if (!level || !level->dimension) return 0;
    if (level->dimension->id == -1) return 1;
    if (level->dimension->id == 1) return 2;
    return 0;
}

static Biome* Axo_VanillaDimensionBiome(jint dimension) {
    if (dimension == 1) return Biome::hell;
    if (dimension == 2) return Biome::sky;
    return nullptr;
}

void JNICALL Java_axo_jvm_Bridge_registerDimensionBiomeSpawn(JNIEnv*, jclass, jint biomeId, jint dimension, jint weight) {
    if (biomeId < 0 || biomeId > 255 || dimension < 0 || dimension > 2 || weight <= 0) return;
    AxoDimensionBiomeSpawn spawn;
    spawn.biomeId = biomeId;
    spawn.weight = weight;
    g_axoDimensionBiomeSpawns[dimension].push_back(spawn);
}

static Biome* Axo_PickDimensionBiome(jint dimension, int64_t seed, int xOffs, int zOffs, Biome* vanilla) {
    std::vector<AxoDimensionBiomeSpawn>& spawns = g_axoDimensionBiomeSpawns[dimension];
    if (spawns.empty()) return nullptr;
    jint vanillaWeight = vanilla ? 256 : 0;
    jint totalWeight = vanillaWeight;
    for (size_t i = 0; i < spawns.size(); i++) totalWeight += spawns[i].weight;
    if (totalWeight <= 0) return nullptr;
    uint64_t h = (uint64_t)seed ^ ((uint64_t)xOffs * 341873128712ULL) ^ ((uint64_t)zOffs * 132897987541ULL) ^ ((uint64_t)dimension * 668268263ULL);
    h ^= h >> 33;
    h *= 0xFF51AFD7ED558CCDULL;
    h ^= h >> 33;
    h *= 0xC4CEB9FE1A85EC53ULL;
    h ^= h >> 33;
    jint roll = (jint)(h % (uint64_t)totalWeight);
    if (vanillaWeight > 0 && roll < vanillaWeight) return vanilla;
    roll -= vanillaWeight;
    for (size_t i = 0; i < spawns.size(); i++) {
        roll -= spawns[i].weight;
        if (roll < 0) {
            Biome* biome = Biome::biomes[spawns[i].biomeId];
            if (biome) return biome;
        }
    }
    return Biome::biomes[spawns[spawns.size() - 1].biomeId];
}

void AxoBridge_ApplyDimensionBiomes(LevelChunk* levelChunk, Level* level, int xOffs, int zOffs) {
    jint dimension = Axo_NormalizeDimension(level);
    if (dimension == 0) return;
    if (g_axoDimensionBiomeSpawns[dimension].empty()) return;

    Biome* vanilla = nullptr;
    int firstId = -1;
    if (levelChunk && levelChunk->biomes.length > 0) {
        firstId = levelChunk->biomes.data[0] & 0xFF;
        bool uniform = true;
        for (unsigned int i = 1; i < levelChunk->biomes.length; i++) {
            if ((levelChunk->biomes.data[i] & 0xFF) != firstId) {
                uniform = false;
                break;
            }
        }
        if (uniform && firstId > 0 && Biome::biomes[firstId]) vanilla = Biome::biomes[firstId];
    }
    if (!vanilla) vanilla = Axo_VanillaDimensionBiome(dimension);

    if (!g_dimensionBiomeLogged[dimension]) {
        g_dimensionBiomeLogged[dimension] = true;
        printf("[AxoJVM] Dimension biome hook: dim=%d len=%u firstId=%d vanillaId=%d\n",
            dimension, levelChunk ? levelChunk->biomes.length : 0, firstId, vanilla ? vanilla->id : -1);
        fflush(stdout);
    }

    if (!vanilla) return;
    g_dimensionVanillaBiome[dimension] = vanilla;

    Biome* biome = Axo_PickDimensionBiome(dimension, level->getSeed(), xOffs, zOffs, vanilla);
    if (!biome || biome == vanilla) return;
    if (levelChunk->biomes.length == 0) levelChunk->biomes.resize(256);
    for (unsigned int i = 0; i < levelChunk->biomes.length; i++) {
        levelChunk->biomes.data[i] = (byte)biome->id;
    }
}

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
    g_runSurfaceMethod = env->GetStaticMethodID(g_worldGenRegistryClass, "runSurface", "(JIIJII)V");
    g_runDecorateMethod = env->GetStaticMethodID(g_worldGenRegistryClass, "runDecorate", "(JIIJII)V");
    if (env->ExceptionCheck()) env->ExceptionClear();
}

void AxoBridge_RunSurfaceGen(Level* level, byteArray blocks, int xOffs, int zOffs) {
    JNIEnv* env = Axo_WorldGenEnv();
    if (!env) return;
    AxoWorldGen_CacheClasses(env);
    if (!g_worldGenRegistryClass || !g_runSurfaceMethod) return;

    t_surfaceBlocks = blocks.data;
    t_surfaceOriginX = xOffs * 16;
    t_surfaceOriginZ = zOffs * 16;

    jint dimension = Axo_NormalizeDimension(level);
    Biome* biome = nullptr;
    if (dimension != 0) {
        biome = Axo_PickDimensionBiome(dimension, level->getSeed(), xOffs, zOffs, g_dimensionVanillaBiome[dimension]);
    }
    if (!biome) biome = level->getBiome(t_surfaceOriginX + 8, t_surfaceOriginZ + 8);
    int biomeId = biome ? biome->id : -1;

    Random surfaceRandom((int64_t)t_surfaceOriginX * 341873128712LL + (int64_t)t_surfaceOriginZ * 132897987541LL);

    env->CallStaticVoidMethod(g_worldGenRegistryClass, g_runSurfaceMethod,
        (jlong)(intptr_t)level, (jint)t_surfaceOriginX, (jint)t_surfaceOriginZ,
        (jlong)(intptr_t)&surfaceRandom, (jint)biomeId, dimension);
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
    jint dimension = Axo_NormalizeDimension(level);
    Biome* biome = level->getBiome(xo + 16, zo + 16);
    int biomeId = biome ? biome->id : -1;

    env->CallStaticVoidMethod(g_worldGenRegistryClass, g_runDecorateMethod,
        (jlong)(intptr_t)level, (jint)xo, (jint)zo, (jlong)(intptr_t)random, (jint)biomeId, dimension);
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
    if (tile < 0 || tile > 255) return;
    if (t_surfaceBlocks) {
        Axo_SetSurfaceBlock(x, y, z, tile, 0);
        return;
    }
    ((Level*)levelPtr)->setTileAndData(x, y, z, tile, 0, Tile::UPDATE_CLIENTS);
}

void JNICALL Java_axo_jvm_Bridge_setBlockWithData(JNIEnv*, jclass, jlong levelPtr, jint x, jint y, jint z, jint tile, jint data) {
    if (tile < 0 || tile > 255) return;
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