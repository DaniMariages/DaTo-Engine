#include "ComponentUI.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentText.h"
#include "Application.h"
#include "Module.h"
#include "ComponentButton.h"
#include "ComponentImage.h"
#include "ComponentCanvas.h"


ComponentUI::ComponentUI(UI_Type uiType, GameObject* gameObject, uint width, uint heigth, uint PosX, uint PosY, const char* imagePath) : 
	Component(gameObject)
{
	//Text Component
	textComp = nullptr;
	InputTextComp = nullptr;
	textCH = "";
	font = ExternalApp->fonts->actualFont;

	actualText = "";
	newText = "";

	//Button
	actualButtonAction = 1;

	type = typeComponent::UI;

	//Checker
	CheckSelected = false;
	//active = new ComponentTexture();
	//Importer::ImporterTexture::Load(active, "Assets/Textures/Ckeck-checkedbox.png");

	//disabled = new ComponentTexture();
	//Importer::ImporterTexture::Load(disabled, "Assets/Textures/Unchecked-checkbox.png");

	//Input Text
	IsTextEditing = false;

	isDragabble = false;
	isChildOfText = false;
	isSelected = false;
	isBeeingClicked = false;

	ui_Type = uiType;
	actualMouseState = MouseState::IDLE_UI;

	PlaneInScene = new UIPlane;
	PlaneInGame = new UIPlane;

	parent = gameObject;

	widthPanel = width;
	heigthPanel = heigth;

	positionX = PosX;
	positionY = PosY;

	texture = nullptr;

	if (imagePath != nullptr)
	{
		texture = new Texture();
		ExternalApp->importer->LoadTextureUI(texture, imagePath);

		texture = ExternalApp->importer->LoadTexture(imagePath);

		if (texture != nullptr)
		{
			PlaneInGame->textureID = PlaneInScene->textureID = texture->textID;
		}
	}

	if (gameObject->Parent != nullptr)
	{
		gameObject->transform->SetPosition(float3((float)positionX, (float)positionY, 0));
		gameObject->transform->SetScale(float3((float)widthPanel, (float)heigthPanel, 1));
		gameObject->transform->SetRotation(Quat(0, 0, 0, 0));

		gameObject->transform->UpdateTransform();

		CreatePanel(PlaneInScene->vertex, gameObject->transform->GetPosition(), width, heigth);
		CreatePanel(PlaneInGame->vertex, gameObject->transform->GetPosition(), width, heigth);
	}

	PlaneInScene->uv[0] = float2(0, 1);
	PlaneInScene->uv[1] = float2(1, 1);
	PlaneInScene->uv[3] = float2(1, 0);
	PlaneInScene->uv[2] = float2(0, 0);

	PlaneInGame->uv[0] = float2(0, 1);
	PlaneInGame->uv[1] = float2(1, 1);
	PlaneInGame->uv[3] = float2(1, 0);
	PlaneInGame->uv[2] = float2(0, 0);

	GenerateBuffers(PlaneInScene->buffer, PlaneInScene->vertex, PlaneInScene->uv);
	GenerateBuffers(PlaneInGame->buffer, PlaneInGame->vertex, PlaneInGame->uv);
}

