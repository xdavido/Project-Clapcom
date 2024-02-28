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
    //The model should probably be taken from the module renderer model array, need to figure out how
    //Model ourModel("");
    //Only one animation, should be changed into an array of animations in the future
    //Animation ourAnimation("",&ourModel);
  
    //ourAnimator(&ourAnimation);
    //animationNames[0] = "Ani1     (testing)";
    //animationNames[1] = "Ani2     (testing)";
    //animationNames[2] = "Ani3     (testing)";
    Animation test1, test2, test3;
    AddAnimation(test1, "Ani1     (testing)");
    AddAnimation(test2, "Ani2     (testing)");
    AddAnimation(test3, "Ani3     (testing)");

}

CAnimation::~CAnimation()
{

}

void CAnimation::Update() {
    
    //animator->UpdateAnimation(0.5f);
    LOG("Updating Animation");

}

void CAnimation::AddAnimation(Animation newAnimation, std::string animationName) {
    for (int i = 0; i < sizeof(animations); i++) {
        if (animationNames[i].empty()) {
            animationNames[i] = animationName;
            animations.push_back(newAnimation);
            totalAnimations++;
            break;
        }
    }
}

void CAnimation::RemoveAnimation(int ID) {
    if (ID < animations.size()) {
        animations.erase(animations.begin() + ID);
    }
    selectedAnimation = 0;
    for (int i = ID; i < (totalAnimations-1); i++) {
        animationNames[i] = animationNames[i + 1];
    }
    totalAnimations--;
}

void CAnimation::OnInspector() {

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::CollapsingHeader("Animation", flags))
    {
        ImGui::Indent();

        if (ImGui::BeginCombo("Animations", "Select animation")) {
            for (int i = 0; i < totalAnimations; i++) {
                if (selectedAnimation == i) isSelected = true;
                if (selectedAnimation != i) isSelected = false;
                ImGui::Checkbox(animationNames[i].c_str(), &isSelected);
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

            ImGui::EndCombo();
        }

        ImGui::Checkbox("Playing", &isPlaying);

        if (ImGui::IsItemClicked()) {
            !isPlaying ;
        }

        ImGui::Checkbox("Loop", &isLoop);

        if (ImGui::IsItemClicked()) {
            !isLoop;
        }

        ImGui::Checkbox("PingPong", &isPingPong);

        if (ImGui::IsItemClicked()) {
            !isPingPong;
        }
        //ImGui::Spacing();

        //ImGui::SeparatorText("Animation");

        ImGui::Unindent();
    }
}
