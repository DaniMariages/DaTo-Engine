#pragma once

#include "Component.h"
#include "ComponentUI.h"
#include "Color.h"

#include <vector>
#include <functional>

class GameObject;

class ComponentImage : public ComponentUI
{
public:
	GameObject* gmAtached;

	uint widthPanel;
	uint heigthPanel;

	Texture* image_Path;

	bool scaleToCanvas;

public:
	ComponentImage(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~ComponentImage();

	void ShowInfo();
};