ComponentUI::ComponentUI(GameObject* _parent) : Component(_parent)
{
	//Text Component
	textComp = nullptr;
	InputTextComp = nullptr;
	textCH = "";
	font = ExternalApp->fonts->actualFont;

	actualText = "";
	newText = "";

	//Button
	actualButtonAction = 1;

	//Checker
	CheckSelected = false;
	//active = new ComponentTexture();
	//Importer::ImporterTexture::Load(active, "Assets/Textures/Ckeck-checkedbox.png");

	//disabled = new ComponentTexture();
	//Importer::ImporterTexture::Load(disabled, "Assets/Textures/Unchecked-checkbox.png");

	//Input Text
	IsTextEditing = false;

	isDragabble = false;
	isChildOfText = false;
	isSelected = false;
	isBeeingClicked = false;

	actualMouseState = MouseState::IDLE_UI;

	PlaneInScene = new UIPlane;
	PlaneInGame = new UIPlane;

	parent = _parent;

	widthPanel = 1;
	heigthPanel = 1;

	positionX = 0;
	positionY = 0;

	texture = nullptr;

	//if (imagePath != nullptr)
	//{
	//	texture = new Texture();
	//	ExternalApp->importer->LoadTextureUI(texture, imagePath);

	//	if (texture != nullptr)
	//	{
	//		PlaneInGame->textureID = PlaneInScene->textureID = texture->textID;
	//	}
	//}

	if (_parent != nullptr)
	{
		_parent->transform->SetPosition(float3((float)positionX, (float)positionY, 0));
		_parent->transform->SetScale(float3((float)widthPanel, (float)heigthPanel, 1));
		_parent->transform->SetRotation(Quat(0, 0, 0, 0));

		_parent->transform->UpdateTransform();

		CreatePanel(PlaneInScene->vertex, _parent->transform->GetPosition(), 150, 150);
		CreatePanel(PlaneInGame->vertex, _parent->transform->GetPosition(), 150, 150);
	}

	PlaneInScene->uv[0] = float2(0, 1);
	PlaneInScene->uv[1] = float2(1, 1);
	PlaneInScene->uv[3] = float2(1, 0);
	PlaneInScene->uv[2] = float2(0, 0);

	PlaneInGame->uv[0] = float2(0, 1);
	PlaneInGame->uv[1] = float2(1, 1);
	PlaneInGame->uv[3] = float2(1, 0);
	PlaneInGame->uv[2] = float2(0, 0);

	GenerateBuffers(PlaneInScene->buffer, PlaneInScene->vertex, PlaneInScene->uv);
	GenerateBuffers(PlaneInGame->buffer, PlaneInGame->vertex, PlaneInGame->uv);
}

ComponentUI::~ComponentUI()
{
	delete texture;
	texture = nullptr;

	delete InputTextComp;
	InputTextComp = nullptr;

	font = nullptr;

	active = nullptr;

	disabled = nullptr;

	PlaneInScene = nullptr;
	PlaneInGame = nullptr;

	parent = nullptr;
}

void ComponentUI::SetTexture(Texture* T_Texture)
{

}

Texture* ComponentUI::GetTexture()
{
	return texture;
}

void ComponentUI::Enable()
{

}

void ComponentUI::Update()
{
	MousePicker();

	if (ui_Type == INPUT_TEXT)
	{
		ComponentTexture* compTex = (ComponentTexture*)parent->GetComponent(typeComponent::Material);
		if (actualText == "")
		{
			compTex->SetTexture(texture);
		}

		InputText* inputText = (InputText*)this;
		//inputText->Update(this);
	}

	/*if (->editor->isRunning)
	{
		switch (actualMouseState)
		{
		case IDLE_UI:
			if (ui_Type == BUTTON) {
				ButtonUI* button = (ButtonUI*)this;
				button->OnIdle(this);
			}
			break;
			if (ui_Type == CHECKER) {
				CheckerUI* checker = (CheckerUI*)this;
				checker->OnIdle(this);
			}
			break;
		case HOVER_UI:
			if (ui_Type == BUTTON) {
				ButtonUI* button = (ButtonUI*)this;
				button->OnHover(this);
			}
			if (ui_Type == CHECKER) {
				CheckerUI* checker = (CheckerUI*)this;
				checker->OnHover(this);
			}
			break;
		case CLICK_UI:
			if (ui_Type == INPUT_TEXT)
			{
				InputText* inputText = (InputText*)this;
				inputText->OnClick(this);
			}
			if (ui_Type == BUTTON)
			{
				ButtonUI* button = (ButtonUI*)this;
				button->OnClick(&actualButtonAction);
			}
			if (ui_Type == CHECKER)
			{
				CheckerUI* checker = (CheckerUI*)this;
				checker->OnClick(this);
			}
			break;
		case CLICKED_UI:
			if (app->scene->draggable)
			{
				MoveComponent();
			}
			break;
		case CLICKED_RELEASED_UI:
			this->isBeeingClicked = false;

			if (ui_Type == BUTTON)
			{
				ButtonUI* button = (ButtonUI*)this;
				button->OnIdle(this);
				isDragabble = false;
			}
			if (ui_Type == CHECKER) {
				CheckerUI* checker = (CheckerUI*)this;
				checker->OnIdle(this);
				isDragabble = false;
			}
			break;
		default:
			break;
		}
	}*/
}

