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
    void Java_axo_jvm_Bridge_registerBiome(JNIEnv* env, jclass, jint, jstring, jint, jint, jfloat, jfloat, jint, jint);
    void AxoBridge_PaintCustomTextures(BufferedImage* atlasImage, int iconType);
    void AxoBridge_RegisterCustomIcons(PreStitchedTextureMap* textureMap);
#ifdef __cplusplus
}
#endif

#endif // AXO_BRIDGE_H
