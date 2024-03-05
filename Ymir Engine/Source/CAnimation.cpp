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
    //Initializing animator with an empty animation
    Animation* temp = new Animation();
    //Animation  test2, test3, test4;
    animator = new Animator(temp);
    //AddAnimation(test2, "Ani1     (testing)");
    //AddAnimation(test3, "Ani2     (testing)");
    //AddAnimation(test4, "Ani3     (testing)");

    active = true;

}

CAnimation::~CAnimation()
{
    animations.~vector();
}

void CAnimation::Update() {
    
    if (selectedAnimationPlaying != -1) {
        animator->UpdateAnimation(External->GetDT());
        //LOG("Updating Animation %s", &aniParamaters[selectedAnimationPlaying].name);
    }

}

void CAnimation::AddAnimation(Animation &newAnimation, std::string animationName) {
    animator->animations.push_back(newAnimation);
    totalAnimations++;
    animator->animations[totalAnimations].name = animationName;

}

void CAnimation::RemoveAnimation(int ID) {
    if (ID < animator->animations.size()) {
        animator->animations.erase(animator->animations.begin() + ID);
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
            aniName = animator->animations[selectedAnimation].name;
        }

        if (ImGui::BeginCombo("Animations", aniName.c_str())) {
            for (int i = 0; i < totalAnimations+1; i++) {
                if (selectedAnimation == i) isSelected = true;
                if (selectedAnimation != i) isSelected = false;
                ImGui::Checkbox(animator->animations[i].name.c_str(), &isSelected);
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

        if (!animator->animations.empty() && selectedAnimation != -1) {
            ImGui::Checkbox("Playing", &animator->animations[selectedAnimation].isPlaying);

            if (ImGui::IsItemClicked()) {

                !animator->animations[selectedAnimation].isPlaying;

                selectedAnimationPlaying = selectedAnimation;

                for (int i = 0; i < totalAnimations; i++) {

                    if (i != selectedAnimation) {

                        animator->animations[i].isPlaying = false;

                    }

                }

                //When click on play, update the animation to the current selected one
                //animator->PlayAnimation(&animations[selectedAnimationPlaying]);
            }

            ImGui::Checkbox("Loop", &animator->animations[selectedAnimation].isLoop);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].isLoop;
                //Do loop on current animation
            }

            ImGui::Checkbox("PingPong", &animator->animations[selectedAnimation].isPingPong);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].isPingPong;
                //Do PingPong on current animation
            }
        }

        ImGui::Unindent();
    }
}
