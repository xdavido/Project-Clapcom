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

G_UI::G_UI(UI_TYPE t, GameObject* pParent, int x, int y) : GameObject("", pParent)
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
					//if (TimeManager::IsOnPlay()) // idk how to do this
					//{
					//	static_cast<C_UI*>(mComponents[i])->StateLogic();
					//}

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

bool G_UI::AddUIComponent(UI_TYPE type, int x, int y, GameObject* parent)
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
		int w = 300;
		int h = 50;

		// Unity-like
		AddUIComponent(UI_TYPE::IMAGE, x, y);
		GetComponentUI(UI_TYPE::IMAGE)->width = w;
		GetComponentUI(UI_TYPE::IMAGE)->height = h;

		G_UI* aux = External->scene->CreateGUI(UI_TYPE::TEXT, this, 20, h / 3);
		aux->GetComponentUI(UI_TYPE::TEXT)->width = w;
		aux->GetComponentUI(UI_TYPE::TEXT)->height = h;
		aux->ReParent(this);
		aux->canvas = static_cast<G_UI*>(mParent)->canvas;

		UI_Button* comp = new UI_Button(this, x, y);
		mComponents.push_back(comp);

		name = "Button";

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

		comp->image = static_cast<UI_Image*>(GetComponentUI(UI_TYPE::IMAGE));
		comp->displayText = static_cast<UI_Text*>(aux->GetComponentUI(UI_TYPE::TEXT));

		comp = nullptr;
		aux = nullptr;
	}
	break;
	case UI_TYPE::INPUTBOX:
	{
		int w = 300;
		int h = 50;

		// Unity-like
		AddUIComponent(UI_TYPE::IMAGE, x, y);
		GetComponentUI(UI_TYPE::IMAGE)->width = w;
		GetComponentUI(UI_TYPE::IMAGE)->height = h;

		G_UI* aux = External->scene->CreateGUI(UI_TYPE::TEXT, this, 20, h / 3);
		aux->GetComponentUI(UI_TYPE::TEXT)->width = w;
		aux->GetComponentUI(UI_TYPE::TEXT)->height = h;
		aux->ReParent(this);

		//new G_UI(UI_TYPE::IMAGE, this);

		//AddUIComponent(UI_TYPE::IMAGE);
		//AddUIComponent(UI_TYPE::TEXT);
		UI_InputBox* comp = new UI_InputBox(this, x, y);
		mComponents.push_back(comp);

		name = "Input Box";

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
		comp->displayText = static_cast<UI_Text*>(aux->GetComponentUI(UI_TYPE::TEXT));
		comp->image = static_cast<UI_Image*>(GetComponentUI(UI_TYPE::IMAGE));
		aux->canvas = canvas;

		comp = nullptr;
		aux = nullptr;
	}
	break;
	case UI_TYPE::CHECKBOX:
	{
		int w = 170;
		int h = 20;

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
		aux->ReParent(this);
		aux->canvas = static_cast<G_UI*>(mParent)->canvas;

		// Checkmark
		G_UI* aux2 = External->scene->CreateGUI(UI_TYPE::IMAGE, this);
		aux2->name = "Checkmark";
		aux2->ReParent(aux);
		aux2->canvas = static_cast<G_UI*>(mParent)->canvas;

		float widthCheck = aux->GetComponentUI(UI_TYPE::IMAGE)->width;
		float heightCheck = aux->GetComponentUI(UI_TYPE::IMAGE)->height;

		// Label
		G_UI* aux3 = External->scene->CreateGUI(UI_TYPE::TEXT, this, widthCheck * 1.2, heightCheck * 0.4);
		aux3->GetComponentUI(UI_TYPE::TEXT)->width = w;
		aux3->GetComponentUI(UI_TYPE::TEXT)->height = h;
		aux3->ReParent(this);
		aux3->canvas = static_cast<G_UI*>(mParent)->canvas;

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
		int w = 300;
		int h = 50;

		// Background
		G_UI* aux = External->scene->CreateGUI(UI_TYPE::IMAGE, this, 20, h / 3);
		aux->GetComponentUI(UI_TYPE::IMAGE)->width = w;
		aux->GetComponentUI(UI_TYPE::IMAGE)->height = h;
		aux->ReParent(this);
		aux->canvas = static_cast<G_UI*>(mParent)->canvas;
		aux->name = "Background";

		// Fill
		G_UI* aux1 = External->scene->CreateGUI(UI_TYPE::IMAGE, this, 20, h / 3);
		aux1->GetComponentUI(UI_TYPE::IMAGE)->width = w;
		aux1->GetComponentUI(UI_TYPE::IMAGE)->height = h;
		aux1->ReParent(this);
		aux1->canvas = static_cast<G_UI*>(mParent)->canvas;
		aux1->name = "Fill";

		// Handle
		G_UI* aux2 = External->scene->CreateGUI(UI_TYPE::IMAGE, this, -50, -18.3);
		aux2->GetComponentUI(UI_TYPE::IMAGE)->width = 70;
		aux2->GetComponentUI(UI_TYPE::IMAGE)->height = 70;
		aux2->ReParent(this);
		aux2->canvas = static_cast<G_UI*>(mParent)->canvas;
		aux2->name = "Handle";

		/*UI_Slider* comp = new UI_Slider(this, x, y, GetChildByUID(aux1->UID), GetChildByUID(aux2->UID));
		mComponents.push_back(comp);

		name = "Slider";

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

		comp = nullptr;*/
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