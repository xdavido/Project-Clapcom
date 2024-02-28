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

}

CAnimation::~CAnimation()
{

}

void CAnimation::Update() {
    
    animator->UpdateAnimation(0.5f);
    LOG("Updating Animation");
}

void CAnimation::OnInspector() {

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::CollapsingHeader("Animation", flags))
    {
        ImGui::Indent();

        ImGui::Spacing();

        ImGui::SeparatorText("Animation");

        ImGui::Unindent();
    }
}
