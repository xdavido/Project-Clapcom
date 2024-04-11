#include "CAnimation.h"

#include "GameObject.h"

#include "CTransform.h"
#include "CMesh.h"
#include "TimeManager.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include "External/mmgr/mmgr.h"

#include "ModuleFileSystem.h"
#include "ModuleResourceManager.h"

CAnimation::CAnimation(GameObject* owner) : Component(owner, ComponentType::ANIMATION)
{
    //Initializing animator with an empty animation
    //Animation  test2, test3, test4;
    animator = new Animator();
    for (int i = 0; i < owner->mChildren.size(); i++) {
        if (owner->mChildren[i]->GetComponent(MATERIAL)) {
            CMaterial* cMat = (CMaterial*)owner->mChildren[i]->GetComponent(MATERIAL);
            cMat->shaderPath = SHADER_ANIMATION;
            cMat->shader.LoadShader(cMat->shaderPath);
        }
    }
    

    active = true;

}

CAnimation::~CAnimation()
{

}

void CAnimation::Update() {
    
    
    animator->UpdateAnimation(External->GetDT());


}

void CAnimation::AddAnimation(ResourceAnimation&newAnimation) {
    for (int i = 0; i < animator->animations.size(); i++) {
        if (newAnimation.name == animator->animations[i].name) {
            newAnimation.name.append("_Copy");
        }
    }

    animator->animations.push_back(newAnimation);

    if (animator->GetCurrentAnimation() == nullptr)
        animator->SetCurrentAnimation(&newAnimation);
}

void CAnimation::RemoveAnimation(int ID) {
    if (ID < animator->animations.size()) {
        animator->animations.erase(animator->animations.begin() + ID);
    }
    selectedAnimation = -1;
}

void CAnimation::PlayAnimation(std::string animationName)
{
    ResourceAnimation* animationToPlay = nullptr;

    for (int i = 0; i < animator->animations.size(); i++) {
        if (animator->animations[i].name == animationName) {
            animationToPlay = &animator->animations[i];
            break;
        }
    }

    if (animationToPlay != nullptr) {
        animator->PlayAnimation(animationToPlay);
    }
    else {
        LOG("Animation not found");
    }
}

void CAnimation::PauseAnimation() {

    animator->PauseAnimation();
}

void CAnimation::ResumeAnimation() {

    animator->ResumeAnimation();
}

void CAnimation::StopAnimation() {

    animator->StopAnimation();
}

void CAnimation::SetLoop(std::string animationName, bool loop) {

    if (animationName != "") {
        for (int i = 0; i < animator->animations.size(); i++) {
            if (animator->animations[i].name == animationName) {
                animator->animations[i].loop = loop;
                return;
            }
        }
    }
    else {
        for (int i = 0; i < animator->animations.size(); i++) {
            animator->animations[i].loop = loop;
        }
    }
}

void CAnimation::SetBackwards(std::string animationName, bool backwards) {

    if (animationName != "") {
        for (int i = 0; i < animator->animations.size(); i++) {
            if (animator->animations[i].name == animationName) {
                animator->animations[i].backwards = backwards;
                return;
            }
        }
    }
    else {
        for (int i = 0; i < animator->animations.size(); i++) {
            animator->animations[i].backwards = backwards;
        }
    }
}

void CAnimation::SetPingPong(std::string animationName, bool pingPong) {

    if (animationName != "") {
        for (int i = 0; i < animator->animations.size(); i++) {
            if (animator->animations[i].name == animationName) {
                animator->animations[i].pingPong = pingPong;
                return;
            }
        }
    }
    else {
        for (int i = 0; i < animator->animations.size(); i++) {
            animator->animations[i].pingPong = pingPong;
        }
    }
}

void CAnimation::SetSpeed(std::string animationName, float speed) {

    if (animationName != "") {
        for (int i = 0; i < animator->animations.size(); i++) {
            if (animator->animations[i].name == animationName) {
                animator->animations[i].speed = speed;
                return;
            }
        }
    }
    else {
        for (int i = 0; i < animator->animations.size(); i++) {
            animator->animations[i].speed = speed;
        }
    }
}

void CAnimation::AddBlendOption(std::string animationName, std::string blendName, float frames) {
    
    if (!animator->FindAnimation(blendName)) return;

    if (animationName != "") {
        for (int i = 0; i < animator->animations.size(); i++) {
            if (animator->animations[i].name == animationName) {
                animator->animations[i].blendMap.insert(std::make_pair(blendName, frames));
                return;
            }
        }
    }
    else {
        for (int i = 0; i < animator->animations.size(); i++) {
            animator->animations[i].blendMap.insert(std::make_pair(blendName, frames));;
        }
    }
}

