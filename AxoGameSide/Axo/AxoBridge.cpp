#include <stdio.h>
#include <windows.h>
#include "include/jni.h"
#include "AxoBridge.h"

typedef jint(JNICALL* CreateJVM_t)(JavaVM**, void**, void*);

static JavaVM* g_jvm = nullptr;
static JNIEnv* g_env = nullptr;
static HMODULE g_jvmDll = nullptr;

// Detect which side is running axo
const char* DetectGameSide() {
    unsigned char* base = (unsigned char*)GetModuleHandleA(NULL);
    if (!base) return "client";
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)base;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(base + dosHeader->e_lfanew);
    WORD subsystem = ntHeaders->OptionalHeader.Subsystem;

    if (subsystem == 3) {
        return "server";
    }

    return "client";
}

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

    const char* gameSide = DetectGameSide();

    printf("[AxoJVM] Gameside: %s\n", gameSide);
    fflush(stdout);

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
    static char sideOptionBuf[64];
    snprintf(sideOptionBuf, sizeof(sideOptionBuf), "-Daxo.game.side=%s", gameSide);
    JavaVMOption options[3] = {};
    options[0].optionString = (char*)"-Djava.class.path=axojvm/axojvm-runtime.jar";
    options[1].optionString = (char*)"-Djava.home=jvm";
    options[2].optionString = sideOptionBuf;

    JavaVMInitArgs args = {};
    args.version = JNI_VERSION_21;
    args.options = options;
    args.nOptions = 3;
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
        return;
    }

    // Register to native
    JNINativeMethod methods[] = {
    {"registerTile", 
     "(ILjava/lang/String;Ljava/lang/String;FFLjava/lang/String;Ljava/lang/String;Z)V",
     (void*)Java_axo_jvm_Bridge_registerTile},
    {"registerItem", "(ILjava/lang/String;Ljava/lang/String;I)V",
     (void*)Java_axo_jvm_Bridge_registerItem}
     };

    jint jrc = g_env->RegisterNatives(bridgeClass, methods, 2);
    if (jrc != JNI_OK) {
        if (g_env->ExceptionCheck()) { g_env->ExceptionDescribe(); g_env->ExceptionClear(); }
        printf("[AxoJVM] ERROR: RegisterNatives failed\n");
        return;
    }
    printf("[AxoJVM] Native methods registered\n");
    fflush(stdout);

    // Call bootstrap
    jmethodID bootstrapMethod = g_env->GetStaticMethodID(bridgeClass, "bootstrap", "()V");
    if (!bootstrapMethod) {
        if (g_env->ExceptionCheck()) {
            g_env->ExceptionDescribe();
            g_env->ExceptionClear();
        }
        printf("[AxoJVM] ERROR: Bridge.bootstrap() method not found\n");
        fflush(stdout);
        return;
    }

    printf("[AxoJVM] Calling Bridge.bootstrap()...\n");
    fflush(stdout);

    g_env->CallStaticVoidMethod(bridgeClass, bootstrapMethod);
    if (g_env->ExceptionCheck()) {
        g_env->ExceptionDescribe();
        g_env->ExceptionClear();
        printf("[AxoJVM] ERROR: Java exception during bootstrap\n");
        return;
    }
    else {
        printf("[AxoJVM] Bootstrap complete\n");
    }
    fflush(stdout);
    g_env->DeleteLocalRef(bridgeClass);

}

extern "C" void AxoBridge_ShutdownJVM() {
    if (!g_jvm || !g_env) {
        return;
    }
    printf("[AxoJVM] Requesting JVM shutdown...\n");
    fflush(stdout);

    jclass bridgeClass = g_env->FindClass("axo/jvm/Bridge");
    if (bridgeClass) {
        jmethodID shutdownMethod = g_env->GetStaticMethodID(bridgeClass, "shutdown", "()V");
        if (shutdownMethod) {
            g_env->CallStaticVoidMethod(bridgeClass, shutdownMethod);
            fflush(stdout);
            fflush(stderr);
        }
        g_env->DeleteLocalRef(bridgeClass);
    }
    if (g_env->ExceptionCheck()) {
        g_env->ExceptionDescribe();
        g_env->ExceptionClear();
    }
    printf("[AxoJVM] Destroying JVM\n");
    fflush(stdout);
    g_jvm->DestroyJavaVM();
    g_jvm = nullptr;
    g_env = nullptr;
    if (g_jvmDll) {
        FreeLibrary(g_jvmDll);
        g_jvmDll = nullptr;
    }
    printf("[AxoJVM] JVM shutdown complete\n");
    fflush(stdout);
}
