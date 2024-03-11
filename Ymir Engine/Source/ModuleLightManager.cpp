#include "ModuleLightManager.h"

#include "ModuleScene.h"

ModuleLightManager::ModuleLightManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    LOG("Creating ModuleLightManager");



}

ModuleLightManager::~ModuleLightManager()
{

}

Light* ModuleLightManager::CreateLight(LightType type)
{
    Light* tmpLight = nullptr;
    //CLight* tmpComponentLight = nullptr;

    switch (type) {

        case LightType::UNKNOWN:
        {
            tmpLight = static_cast<Light*>(App->scene->CreateGameObject("Light", App->scene->mRootNode));

            break;
        }
        case LightType::POINT_LIGHT:
        {
            tmpLight = static_cast<PointLight*>(App->scene->CreateGameObject("Point Light", App->scene->mRootNode));

            break;
        }
        case LightType::DIRECTIONAL_LIGHT:
        {
            tmpLight = static_cast<DirectionalLight*>(App->scene->CreateGameObject("Directional Light", App->scene->mRootNode));

            break;
        }
        case LightType::SPOT_LIGHT:
        {
            tmpLight = static_cast<SpotLight*>(App->scene->CreateGameObject("Spot Light", App->scene->mRootNode));

            break;
        }
        case LightType::AREA_LIGHT:
        {
            tmpLight = static_cast<AreaLight*>(App->scene->CreateGameObject("Area Light", App->scene->mRootNode));

            break;
        }
    }

    if (tmpLight) {

        //tmpComponentLight = new CLight(tmpLight);
        //tmpLight->AddComponent(tmpComponentLight);

    }   

    return tmpLight;
}
