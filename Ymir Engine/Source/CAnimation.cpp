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
    //Animation  test2, test3, test4;
    animator = new Animator();
    //AddAnimation(test2, "Ani1     (testing)");
    //AddAnimation(test3, "Ani2     (testing)");
    //AddAnimation(test4, "Ani3     (testing)");

    active = true;

}

CAnimation::~CAnimation()
{

}

void CAnimation::Update() {
    
    if (selectedAnimationPlaying != -1 && animator->GetCurrentAnimation()->isPlaying) {
        animator->UpdateAnimation(External->GetDT());
    }

}

void CAnimation::AddAnimation(Animation &newAnimation, std::string animationName) {
    animator->PlayAnimation(&newAnimation);
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

        std::string animationName;

        animationName = (selectedAnimation == -1) ? "None (Select animation)" : animator->animations[selectedAnimation].name;

        if (ImGui::BeginCombo("Animations", animationName.c_str())) {
            for (int i = 0; i < totalAnimations+1; i++) {

                isSelected = (selectedAnimation == i) ? true : false;

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

            isSelected = (selectedAnimation == -1) ? true : false;
   

            ImGui::Checkbox("None", &isSelected);

            if (ImGui::IsItemClicked()) {
                selectedAnimation = -1;
            }

            ImGui::EndCombo();
        }

        if (!animator->animations.empty() && selectedAnimation != -1) {
            float speed = animator->GetCurrentAnimation()->GetSpeed();
            if (ImGui::DragFloat("Speed", &speed)) {
                animator->GetCurrentAnimation()->SetSpeed(speed);
            }

            ImGui::Checkbox("Loop", &animator->GetCurrentAnimation()->loop);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].loop;
                //Do loop on current animation
            }

            ImGui::Checkbox("PingPong", &animator->GetCurrentAnimation()->pingPong);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].pingPong;
                //Do PingPong on current animation
            }

            if (ImGui::Button("Play")) {

                animator->GetCurrentAnimation()->isPlaying = true;

                selectedAnimationPlaying = selectedAnimation;

                for (int i = 0; i < totalAnimations; i++) {

                    if (i != selectedAnimation) {

                        animator->animations[i].isPlaying = false;

                    }

                }

                if (animator->GetCurrentAnimation()->isPlaying == false) {
                    selectedAnimationPlaying = -1;
                }
            }
        }

        ImGui::Unindent();
    }
}
