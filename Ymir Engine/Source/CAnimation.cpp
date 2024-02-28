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
    animationNames[0] = "Ani1     (testing)";
    animationNames[1] = "Ani2     (testing)";
    animationNames[2] = "Ani3     (testing)";

}

CAnimation::~CAnimation()
{

}

void CAnimation::Update() {
    
    //ourAnimator.UpdateAnimation(TimeManager::DeltaTime);

}

void CAnimation::OnInspector() {

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::CollapsingHeader("Animation", flags))
    {
        ImGui::Indent();

        if (ImGui::BeginCombo("Animations", "Select animation")) {
            for (int i = 0; i < animationMaxNum; i++) {
                if (selectedAnimation == i) isSelected = true;
                if (selectedAnimation != i) isSelected = false;
                ImGui::Checkbox(animationNames[i].c_str(), &isSelected);

                if (ImGui::IsItemClicked()) {
                    selectedAnimation = i;
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
