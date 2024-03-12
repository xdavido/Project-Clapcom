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
    switch (type) {

        //case LightType::UNKNOWN:
        //{
        //    Light* tmpLight = static_cast<Light*>(App->scene->CreateGameObject("Light", App->scene->mRootNode));
        //    CLight* tmpComponentLight = new CLight(tmpLight, LightType::UNKNOWN);
        //    tmpLight->AddComponent(tmpComponentLight);
        //    return tmpLight;

        //    break;
        //}
        case LightType::POINT_LIGHT:
        {
            PointLight* tmpLight = new PointLight();
            tmpLight->lightGO = App->scene->CreateGameObject("Point Light", App->scene->mRootNode);

            CLight* tmpComponentLight = new CLight(tmpLight->lightGO, tmpLight);
            tmpLight->lightGO->AddComponent(tmpComponentLight);

            return tmpLight;

            break;
        }
        /*case LightType::DIRECTIONAL_LIGHT:
        {
            DirectionalLight* tmpLight = static_cast<DirectionalLight*>(App->scene->CreateGameObject("Directional Light", App->scene->mRootNode));
            CLight* tmpComponentLight = new CLight(tmpLight, LightType::DIRECTIONAL_LIGHT);
            tmpLight->AddComponent(tmpComponentLight);
            return tmpLight;

            break;
        }*/
        //case LightType::SPOT_LIGHT:
        //{
        //    SpotLight* tmpLight = static_cast<SpotLight*>(App->scene->CreateGameObject("Spot Light", App->scene->mRootNode));
        //    CLight* tmpComponentLight = new CLight(tmpLight, LightType::SPOT_LIGHT);
        //    tmpLight->AddComponent(tmpComponentLight);
        //    return tmpLight;

        //    break;
        //}
        //case LightType::AREA_LIGHT:
        //{
        //    AreaLight* tmpLight = static_cast<AreaLight*>(App->scene->CreateGameObject("Area Light", App->scene->mRootNode));
        //    CLight* tmpComponentLight = new CLight(tmpLight, LightType::AREA_LIGHT);
        //    tmpLight->AddComponent(tmpComponentLight);
        //    return tmpLight;

        //    break;
        //}
    }

}
