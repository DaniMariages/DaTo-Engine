#pragma once
#include "ComponentCamera.h"

#include "../External/ImGui/imgui.h"
#include "../External/ImGui/backends/imgui_impl_sdl2.h"
#include "../External/ImGui/backends/imgui_impl_opengl3.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	this->parent = parent;
	type = typeComponent::Camera;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 1000.0f;

	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);

	drawBoundingBoxes = true;
	frustumCulling = true;
	activeCamera = true;
}

void ComponentCamera::Enable() {}

void ComponentCamera::Disable() {}

void ComponentCamera::Update() {}

float ComponentCamera::GetFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void ComponentCamera::SetFOV(float fov)
{
	frustum.horizontalFov = fov * DEGTORAD;
	frustum.verticalFov = fov * DEGTORAD;
}

void ComponentCamera::SetAspectRatio(float aspectRatio) {

	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspectRatio);
}

float ComponentCamera::GetNearPlane() const
{
	return frustum.nearPlaneDistance;
}

float ComponentCamera::GetFarPlane() const
{
	return frustum.farPlaneDistance;
}

void ComponentCamera::DrawFrustumCube() const
{
	float3 frustumCorners[8];
	frustum.GetCornerPoints(frustumCorners);
	ExternalApp->renderer3D->DrawBox(frustumCorners, float3(0, 255, 0));
}

float* ComponentCamera::GetRawViewMatrix()
{
	static float4x4 m;

	m = frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.v;
}

float4x4 ComponentCamera::GetViewMatrix()
{
	float4x4 tempMat4x4 = frustum.ViewMatrix();
	return tempMat4x4.Transposed();
}

float* ComponentCamera::GetProjectionMatrix()
{
	static float4x4 m;

	m = frustum.ProjectionMatrix();
	m.Transpose();

	return (float*)m.v;
}

float3 ComponentCamera::GetFront()
{
	return frustum.front;
}

float3 ComponentCamera::GetUp()
{
	return frustum.up;
}

float3 ComponentCamera::GetRight()
{
	return frustum.WorldRight();
}

void ComponentCamera::SetPos(float3 xyz)
{
	frustum.pos = xyz;
}

void ComponentCamera::SetPos(float x, float y, float z)
{
	frustum.pos = float3(x, y, z);
}

void ComponentCamera::UpdatePos(float3 newPos)
{
	frustum.pos += newPos;
}

float3 ComponentCamera::GetPos() const
{
	return frustum.pos;
}


void ComponentCamera::SetFront(float3 front)
{
	frustum.front = front;
}

void ComponentCamera::SetUp(float3 up)
{
	frustum.up = up;
}


void ComponentCamera::Match(ComponentCamera* reference)
{
	SetPos(reference->frustum.pos);
	SetFront(reference->frustum.front);
	SetUp(reference->frustum.up);
}

void ComponentCamera::Pan(float3& newPos, float speed, float dt)
{
	int dx = -ExternalApp->input->GetMouseXMotion();
	int dy = -ExternalApp->input->GetMouseYMotion();

	float Sensitivity = 1.6f * dt;

	float DeltaX = (float)dx * Sensitivity;
	float DeltaY = (float)dy * Sensitivity;

	newPos -= GetUp() * speed * DeltaY;
	newPos += GetRight() * speed * DeltaX;
}

//----------------- PUEDE SER UTIL CUANDO TENGAMOS TRANSFORM -----------------
//
//void ComponentCamera::Orbit(float motion_x, float motion_y)
//{
//	float3 point = looking_at;
//
//	if (App->scene->selected_object != nullptr)
//	{
//		looking_at = App->scene->selected_object->transform->GetPosition();
//	}
//
//	float3 focus = frustum.Pos() - point;
//
//	Quat y = Quat(frustum.Up(), motion_x);
//	Quat x = Quat(frustum.WorldRight(), motion_y);
//
//	focus = x.Transform(focus);
//	focus = y.Transform(focus);
//
//	frustum.SetPos(focus + point);
//
//	Look(point);
//}

void ComponentCamera::Zoom(float3& newPos, float speed)
{
	if (ExternalApp->input->GetMouseZ() > 0) newPos += GetFront() * speed;
	if (ExternalApp->input->GetMouseZ() < 0) newPos -= GetFront() * speed;
}

void ComponentCamera::LookAt(float3& Spot)
{
	float3 Z = (Spot - GetPos()).Normalized();
	SetFront(Z);

	float3 X = float3(0, 1, 0).Cross(GetFront()).Normalized();

	float3 Y = GetFront().Cross(X);
	SetUp(Y);
}

void ComponentCamera::Move(float3& newPos, float speed)
{
	if (ExternalApp->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += GetFront() * speed;
	if (ExternalApp->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= GetFront() * speed;

	if (ExternalApp->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= GetRight() * speed;
	if (ExternalApp->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += GetRight() * speed;
}

void ComponentCamera::Rotate(float speed, float dt)
{
	int dx = -ExternalApp->input->GetMouseXMotion();
	int dy = -ExternalApp->input->GetMouseYMotion();

	float Sensitivity = 0.35f * dt;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		float3 rotationAxis(0.0f, 1.0f, 0.0f);
		Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

		SetUp(rotationQuat * GetUp());
		SetFront(rotationQuat * GetFront());
	}
	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Quat rotationQuat = Quat::RotateAxisAngle(GetRight(), DeltaY);

		SetUp(rotationQuat * GetUp());
		SetFront(rotationQuat * GetFront());

		if (GetUp().y < 0.0f)
		{
			SetFront(float3(0.0f, GetFront().y > 0.0f ? 1.0f : -1.0f, 0.0f));

			SetUp(GetFront().Cross(GetRight()));
		}

	}
}


void ComponentCamera::DrawInspector()
{

	if (ImGui::CollapsingHeader("Component Camera"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		//if (ImGui::Checkbox("Viewport Camera", &active_camera)) {}

		if (ImGui::Checkbox("Frustum Culling", &frustumCulling)) {}

		//Set FOV
		float Inspector_FOV = GetFOV();
		if (ImGui::SliderFloat("FOV", &Inspector_FOV, 30, 120, "%0.2f", ImGuiSliderFlags_None)) { SetFOV(Inspector_FOV); }

		//Set NearPlane
		float NearPlane = GetNearPlane();
		ImGui::DragFloat("Near plane", &frustum.nearPlaneDistance, 0.1f, 0.01f, frustum.nearPlaneDistance);

		//Set FarPlane
		float FarPlane = GetFarPlane();
		ImGui::DragFloat("Far plane", &frustum.farPlaneDistance, 0.1f, frustum.farPlaneDistance, 10000.f);

		//Add Toggle for frustum draw?

	}
}
