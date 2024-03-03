#include "CAnimation.h"

#include "GameObject.h"
#include "Animation.h"
#include "AnimationController.h"
#include "CTransform.h"
#include "CMesh.h"
#include "TimeManager.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

CAnimation::CAnimation(GameObject* owner) : Component(owner, ComponentType::ANIMATION)
{
    //Just for testing, need to remove from GameObject.h and GameObject.cpp when done with testing
    Animation test1, test2, test3;
    AddAnimation(test1, "Ani1     (testing)");
    AddAnimation(test2, "Ani2     (testing)");
    AddAnimation(test3, "Ani3     (testing)");

    active = true;

}

CAnimation::~CAnimation()
{
    animations.~vector();
    aniParamaters.~vector();
}

void CAnimation::Update() {
    
    if (selectedAnimationPlaying != -1) {
        animator->UpdateAnimation(0.1f);
        //LOG("Updating Animation %s", &aniParamaters[selectedAnimationPlaying].name);
    }

}

void CAnimation::AddAnimation(Animation newAnimation, std::string animationName) {
    AnimationParameters temp(animationName);
    animations.push_back(newAnimation);

    aniParamaters.push_back(temp);
    totalAnimations++;

}

void CAnimation::RemoveAnimation(int ID) {
    if (ID < animations.size()) {
        animations.erase(animations.begin() + ID);
        aniParamaters.erase(aniParamaters.begin() + ID);
    }
    selectedAnimation = -1;
    selectedAnimationPlaying = -1;

    totalAnimations--;
}

void CAnimation::OnInspector() {

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::CollapsingHeader("Animation", flags))
    {
        ImGui::Indent();

        if (selectedAnimation == -1) {
            aniName = "None (Select animation)";
        }
        if (selectedAnimation != -1) {
            aniName = aniParamaters[selectedAnimation].name;
        }

        if (ImGui::BeginCombo("Animations", aniName.c_str())) {
            for (int i = 0; i < totalAnimations; i++) {
                if (selectedAnimation == i) isSelected = true;
                if (selectedAnimation != i) isSelected = false;
                ImGui::Checkbox(aniParamaters[i].name.c_str(), &isSelected);
                ImGui::SameLine(); 

                if (ImGui::IsItemClicked()) {
                    selectedAnimation = i;
                }

                if (ImGui::Button("Remove")) {

                }

                if (ImGui::IsItemClicked()) {
                    RemoveAnimation(i);
                }

            }

            if (selectedAnimation == -1) isSelected = true;
            if (selectedAnimation != -1) isSelected = false;

            ImGui::Checkbox("None", &isSelected);

            if (ImGui::IsItemClicked()) {
                selectedAnimation = -1;
            }

            ImGui::EndCombo();
        }

        if (!animations.empty() && selectedAnimation != -1) {
            ImGui::Checkbox("Playing", &aniParamaters[selectedAnimation].isPlaying);

            if (ImGui::IsItemClicked()) {

                !aniParamaters[selectedAnimation].isPlaying;

                selectedAnimationPlaying = selectedAnimation;

                for (int i = 0; i < totalAnimations; i++) {

                    if (i != selectedAnimation) {

                        aniParamaters[i].isPlaying = false;

                    }

                }

                //When click on play, update the animation to the current selected one
                //animator->PlayAnimation(&animations[selectedAnimationPlaying]);
            }

            ImGui::Checkbox("Loop", &aniParamaters[selectedAnimation].isLoop);

            if (ImGui::IsItemClicked()) {
                !aniParamaters[selectedAnimation].isLoop;
                //Do loop on current animation
            }

            ImGui::Checkbox("PingPong", &aniParamaters[selectedAnimation].isPingPong);

            if (ImGui::IsItemClicked()) {
                !aniParamaters[selectedAnimation].isPingPong;
                //Do PingPong on current animation
            }
        }

        ImGui::Unindent();
    }
}
