#include "Application.h"
#include "ComponentCanvas.h"
#include "GameObject.h"
#include "ComponentCamera.h"

ComponentCanvas::ComponentCanvas(typeComponent ctype, bool act, GameObject* objs, uint w, uint h) : Component(parent)
{
	width = w;
	height = h;

	ComponentTransform* comp_trans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);
	comp_trans->SetGlobalPosition(float3(0, 0, 200));
	comp_trans->GetGlobalTransform();
}

ComponentCanvas::ComponentCanvas(GameObject* parent) : Component(parent) 
{
	width = 0; 
	height = 0;

	ComponentTransform* compTrans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);
	compTrans->SetGlobalPosition(float3(0, 0, 200));
	compTrans->GetGlobalTransform();
}

void ComponentCanvas::Update()
{
	DebugDraw();
}

void ComponentCanvas::DebugDraw()
{
	ComponentTransform* comp_trans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);

	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float3 pos = comp_trans->GetPosition();

	float3 v1 = float3(pos.x, pos.y, pos.z);
	float3 v2 = float3(pos.x + width, pos.y, pos.z);
	float3 v3 = float3(pos.x, pos.y + height, pos.z);
	float3 v4 = float3(pos.x + width, pos.y + height, pos.z);

	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v4.x, v4.y, v4.z);
	glVertex3f(v3.x, v3.y, v3.z);

	glEnd();
}