#pragma once

#include "Component.h"
#include "Color.h"
#include "ModuleFont.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleImport.h"
#include "ComponentTexture.h"

class GameObject;
class InputText;
enum FONTS;
enum _functions;

struct UIPlane 
{
	float3 vertex[4];
	float2 uv[4];
	uint buffer[3];

	uint textureID;
};

enum UI_Type
{
	BUTTON,
	TEXT,
	IMAGE,
	CHECKER,
	CANV,
	INPUT_TEXT,
	DEF
};

enum MouseState
{
	HOVER_UI,
	CLICK_UI,
	CLICKED_UI,
	IDLE_UI,
	CLICKED_RELEASED_UI
};

class ComponentUI : public Component
{
public:
	UI_Type ui_Type;
	Texture* texture;

	UIPlane* PlaneInScene;
	UIPlane* PlaneInGame;
	GameObject* parent;

	uint widthPanel;
	uint heigthPanel;
	uint AsRootWidthPanel;
	uint AsRootHeigthPanel;

	MouseState actualMouseState;

	bool isDragabble;
	int positionX;
	int positionY;
	int AsRootPositionX;
	int AsRootPositionY;

	float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	//Text Component
	InputText* InputTextComp;
	string textCH;
	Font* font;
	string actualText;
	string newText;
	FONTS actualFonts;
	_functions actualChecker;

	//Button
	int actualButtonAction;

	//Checker
	ComponentTexture* active;
	ComponentTexture* disabled;
	bool CheckSelected;

	//Input Text
	bool IsTextEditing;

	bool isChildOfText;
	bool isSelected;
	bool isBeeingClicked;

public:
	ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	ComponentUI(GameObject* _parent);
	~ComponentUI();

	void SetTexture(Texture* T_Texture = nullptr);
	bool MouseIsInside(float2 mouse);
	Texture* GetTexture();

	void MousePicker();

	void CreatePanel(float3 vertex[], float3 transform, uint width, uint heigth);
	void GenerateBuffers(uint buffer[], float3 vertex[], float2 uv[]);
	void RegenerateBuffers(uint buffer[], float3 vertex[]);

	void Enable() override;
	void Update();
	void Disable() override;

	void MoveComponent();

	ComponentUI* CreateGameObjectUI(GameObject* gm, UI_Type type = UI_Type::DEF, uint width = 20, uint heigth = 20, uint posX = 0, uint posY = 0, const char* imagePath = nullptr, const char* text = "Text", int buttonFuntion = 1, const char* imagePathDisabled = nullptr, uint OrinigalPosX = 0, uint OrinigalPosY = 0, uint OrinigalWidth = 0, uint Orinigalheight = 0);

};