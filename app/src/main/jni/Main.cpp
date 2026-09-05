#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include "imgui.h"

#define LOG_TAG "MyModMenu"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Mod States
bool showMenu = false;
bool wasYPressedLastFrame = false;

bool flyEnabled = false;
bool speedEnabled = false;
bool noclipEnabled = false;
float speedMultiplier = 2.0f;

// Check if the 'Y' button (KeyCode 125 / 'y') is pressed down
bool IsYButtonPressed(JNIEnv* env) {
    if (!env) return false;
    
    jclass inputClass = env->FindClass("UnityEngine/Input");
    if (!inputClass) return false;

    jmethodID getKeyMethod = env->GetStaticMethodID(inputClass, "GetKey", "(Ljava/lang/String;)Z");
    if (!getKeyMethod) return false;

    jstring keyString = env->NewStringUTF("y");
    jboolean pressed = env->CallStaticBooleanMethod(inputClass, getKeyMethod, keyString);
    env->DeleteLocalRef(keyString);

    return pressed;
}

// Function that handles button inputs and toggles visibility
void UpdateInput(JNIEnv* env) {
    bool isYDown = IsYButtonPressed(env);

    // Toggle on initial press down only
    if (isYDown && !wasYPressedLastFrame) {
        showMenu = !showMenu;
        LOGI("Menu toggled: %s", showMenu ? "ON" : "OFF");
    }
    wasYPressedLastFrame = isYDown;
}

// Draws the floating ImGui UI
void RenderModMenu() {
    if (!showMenu) return; // Do not draw if menu is toggled off

    ImGui::Begin("🔥 Custom Mod Menu 🔥", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Status: Active (Press Y to Hide)");
    ImGui::Separator();

    // Toggle Buttons
    ImGui::Checkbox("Fly Hack", &flyEnabled);
    ImGui::Checkbox("Speed Boost", &speedEnabled);
    ImGui::Checkbox("NoClip", &noclipEnabled);

    // Speed Slider
    if (speedEnabled) {
        ImGui::Spacing();
        ImGui::SliderFloat("Speed Multiplier", &speedMultiplier, 1.0f, 10.0f);
    }

    ImGui::Spacing();
    
    // Reset Action Button
    if (ImGui::Button("Reset All Mods")) {
        flyEnabled = false;
        speedEnabled = false;
        noclipEnabled = false;
        speedMultiplier = 2.0f;
        LOGI("All features reset.");
    }

    ImGui::End();
}

// Background thread to delay initialization until Unity finishes loading
void* InitThread(void* arg) {
    LOGI("Waiting 4 seconds for Unity engine initialization...");
    sleep(4);
    LOGI("Hooking input and render pipeline...");
    
    // Apply your graphics context hooks or ImGui setup here
    return NULL;
}

// Entry point called when APK loads .so library
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("Mod Menu Loaded Successfully!");

    // Launch background setup thread
    pthread_t thread;
    pthread_create(&thread, NULL, InitThread, NULL);

    return JNI_VERSION_1_6;
}
