#include "G_UI.h"
#include "UI_Canvas.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Button.h"
#include "UI_InputBox.h"
#include "UI_CheckBox.h"
#include "UI_Slider.h"

#include "External/ImGui/imgui.h"

#include "ModuleScene.h"

#include "External/mmgr/mmgr.h"

G_UI::G_UI(UI_TYPE t, GameObject* pParent, float x, float y) : GameObject("", pParent)
{
	//RemoveComponent(transform);//TODO: fer amb altre transform
	canvas = nullptr;

	mParent->AddChild(this);

	AddUIComponent(t, x, y, pParent);
}

G_UI::~G_UI()
{
	canvas = nullptr;
	//RELEASE(canvas);
}

update_status G_UI::Update(float dt)
{
	if (External->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && !ImGui::GetIO().WantTextInput && !External->input->GetInputActive())
	{
		LOG("Break!");
	}

	update_status ret = UPDATE_CONTINUE;

	if (active)
	{
		if (!mChildren.empty())
		{
			for (auto i = 0; i < mChildren.size(); i++)
			{
				mChildren[i]->Update(dt);
			}
		}

		CTransform* transformComponent = mParent->mTransform;
		if (transformComponent != nullptr)
		{
			if (transformComponent->dirty_)
			{
				transformComponent->UpdateTransformsChilds();
				//canvas->UpdateBoundingBoxes(); Not implemented yet
			}
		}

		if (mComponents.size() > 1)
		{
			for (int i = 0; i < mComponents.size(); i++)
			{
				if (mComponents[i]->ctype == ComponentType::UI)
				{
					if (true) 
					{
						static_cast<C_UI*>(mComponents[i])->StateLogic();
					}

					static_cast<C_UI*>(mComponents[i])->Update(dt);

					static_cast<C_UI*>(mComponents[i])->UpdateUITransform();

					if (static_cast<C_UI*>(mComponents[i])->isDragging)
					{
						static_cast<C_UI*>(mComponents[i])->Drag(dt);
					}
					if (static_cast<C_UI*>(mComponents[i])->fade)
					{
						static_cast<C_UI*>(mComponents[i])->FadeUI(dt);
					}
				}
			}

			if (canvas != nullptr)
			{
				//canvas->UpdateUITransform(); Not implemented yet
			}
		}
	}

	return ret;
}

C_UI* G_UI::GetComponentUI(UI_TYPE type)
{
	C_UI* comp = nullptr;

	for (auto i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->ctype == ComponentType::UI && static_cast<C_UI*>(mComponents[i])->UI_type == type)
		{
			return comp = static_cast<C_UI*>(mComponents[i]);
		}
	}

	return nullptr;
}

std::vector<C_UI*> G_UI::GetComponentsUI_ByType(UI_TYPE type)
{
	std::vector<C_UI*> vec = {};
	for (auto i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->ctype == ComponentType::UI && static_cast<C_UI*>(mComponents[i])->UI_type == type)
		{
			vec.push_back((C_UI*)mComponents[i]);
		}
	}
	return vec;
}