void CAnimation::SetResetToZero(std::string animationName, bool resetToZero) {

    if (animationName != "") {
        for (int i = 0; i < animator->animations.size(); i++) {
            if (animator->animations[i].name == animationName) {
                animator->animations[i].resetToZero = resetToZero;
                return;
            }
        }
    }
    else {
        for (int i = 0; i < animator->animations.size(); i++) {
            animator->animations[i].resetToZero = resetToZero;
        }
    }
}

bool CAnimation::HasFinished(std::string animationName) {

    if (animator->previousAnimation == nullptr) return false;

    return (animationName == animator->previousAnimation->name &&
        animationName != animator->currentAnimation->name) ? true : false;
}

void CAnimation::YAnimDragDropTarget() {

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("yanim"))
        {
            std::string libraryFilePathDrop = (const char*)payload->Data;

            libraryFilePathDrop.erase(libraryFilePathDrop.find(".yanim") + 6);
            //libraryFilePathDrop = "./" + libraryFilePathDrop;
            /*for (char& c : libraryFilePathDrop) {
                if (c == '\\') {
                    c = '/';
                }
            }*/
            //libraryFilePathDrop.replace(libraryFilePathDrop.begin(), libraryFilePathDrop.end(), 'e', '/');

            LOG("File path: %s", libraryFilePathDrop.c_str());

            ResourceAnimation* rAnim = (ResourceAnimation*)External->resourceManager->CreateResourceFromLibrary(libraryFilePathDrop, ResourceType::ANIMATION, mOwner->UID);
            AddAnimation(*rAnim);
        }
        ImGui::EndDragDropTarget();
    }
}

void CAnimation::OnInspector() {

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    bool exists = true;

    ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
    ImGui::SameLine();


    if (ImGui::CollapsingHeader(("Animation##" + std::to_string(UID)).c_str(), &exists, flags))
    {
        ImGui::Indent();

        if (!active) { ImGui::BeginDisabled(); }

        std::string animationName;

        animationName = (selectedAnimation == -1) ? "None (Select animation)" : animator->animations[selectedAnimation].name;

        ImGui::Button("Drop .yanim to Add animation", ImVec2(200, 50));
        YAnimDragDropTarget();

        ImGui::Separator();

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

        ImGui::Indent(20.0f);

        if (!animator->animations.empty() && selectedAnimation != -1) {

            ImGui::Spacing();
            ImGui::Spacing();

            if (ImGui::InputText("Name", &animator->animations[selectedAnimation].name)) {

            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::SliderFloat("Playback Time", &animator->animations[selectedAnimation].currentTime, .0f, animator->animations[selectedAnimation].GetDuration())) {

            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::DragFloat("Speed", &animator->animations[selectedAnimation].speed, 1.0f, .0f, 100.0f)) {

            }

            ImGui::Checkbox("Loop", &animator->animations[selectedAnimation].loop);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].loop;
            }

            ImGui::SameLine();

            ImGui::Button("?");

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Activate looping, works with all buttons from below");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("PingPong", &animator->animations[selectedAnimation].pingPong);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].pingPong;
            }

            ImGui::SameLine();

            ImGui::Button("?");

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Goes from the start to the end and then back. Works with both Loop and backwards features");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("Backwards", &animator->animations[selectedAnimation].backwards);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].backwards;
            }

            ImGui::SameLine();

            ImGui::Button("?");

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Backwards starts the animation from the end. When combined with PingPong it does the PingPong effect starting from the end. It's incompatible with Ease-In and Ease-Out");
                ImGui::EndTooltip();
            }

            ImGui::Spacing();

            ImGui::Checkbox("Ease-In", &animator->animations[selectedAnimation].easeIn);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].easeIn;
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


            ImGui::Checkbox("Ease-Out", &animator->animations[selectedAnimation].easeOut);

            if (ImGui::IsItemClicked()) {
                !animator->animations[selectedAnimation].easeOut;
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

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Unindent();

            if (ImGui::Button("Play")) {
                animator->PlayAnimation(&animator->animations[selectedAnimation]);

            }

            ImGui::SameLine();

            if (ImGui::Button("Pause")) {

                animator->PauseAnimation();
            }

            ImGui::SameLine();

            if (ImGui::Button("Resume")) {

                animator->ResumeAnimation();
            }

            ImGui::SameLine();

            if (ImGui::Button("Stop")) {

                animator->StopAnimation();
            }

        }

        if (!active) { ImGui::EndDisabled(); }

        ImGui::Unindent();
    }
}
