#ifndef AXO_BRIDGE_H
#define AXO_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif
	void AxoBridge_BootstrapJVM();
	void AxoBridge_ShutdownJVM();
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C++" {
#endif
    #include "include/jni.h"
    JNIEnv* Axo_GetJNIEnv();
    JavaVM* Axo_GetJavaVM();
    void Java_axo_jvm_Bridge_registerTile(
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
        jboolean jCanWalkThrough,
        jstring jRegistryName
    );

    void Java_axo_jvm_Bridge_registerItem(
        JNIEnv* env, jclass,
        jint id,
        jstring jname,
        jstring jiconName,
        jint maxStackSize,
        jstring jRegistryName
    );
#ifdef __cplusplus
}
#endif

class PreStitchedTextureMap;

#ifdef __cplusplus
extern "C++" {
#endif
    class BufferedImage;
    std::wstring AxoBridge_GetLang(const std::string& key);
    void Java_axo_jvm_Bridge_registerCrop(JNIEnv* env, jclass, jint, jstring, jobjectArray, jint, jint, jstring jRegistryName);
    void Java_axo_jvm_Bridge_registerSeed(JNIEnv* env, jclass,jint id,jstring jname,jstring jiconName,jint maxStackSize,jint plantBlockId, jstring jRegistryName);
    void Java_axo_jvm_Bridge_registerBiome(JNIEnv* env, jclass, jint, jstring, jint, jint, jint, jint, jfloat, jfloat, jfloat, jfloat, jint, jint);
    void AxoBridge_PaintCustomTextures(BufferedImage* atlasImage, int iconType);
    void AxoBridge_RegisterCustomIcons(PreStitchedTextureMap* textureMap);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C++" {
#endif
    #include "../ArrayWithLength.h"

    class Level;
    class Random;
    class LevelChunk;

    void AxoBridge_RunSurfaceGen(Level* level, byteArray blocks, int xOffs, int zOffs);
    void AxoBridge_RunDecorateGen(Level* level, Random* random, int xt, int zt);
    void AxoBridge_ApplyBiomeSpawns(BiomeArray& desertBiomes, BiomeArray& warmBiomes, BiomeArray& coolBiomes, BiomeArray& icyBiomes);
    void AxoBridge_ApplyDimensionBiomes(LevelChunk* levelChunk, Level* level, int xOffs, int zOffs);
    void Java_axo_jvm_Bridge_registerDimensionBiomeSpawn(JNIEnv*, jclass, jint, jint, jint);

    jint Java_axo_jvm_Bridge_randomInt(JNIEnv*, jclass, jlong, jint);
    void Java_axo_jvm_Bridge_setRandomSeed(JNIEnv*, jclass, jlong, jlong);
    jint Java_axo_jvm_Bridge_getBlock(JNIEnv*, jclass, jlong, jint, jint, jint);
    void Java_axo_jvm_Bridge_setBlock(JNIEnv*, jclass, jlong, jint, jint, jint, jint);
    void Java_axo_jvm_Bridge_setBlockWithData(JNIEnv*, jclass, jlong, jint, jint, jint, jint, jint);
    jint Java_axo_jvm_Bridge_getBiomeAt(JNIEnv*, jclass, jlong, jint, jint);
    jint Java_axo_jvm_Bridge_getSeaLevel(JNIEnv*, jclass, jlong);
    void Java_axo_jvm_Bridge_registerBiomeSpawn(JNIEnv*, jclass, jint, jint, jint);
#ifdef __cplusplus
}
#endif
#endif // AXO_BRIDGE_H