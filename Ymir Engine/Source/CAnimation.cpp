#include "CAnimation.h"

#include "GameObject.h"
#include "Animation.h"
#include "CTransform.h"
#include "CMesh.h"
#include "TimeManager.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include "External/mmgr/mmgr.h"

CAnimation::CAnimation(GameObject* owner) : Component(owner, ComponentType::ANIMATION)
{
    //Initializing animator with an empty animation
    //Animation  test2, test3, test4;
    animator = new Animator();
    CMaterial* cMat = (CMaterial*)owner->GetComponent(MATERIAL);
    cMat->shaderPath = SHADER_ANIMATION;
    cMat->shader.LoadShader(cMat->shaderPath);

    active = true;

}

CAnimation::~CAnimation()
{

}

void CAnimation::Update() {
    
    if (animationPlaying) {
        animator->UpdateAnimation(External->GetDT());
    }

}

void CAnimation::AddAnimation(Animation &newAnimation) {

    animator->animations.push_back(newAnimation);
}

void CAnimation::RemoveAnimation(int ID) {
    if (ID < animator->animations.size()) {
        animator->animations.erase(animator->animations.begin() + ID);
    }
    selectedAnimation = -1;
}

void CAnimation::PlayAnimation(std::string animationName)
{
    Animation* animationToPLay = nullptr;

    if (animationName != "") {
        for (int i = 0; i < animator->animations.size(); i++) {
            if (animator->animations[i].name == animationName) {
                animationToPLay = &animator->animations[i];
                break;
            }
        }
    }

    if (animationToPLay != nullptr) {
        animator->PlayAnimation(animationToPLay);
    }
    else {
        LOG("Animation not found");
    }
}

void CAnimation::OnInspector() {

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::CollapsingHeader("Animation", flags))
    {
        ImGui::Indent();

        std::string animationName;

        animationName = (selectedAnimation == -1) ? "None (Select animation)" : animator->animations[selectedAnimation].name;

        if (ImGui::BeginCombo("Animations", animationName.c_str())) {
            for (int i = 0; i < animator->animations.size(); i++) {

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

            float currentTime = animator->GetCurrentAnimationTime();
            if (ImGui::SliderFloat("Playback Time", &currentTime, .0f, animator->GetCurrentAnimation()->GetDuration())) {
                animator->SetCurrentAnimationTime(currentTime);
            }

            float speed = animator->GetCurrentAnimation()->GetSpeed();
            if (ImGui::DragFloat("Speed", &speed, 1.0f, .0f, 100.0f)) {
                animator->GetCurrentAnimation()->SetSpeed(speed);
            }

            ImGui::Checkbox("Loop", &animator->GetCurrentAnimation()->loop);

            if (ImGui::IsItemClicked()) {
                !animator->GetCurrentAnimation()->loop;
            }

            ImGui::SameLine();

            ImGui::Button("?");

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Activate looping, works with all buttons from below");
                ImGui::EndTooltip();
            }


            ImGui::Checkbox("PingPong", &animator->GetCurrentAnimation()->pingPong);

            if (ImGui::IsItemClicked()) {
                !animator->GetCurrentAnimation()->pingPong;
            }

            ImGui::SameLine();

            ImGui::Button("?");

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Goes from the start to the end and then back. Works with both Loop and backwards features");
                ImGui::EndTooltip();
            }


            ImGui::Checkbox("Backwards", &animator->GetCurrentAnimation()->backwards);

            if (ImGui::IsItemClicked()) {
                !animator->GetCurrentAnimation()->backwards;
            }

            ImGui::SameLine();

            ImGui::Button("?");

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Backwards starts the animation from the end. When combined with PingPong it does the PingPong effect starting from the end. It's incompatible with Ease-In and Ease-Out");
                ImGui::EndTooltip();
            }


            ImGui::Checkbox("Ease-In", &animator->GetCurrentAnimation()->easeIn);

            if (ImGui::IsItemClicked()) {
                !animator->GetCurrentAnimation()->easeIn;
            }

            ImGui::SameLine();

            ImGui::Button("?");

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Ease-In only works properly when both PingPong and Backwards are not activated. You can decide the multiplier for more or less Ease-In speed, but the default (1.025) one or similar one are recommended");
                ImGui::EndTooltip();
            }

            ImGui::SameLine();

            ImGui::InputFloat("Factor", &animator->animations[selectedAnimation].easeInMultiplier);

            ImGui::Checkbox("Ease-Out", &animator->GetCurrentAnimation()->easeOut);

            if (ImGui::IsItemClicked()) {
                !animator->GetCurrentAnimation()->easeOut;
            }

            ImGui::SameLine();

            ImGui::Button("?");

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Ease-Out only works properly when both PingPong and Backwards are not activated. You can decide the multiplier for more or less Ease-Out speed, but the default one (0.995) or similar one are recommended");
                ImGui::EndTooltip();
            }

            ImGui::SameLine();

            ImGui::InputFloat("Factor", &animator->animations[selectedAnimation].easeOutMultiplier);

            if (ImGui::Button("Play")) {

                animator->SetCurrentAnimationTime(.0f);
                animator->GetCurrentAnimation()->isPlaying = true;

            }

            ImGui::SameLine();

            if (ImGui::Button("Resume")) {

                animator->ResumeAnimation();
            }

            ImGui::SameLine();

            if (ImGui::Button("Stop")) {

                animator->PauseAnimation();
            }


        }

        ImGui::Unindent();
    }
}
