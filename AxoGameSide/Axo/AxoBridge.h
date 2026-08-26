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
    void Java_axo_jvm_Bridge_registerTile(
        JNIEnv* env, jclass,
        jint id,
        jstring jname,
        jstring jmaterial,
        jfloat destroyTime,
        jfloat explosionResistance,
        jstring jsoundType,
        jstring jiconName,
        jboolean isSolidRender
    );
#ifdef __cplusplus
}
#endif

#endif // AXO_BRIDGE_H