void ComponentUI::Disable()
{

}

ComponentUI* ComponentUI::CreateGameObjectUI(GameObject* parent, UI_Type type, uint width, uint heigth, uint posX, uint posY, const char* imagePath, const char* text, int buttonFuntion, const char* imagePathDisabled, uint OrinigalPosX, uint OrinigalPosY, uint OrinigalWidth, uint Orinigalheight)
{
	ComponentUI* comp_UI = nullptr;

	switch (type)
	{
	case UI_Type::BUTTON:
	{
		GameObject* Button = ExternalApp->scene->CreateGameObject("Button", parent);

		ComponentUI* compUI = new ComponentUI(type, Button, width, heigth, posX, posY, imagePath);
		comp_UI = compUI;
		Button->AddComponent(compUI);

		comp_UI->AsRootPositionX = OrinigalPosX; 
		comp_UI->AsRootPositionY = OrinigalPosY; 
		comp_UI->AsRootWidthPanel = OrinigalWidth; 
		comp_UI->AsRootHeigthPanel = Orinigalheight;

		ComponentButton but_UI = ComponentButton(type, Button, width, heigth, posX, posY, imagePath);
		but_UI.actualFunction = (functions)buttonFuntion;
		comp_UI->parent = Button;
		comp_UI->actualButtonAction = buttonFuntion;
		comp_UI->positionX = but_UI.positionX;
		comp_UI->positionY = but_UI.positionY;

		ComponentTexture* compTex = new ComponentTexture(Button);
		Button->AddComponent(compTex);
		compTex->texColor.r = 255;
		compTex->texColor.g = 255;
		compTex->texColor.b = 255;
		compTex->texColor.a = 255;

		if (comp_UI->texture != nullptr)
		{
			compTex->SetTexture(comp_UI->texture);
		}
	}
	break;
	case UI_Type::TEXT:
	{
		GameObject* Text = ExternalApp->scene->CreateGameObject("Text", parent);

		ComponentUI* compUI = new ComponentUI(type, Text, width, heigth, posX, posY, imagePath);
		comp_UI = compUI;

		ComponentText* compText = new ComponentText(Text);
		Text->AddComponent(compText);

		comp_UI->AsRootPositionX = OrinigalPosX; 
		comp_UI->AsRootPositionY = OrinigalPosY; 
		comp_UI->AsRootWidthPanel = OrinigalWidth; 
		comp_UI->AsRootHeigthPanel = Orinigalheight;
		comp_UI->textComp = compText;
		comp_UI->parent = Text;
		comp_UI->textCH = compText->text;
		comp_UI->font = compText->font;
		comp_UI->actualText = compText->actualText;
		comp_UI->newText = compText->newText;
		comp_UI->actualFonts = compText->actualFonts;
		comp_UI->positionX = compText->positionX;
		comp_UI->positionY = compText->positionY;

		ComponentTexture* compTex = new ComponentTexture(Text);
		Text->AddComponent(compTex);
		compTex->texColor.r = 255;
		compTex->texColor.g = 255;
		compTex->texColor.b = 255;
		compTex->texColor.a = 255;

		if (comp_UI->texture != nullptr)
		{
			compTex->SetTexture(comp_UI->texture);
		}
	}
	break;
	case UI_Type::INPUT_TEXT:
	{
		/*GameObject* Text = ExternalApp->scene->CreateGameObject("Input Text", parent);

		ComponentUI* compUI = new ComponentUI(type, parent, width, heigth, posX, posY, imagePath);
		

		comp_UI = (ComponentUI*)Text->AddComponent(typeComponent::UI, type, width, heigth, posX, posY, imagePath);
		InputText text_UI = InputText(type, Text, width, heigth, posX, posY, "");
		comp_UI->AsRootPositionX = OrinigalPosX; comp_UI->AsRootPositionY = OrinigalPosY; comp_UI->AsRootWidthPanel = OrinigalWidth; comp_UI->AsRootHeigthPanel = Orinigalheight;
		comp_UI->InputTextComp = &text_UI;
		comp_UI->parent = Text;
		comp_UI->textCH = text_UI.text;
		comp_UI->font = text_UI.font;
		comp_UI->actualText = text_UI.actualText;
		comp_UI->newText = text_UI.newText;
		comp_UI->actualFonts = text_UI.actualFonts;
		comp_UI->positionX = text_UI.positionX;
		comp_UI->positionY = text_UI.positionY;

		ComponentMaterial* mat = (ComponentMaterial*)(Text->AddComponent(ComponentType::MATERIAL));
		mat->colorTexture.r = 255;
		mat->colorTexture.g = 255;
		mat->colorTexture.b = 255;
		mat->colorTexture.a = 255;

		if (comp_UI->texture != nullptr)
		{
			mat->texture = comp_UI->texture;
		}*/
	}
	break;
	case UI_Type::IMAGE:
	{
		GameObject* Image = ExternalApp->scene->CreateGameObject("Image", parent);

		ComponentUI* compUI = new ComponentUI(type, Image, width, heigth, posX, posY, imagePath);
		comp_UI = compUI;

		ComponentImage* compImage = new ComponentImage(type, Image, width, heigth, posX, posY, imagePath);
		Image->AddComponent(compImage);

		comp_UI->AsRootPositionX = OrinigalPosX;
		comp_UI->AsRootPositionY = OrinigalPosY; 
		comp_UI->AsRootWidthPanel = OrinigalWidth; 
		comp_UI->AsRootHeigthPanel = Orinigalheight;
		comp_UI->parent = Image;

		ComponentTexture* compTex = new ComponentTexture(Image);
		Image->AddComponent(compTex);

		compTex->texColor.r = 255;
		compTex->texColor.g = 255;
		compTex->texColor.b = 255;
		compTex->texColor.a = 255;

		if (comp_UI->texture != nullptr)
		{
			compTex->SetTexture(comp_UI->texture);
		}
	}
	break;
	case UI_Type::CHECKER:
	{
		//GameObject* Checker = new GameObject("Checker", gm);
		//ComponentTransform* transform = dynamic_cast<ComponentTransform*>(Checker->GetComponentGameObject(ComponentType::TRANSFORM));
		//comp_UI = dynamic_cast<ComponentUI*>(Checker->AddComponent(ComponentType::UI, type, width, heigth, posX, posY, imagePathDisabled));
		//CheckerUI check_UI = CheckerUI(type, Checker, width, heigth, posX, posY, imagePath, imagePathDisabled);
		//comp_UI->AsRootPositionX = OrinigalPosX; comp_UI->AsRootPositionY = OrinigalPosY; comp_UI->AsRootWidthPanel = OrinigalWidth; comp_UI->AsRootHeigthPanel = Orinigalheight;
		//comp_UI->actualChecker = (_functions)buttonFuntion;
		//comp_UI->positionX = check_UI.positionX;
		//comp_UI->positionY = check_UI.positionY;
		//comp_UI->widthPanel = check_UI.widthPanel;
		//comp_UI->heigthPanel = check_UI.heigthPanel;
		////comp_UI->active = check_UI.textureActive;
		////comp_UI->disabled = check_UI.textureDisabled;

		//ComponentMaterial* mat = (ComponentMaterial*)(Checker->AddComponent(ComponentType::MATERIAL));
		//mat->colorTexture.r = 255;
		//mat->colorTexture.g = 255;
		//mat->colorTexture.b = 255;
		//mat->colorTexture.a = 255;

		//if (comp_UI->texture != nullptr)
		//{
		//	mat->texture = comp_UI->texture;
		//}
	}
	break;
	case UI_Type::CANV:
	{
		ExternalApp->scene->canvas = ExternalApp->scene->CreateGameObject("Canvas", parent);
		ExternalApp->scene->canvas->transform->SetPosition(float3((float)posX, (float)posY, 0));

		ComponentCanvas* canv_UI = new ComponentCanvas(ExternalApp->scene->canvas, ExternalApp->editor->GetWindowSize().x, ExternalApp->editor->GetWindowSize().y, 150, 150);
		ExternalApp->scene->canvas->AddComponent(canv_UI);
	}
	break;
	default:
		break;
	}

	if (ExternalApp->scene->canvas != nullptr)
	{
		if (ExternalApp->scene->canvas->children.size() > 1)
		{
			GameObject* first = ExternalApp->scene->canvas->children[0];
			ExternalApp->scene->canvas->children[0] = ExternalApp->scene->canvas->children[ExternalApp->scene->canvas->children.size() - 1];
			ExternalApp->scene->canvas->children[ExternalApp->scene->canvas->children.size() - 1] = first;
		}
	}

	return comp_UI;
}

