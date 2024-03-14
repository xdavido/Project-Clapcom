#include "UI_Transform.h"

#include "UI_Canvas.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include "External/mmgr/mmgr.h"

UI_Transform::UI_Transform(C_UI* owner) : Component(owner->mOwner, ComponentType::UI_TRAMSFORM)
{
	boundsEditorReference = owner->boundsEditor;
	boundsGameReference = owner->boundsGame;

}

UI_Transform::~UI_Transform()
{
	RELEASE(boundsEditorReference);
	RELEASE(boundsGameReference);
}

update_status UI_Transform::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}



