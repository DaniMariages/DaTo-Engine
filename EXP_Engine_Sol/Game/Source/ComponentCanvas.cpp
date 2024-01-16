#include "ComponentCanvas.h"
#include "GameObject.h"
#include "GameObject.h"
#include "../External/ImGui/imgui.h"
#include "../External/Glew/include/glew.h"

#include "ComponentTransform.h"

ComponentCanvas::ComponentCanvas(GameObject* gameObject, float width, float heigth, unsigned int PosX, unsigned int PosY) : Component(gameObject)
{
	parent = gameObject;

	widthPanel = width;
	heigthPanel = heigth;

	posX = PosX;
	posY = PosY;

	gameObject->transform->SetPosition(float3((float)posX, (float)posY, 0));
	gameObject->transform->SetScale(float3((float)widthPanel, (float)heigthPanel, 1));
	gameObject->transform->SetRotation(Quat(0, 0, 0, 0));

	gameObject->transform->UpdateTransform();

	comp_transform = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);
	comp_transform->SetScale(float3(widthPanel, heigthPanel, 1));
	comp_transform->SetRotation(Quat(0, 0, 0, 1));

	type = typeComponent::Canvas;

}

ComponentCanvas::ComponentCanvas(GameObject* _parent) : Component(_parent)
{
	parent = _parent;

	widthPanel = ExternalApp->editor->GetWindowSize().x;
	heigthPanel = ExternalApp->editor->GetWindowSize().y;

	posX = 30;
	posY = 30;

	comp_transform = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);
	comp_transform->SetScale(float3(widthPanel, heigthPanel, 1));
	comp_transform->SetRotation(Quat(0, 0, 0, 1));

	type = typeComponent::Canvas;
}

ComponentCanvas::~ComponentCanvas()
{
	parent = nullptr;

	comp_transform = nullptr;
}

void ComponentCanvas::Enable()
{

}

void ComponentCanvas::Update()
{
	comp_transform->SetScale(float3(1, 1, 1));
}

void ComponentCanvas::Disable()
{

}

void ComponentCanvas::Draw()
{
	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float3 pos = comp_transform->GetPosition();
	float3 scale = comp_transform->GetScale();

	widthPanel = scale.x;
	heigthPanel = scale.y;

	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos.x, (pos.y + heigthPanel), pos.z);
	glVertex3f((pos.x + widthPanel), (pos.y + heigthPanel), pos.z);
	glVertex3f((pos.x + widthPanel), pos.y, pos.z);

	glEnd();
}

void ComponentCanvas::ShowInfo()
{
	if (ImGui::TreeNode("Canvas"))
	{
		ImGui::TreePop();
	}
}