void ComponentUI::GenerateBuffers(uint buffer[], float3 vertex[], float2 uv[])
{
	uint* index = new uint[6];

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;

	//Cube Vertex
	glGenBuffers(1, &(GLuint)buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &(GLuint)buffer[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);

	//UVs definition
	glGenBuffers(1, &(GLuint)buffer[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * 4, uv, GL_STATIC_DRAW);
}

void ComponentUI::RegenerateBuffers(uint buffer[], float3 vertex[]) {

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);
}

void ComponentUI::MousePicker()
{
	ImVec2 mousePosInViewport = ExternalApp->editor->GetMousePosInViewport();
	/*
	mousePosInViewport.x = ExternalApp->input->GetMouseX() - ImGui::GetCursorScreenPos().x;
	mousePosInViewport.y = ExternalApp->input->GetMouseY() - ImGui::GetCursorScreenPos().y;*/

	float2 originPoint = float2(mousePosInViewport.x, mousePosInViewport.y);

	/*originPoint.x = (originPoint.x) * 2;
	originPoint.y = -(originPoint.y) * 2;*/

	float2 mouse_pos = float2(originPoint.x, originPoint.y);

	switch (actualMouseState)
	{
	case IDLE_UI:
		if (MouseIsInside(mouse_pos))
			actualMouseState = HOVER_UI;
		break;
	case HOVER_UI:
		if (ExternalApp->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			actualMouseState = CLICK_UI;

		if (!MouseIsInside(mouse_pos))
			actualMouseState = CLICKED_RELEASED_UI;
		break;
	case CLICK_UI:
		if (ExternalApp->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			actualMouseState = CLICKED_UI;
		break;
	case CLICKED_UI:
		if (ExternalApp->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && !MouseIsInside(mouse_pos))
			actualMouseState = CLICKED_RELEASED_UI;
		else if (ExternalApp->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && MouseIsInside(mouse_pos))
			actualMouseState = HOVER_UI;
		break;
	case CLICKED_RELEASED_UI:
		actualMouseState = IDLE_UI;
		break;
	}
}

bool ComponentUI::MouseIsInside(float2 mouse)
{
	bool ret = false;

	if (parent != nullptr)
	{
		if (ExternalApp->scene->gameCamera != nullptr && parent->active)
		{
			if (AsRootPositionX >= 0 && AsRootPositionY >= 0 && mouse.x >= 0 && mouse.y >= 0 && mouse.x < ExternalApp->editor->GetWindowSize().x && mouse.y < ExternalApp->editor->GetWindowSize().y)
			{
				if (mouse.x >= AsRootPositionX && mouse.x <= AsRootPositionX + AsRootWidthPanel && mouse.y >= AsRootPositionY && mouse.y <= AsRootPositionY + AsRootHeigthPanel)
				{
					ret = true;
				}
			}
		}

		if (ui_Type == INPUT_TEXT)
		{
			if (AsRootPositionX >= 0 && AsRootPositionY >= 0 && mouse.x >= 0 && mouse.y >= 0 && mouse.x < ExternalApp->editor->GetWindowSize().x && mouse.y < ExternalApp->editor->GetWindowSize().y)
			{
				if (mouse.x <= AsRootPositionX || mouse.x >= AsRootPositionX + AsRootWidthPanel || mouse.y <= AsRootPositionY || mouse.y >= AsRootPositionY + AsRootHeigthPanel)
				{
					IsTextEditing = false;
				}
			}
		}
	}

	return ret;
}

void ComponentUI::CreatePanel(float3 vertex[], float3 transform, uint width, uint heigth)
{
	vertex[0] = float3(transform.x, transform.y + 1, transform.z);
	vertex[1] = float3(transform.x + 1, transform.y + 1, transform.z);
	vertex[3] = float3(transform.x + 1, transform.y, transform.z);
	vertex[2] = float3(transform.x, transform.y, transform.z);
}

void ComponentUI::MoveComponent()
{
	ImVec2 mousePosInViewport;
	mousePosInViewport.x = ExternalApp->input->GetMouseX() - ImGui::GetCursorScreenPos().x;
	mousePosInViewport.y = ExternalApp->input->GetMouseY() - ImGui::GetCursorScreenPos().y;

	float2 originPoint = float2(ExternalApp->editor->GetMousePosInViewport().x, ExternalApp->editor->GetMousePosInViewport().y);

	float2 mouse_pos = float2(originPoint.x, originPoint.y);

	if (mouse_pos.x >= 0 && mouse_pos.y >= 0 && mouse_pos.x < ExternalApp->editor->GetWindowSize().x && mouse_pos.y < ExternalApp->editor->GetWindowSize().y)
	{
		ComponentTransform* transform = dynamic_cast<ComponentTransform*>(parent->GetComponent(typeComponent::Transform));

		float3 gPos;
		float3 gSca;
		Quat gRot;

		transform->GetGlobalTransform().Decompose(gPos, gRot, gSca);

		float distx = mouse_pos.x - gPos.x;
		float disty = mouse_pos.y - gPos.y;

		ComponentUI* comp_UI = this;

		int dx = ExternalApp->input->GetMouseXMotion();
		int dy = ExternalApp->input->GetMouseYMotion();

		transform->SetPosition(float3(
			transform->GetPosition().x + dx * 0.001,
			transform->GetPosition().y + dx * 0.001,
			0
		));

		//transform->mPosition.x += dx * 0.001;
		//transform->mPosition.y += dy * 0.001;
		//transform->mPosition.z = 0;

		if (comp_UI->parent->children.size() > 0)
		{
			comp_UI->AsRootPositionX += dx * 0.00001;
			comp_UI->AsRootPositionY += dy * 0.00001;

			comp_UI->positionX += dx * 0.001;
			comp_UI->positionY += dy * 0.001;
		}

		if (comp_UI->parent->children.size() == 0)
		{
			comp_UI->AsRootPositionX += dx * 1;
			comp_UI->AsRootPositionY += dy * 1;

			comp_UI->positionX += dx * 0.001;
			comp_UI->positionY += dy * 0.001;
		}

		transform->UpdateTransform();

	}
}