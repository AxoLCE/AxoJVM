#include <stdio.h>
#include <windows.h>
#include "include/jni.h"

typedef jint(JNICALL* CreateJVM_t)(JavaVM**, void**, void*);

static JavaVM* g_jvm = nullptr;
static JNIEnv* g_env = nullptr;
static HMODULE g_jvmDll = nullptr;

extern "C" void AxoBridge_BootstrapJVM() {
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    SetConsoleTitleA("AxoJVM Console");

    printf("[AxoJVM] Bootstrapping JVM...\n");
    fflush(stdout);

    // Load jvm.dll
    g_jvmDll = LoadLibraryA("jvm/bin/server/jvm.dll");
    //if (!g_jvmDll) {
    //    const char* javaHome = getenv("JAVA_HOME");
    //    if (javaHome) {
    //        char path[260];
    //        snprintf(path, sizeof(path), "%s\\bin\\server\\jvm.dll", javaHome);
    //        g_jvmDll = LoadLibraryA(path);
    //    }
    //}

    if (!g_jvmDll) {
        printf("[AxoJVM] ERROR: jvm.dll not found\n");
        return;
    }

    printf("[AxoJVM] jvm.dll loaded\n");
    fflush(stdout);

    // Create JVM
    CreateJVM_t createJVM = (CreateJVM_t)GetProcAddress(g_jvmDll, "JNI_CreateJavaVM");
    if (!createJVM) {
        printf("[AxoJVM] ERROR: JNI_CreateJavaVM not found\n");
        FreeLibrary(g_jvmDll);
        g_jvmDll = nullptr;
        return;
    }

    printf("[AxoJVM] JNI_CreateJavaVM found\n");

    JavaVMOption options[2] = {};
    options[0].optionString = (char*)"-Djava.class.path=axojvm/axojvm-runtime.jar";
    options[1].optionString = (char*)"-Djava.home=jvm";

    JavaVMInitArgs args = {};
    args.version = JNI_VERSION_21;
    args.options = options;
    args.nOptions = 2;
    args.ignoreUnrecognized = JNI_TRUE;

    jint rc = createJVM(&g_jvm, (void**)&g_env, &args);
    if (rc != JNI_OK) {
        printf("[AxoJVM] ERROR: JavaVM failed with code %d\n", rc);
        FreeLibrary(g_jvmDll);
        g_jvmDll = nullptr;
        return;
    }

    printf("[AxoJVM] JavaVM created succesfully\n");
    fflush(stdout);

    // Find bridge class
    jclass bridgeClass = g_env->FindClass("axo/jvm/Bridge");
    if (!bridgeClass) {
        if (g_env->ExceptionCheck()) {
            g_env->ExceptionDescribe();
            g_env->ExceptionClear();
        }
        printf("[AxoJVM] ERROR: Bridge class not found\n");
        fflush(stdout);
    }

    // Call bootstrap
    jmethodID bootstrapMethod = g_env->GetStaticMethodID(bridgeClass, "bootstrap", "()V");
    if (!bootstrapMethod) {
        if (g_env->ExceptionCheck()) {
            g_env->ExceptionDescribe();
            g_env->ExceptionClear();
        }
        printf("[AxoJVM] ERROR: Bridge.bootstrap() method not found\n");
        fflush(stdout);
    }

    printf("[AxoJVM] Calling Bridge.bootstrap()...\n");
    fflush(stdout);

    g_env->CallStaticVoidMethod(bridgeClass, bootstrapMethod);
    if (g_env->ExceptionCheck()) {
        g_env->ExceptionDescribe();
        g_env->ExceptionClear();
        printf("[AxoJVM] ERROR: Java exception during bootstrap\n");
    }
    else {
        printf("[AxoJVM] Bootstrap complete\n");
    }
    fflush(stdout);
    g_env->DeleteLocalRef(bridgeClass);
}