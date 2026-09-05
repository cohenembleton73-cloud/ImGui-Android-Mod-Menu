#include <jni.h>
#include <android/log.h>
#include "imgui.h"

#define LOG_TAG "MyModMenu"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Mod States (Buttons & Sliders)
bool flyEnabled = false;
bool speedEnabled = false;
bool noclipEnabled = false;
float speedMultiplier = 2.0f;

// This function draws the floating UI on screen
void RenderModMenu() {
    // Menu Window Setup
    ImGui::Begin("🔥 Custom Mod Menu 🔥", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Status: Active");
    ImGui::Separator();

    // Toggle Buttons
    ImGui::Checkbox("Fly Hack", &flyEnabled);
    ImGui::Checkbox("Speed Boost", &speedEnabled);
    ImGui::Checkbox("NoClip", &noclipEnabled);

    // Sliding Menu / Slider element
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

// Entry point called when the APK loads the .so library
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("Mod Menu Loaded Successfully!");
    return JNI_VERSION_1_6;
}
