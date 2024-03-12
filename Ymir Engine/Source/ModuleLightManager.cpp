#include "ModuleLightManager.h"

#include "ModuleScene.h"

ModuleLightManager::ModuleLightManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    LOG("Creating ModuleLightManager");

    debuglightsEnabled = true;

}

ModuleLightManager::~ModuleLightManager()
{

}

void ModuleLightManager::EnableLightDebug(bool enable)
{
    debuglightsEnabled = enable;
}

bool ModuleLightManager::IsLightDebugEnabled()
{
    return debuglightsEnabled;
}

Light* ModuleLightManager::CreateLight(LightType type)
{
    switch (type) {

        case LightType::POINT_LIGHT:
        {
            PointLight* tmpLight = new PointLight();
            tmpLight->lightGO = App->scene->CreateGameObject("Point Light", App->scene->mRootNode);

            CLight* tmpComponentLight = new CLight(tmpLight->lightGO, tmpLight);
            tmpLight->lightGO->AddComponent(tmpComponentLight);

            lights.push_back(tmpLight);

            return tmpLight;

            break;
        }
        case LightType::DIRECTIONAL_LIGHT:
        {
            DirectionalLight* tmpLight = new DirectionalLight();
            tmpLight->lightGO = App->scene->CreateGameObject("Directional Light", App->scene->mRootNode);

            CLight* tmpComponentLight = new CLight(tmpLight->lightGO, tmpLight);
            tmpLight->lightGO->AddComponent(tmpComponentLight);

            lights.push_back(tmpLight);

            return tmpLight;

            break;
        }
        case LightType::SPOT_LIGHT:
        {
            SpotLight* tmpLight = new SpotLight();
            tmpLight->lightGO = App->scene->CreateGameObject("Spot Light", App->scene->mRootNode);

            CLight* tmpComponentLight = new CLight(tmpLight->lightGO, tmpLight);
            tmpLight->lightGO->AddComponent(tmpComponentLight);

            lights.push_back(tmpLight);

            return tmpLight;
         
            break;
        }
        case LightType::AREA_LIGHT:
        {
            AreaLight* tmpLight = new AreaLight();
            tmpLight->lightGO = App->scene->CreateGameObject("Area Light", App->scene->mRootNode);

            CLight* tmpComponentLight = new CLight(tmpLight->lightGO, tmpLight);
            tmpLight->lightGO->AddComponent(tmpComponentLight);

            lights.push_back(tmpLight);

            return tmpLight;

            break;
        }

    }

}