bool G_UI::AddUIComponent(UI_TYPE type, float x, float y, GameObject* parent)
{
	bool ret = true;

	switch (type)
	{
	case UI_TYPE::CANVAS:
	{
		if (canvas == nullptr)
		{
			UI_Canvas* comp = new UI_Canvas(this);
			mComponents.push_back(comp);

			canvas = comp;
			External->scene->SetCanvas(this);

			comp = nullptr;
		}

		name = "Canvas";
	}
	break;
	case UI_TYPE::IMAGE:
	{
		UI_Image* comp = new UI_Image(this, x, y);
		mComponents.push_back(comp);

		name = "Image";

		if (External->scene->GetCanvas() == nullptr)
		{
			External->scene->SetCanvas(External->scene->CreateGUI(UI_TYPE::CANVAS));
		}

		if (parent == External->scene->mRootNode)
		{
			ReParent(External->scene->GetCanvas());
		}
		else
		{
			ReParent(parent);
		}
		canvas = static_cast<G_UI*>(mParent)->canvas;

		comp = nullptr;
	}
	break;
	case UI_TYPE::TEXT:
	{
		UI_Text* comp = new UI_Text(this, x, y);
		mComponents.push_back(comp);

		name = "Text";

		if (External->scene->GetCanvas() == nullptr)
		{
			External->scene->SetCanvas(External->scene->CreateGUI(UI_TYPE::CANVAS));
		}

		if (parent == External->scene->mRootNode)
		{
			ReParent(External->scene->GetCanvas());
		}
		else
		{
			ReParent(parent);
		}
		canvas = static_cast<G_UI*>(mParent)->canvas;

		comp = nullptr;
	}
	break;
	case UI_TYPE::BUTTON:
	{
		float w = 300;
		float h = 50;

		UI_Button* comp = new UI_Button(this, x, y);
		mComponents.push_back(comp);

		if (External->scene->GetCanvas() == nullptr)
		{
			External->scene->SetCanvas(External->scene->CreateGUI(UI_TYPE::CANVAS));
		}

		if (parent == External->scene->mRootNode)
		{
			ReParent(External->scene->GetCanvas());
		}
		else
		{
			ReParent(parent);
		}
		canvas = static_cast<G_UI*>(mParent)->canvas;

		// Unity-like
		AddUIComponent(UI_TYPE::IMAGE, x, y);
		GetComponentUI(UI_TYPE::IMAGE)->width = w;
		GetComponentUI(UI_TYPE::IMAGE)->height = h;

		G_UI* aux = External->scene->CreateGUI(UI_TYPE::TEXT, this, 20, h / 3);
		aux->GetComponentUI(UI_TYPE::TEXT)->width = w;
		aux->GetComponentUI(UI_TYPE::TEXT)->height = h;
		aux->vReferences.push_back(comp);

		comp->image = static_cast<UI_Image*>(GetComponentUI(UI_TYPE::IMAGE));
		comp->displayText = aux;

		name = "Button";

		comp = nullptr;
		aux = nullptr;
	}
	break;
	case UI_TYPE::INPUTBOX:
	{
		float w = 300;
		float h = 50;

		UI_InputBox* comp = new UI_InputBox(this, x, y);
		mComponents.push_back(comp);

		if (External->scene->GetCanvas() == nullptr)
		{
			External->scene->SetCanvas(External->scene->CreateGUI(UI_TYPE::CANVAS));
		}

		if (parent == External->scene->mRootNode)
		{
			ReParent(External->scene->GetCanvas());
		}
		else
		{
			ReParent(parent);
		}

		canvas = static_cast<G_UI*>(mParent)->canvas;

		// Unity-like
		AddUIComponent(UI_TYPE::IMAGE, x, y);
		GetComponentUI(UI_TYPE::IMAGE)->width = w;
		GetComponentUI(UI_TYPE::IMAGE)->height = h;

		G_UI* aux = External->scene->CreateGUI(UI_TYPE::TEXT, this, 20, h / 3);
		aux->GetComponentUI(UI_TYPE::TEXT)->width = w;
		aux->GetComponentUI(UI_TYPE::TEXT)->height = h;

		//new G_UI(UI_TYPE::IMAGE, this);

		//AddUIComponent(UI_TYPE::IMAGE);
		//AddUIComponent(UI_TYPE::TEXT);
		
		comp->displayText = static_cast<UI_Text*>(aux->GetComponentUI(UI_TYPE::TEXT));
		comp->image = static_cast<UI_Image*>(GetComponentUI(UI_TYPE::IMAGE));

		name = "Input Box";

		comp = nullptr;
		aux = nullptr;
	}
	break;
	case UI_TYPE::CHECKBOX:
	{
		float w = 170;
		float h = 20;

		if (External->scene->GetCanvas() == nullptr)
		{
			External->scene->SetCanvas(External->scene->CreateGUI(UI_TYPE::CANVAS));
		}

		if (parent == External->scene->mRootNode)
		{
			ReParent(External->scene->GetCanvas());
		}
		else
		{
			ReParent(parent);
		}
		canvas = static_cast<G_UI*>(mParent)->canvas;

		// TODO: Maybe put all this references to the other components in the constructor
		// Unity-like
		// Toggle background
		G_UI* aux = External->scene->CreateGUI(UI_TYPE::IMAGE, this);
		aux->name = "Background";

		// Checkmark
		G_UI* aux2 = External->scene->CreateGUI(UI_TYPE::IMAGE, this);
		aux2->name = "Checkmark";

		float widthCheck = aux->GetComponentUI(UI_TYPE::IMAGE)->width;
		float heightCheck = aux->GetComponentUI(UI_TYPE::IMAGE)->height;

		// Label
		G_UI* aux3 = External->scene->CreateGUI(UI_TYPE::TEXT, this, widthCheck * 1.2, heightCheck * 0.4);
		aux3->GetComponentUI(UI_TYPE::TEXT)->width = w;
		aux3->GetComponentUI(UI_TYPE::TEXT)->height = h;

		UI_Checkbox* comp = new UI_Checkbox(this, x, y, widthCheck, heightCheck);
		mComponents.push_back(comp);

		name = "Checkbox";

		comp->bgImg = static_cast<UI_Image*>(aux->GetComponentUI(UI_TYPE::IMAGE));
		comp->cmImg = static_cast<UI_Image*>(aux2->GetComponentUI(UI_TYPE::IMAGE));
		comp->cmImg->active = false;
		comp->displayText = static_cast<UI_Text*>(aux3->GetComponentUI(UI_TYPE::TEXT));

		comp = nullptr;
		aux = nullptr;
		aux2 = nullptr;
		aux3 = nullptr;
	}
	break;
	case UI_TYPE::SLIDER:
	{
		float w = 300;
		float h = 50;

		if (External->scene->GetCanvas() == nullptr)
		{
			External->scene->SetCanvas(External->scene->CreateGUI(UI_TYPE::CANVAS));
		}

		if (parent == External->scene->mRootNode)
		{
			ReParent(External->scene->GetCanvas());
		}
		else
		{
			ReParent(parent);
		}
		canvas = static_cast<G_UI*>(mParent)->canvas;

		// Background
		G_UI* aux = External->scene->CreateGUI(UI_TYPE::IMAGE, this, 20, h / 3);
		aux->GetComponentUI(UI_TYPE::IMAGE)->width = w;
		aux->GetComponentUI(UI_TYPE::IMAGE)->height = h;
		aux->name = "Background";

		// Fill
		G_UI* aux1 = External->scene->CreateGUI(UI_TYPE::IMAGE, this, 20, h / 3);
		aux1->GetComponentUI(UI_TYPE::IMAGE)->width = 0;
		aux1->GetComponentUI(UI_TYPE::IMAGE)->height = 50;
		aux1->name = "Fill";

		// Handle
		G_UI* aux2 = External->scene->CreateGUI(UI_TYPE::IMAGE, this, -50, -18.3);
		aux2->GetComponentUI(UI_TYPE::IMAGE)->width = 70;
		aux2->GetComponentUI(UI_TYPE::IMAGE)->height = 70;
		aux2->name = "Handle";

		aux2->GetComponentUI(UI_TYPE::IMAGE)->dragLimits = { x, y, w, h };


		UI_Slider* comp = new UI_Slider(this, x, y, static_cast<G_UI*>(GetChildByUID(aux1->UID)), static_cast<G_UI*>(GetChildByUID(aux2->UID)));
		mComponents.push_back(comp);

		name = "Slider";

		comp = nullptr;
		aux = nullptr;
		aux1 = nullptr;
		aux2 = nullptr;
	}
	case UI_TYPE::NONE:
		break;
	default:
		break;
	}

	return ret;
}