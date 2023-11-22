#include "ComponentCamera.h"

#include "Globals.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Application.h"

#include "../External/ImGui/imgui.h"
#include "../External/ImGui/backends/imgui_impl_sdl2.h"
#include "../External/ImGui/backends/imgui_impl_opengl3.h"

#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Math/float3x3.h"
#include "../External/MathGeoLib/include/Math/Quat.h"


ComponentCamera::ComponentCamera(GameObject* _parent) : active_camera(false), Component(_parent)
{
	this->parent = _parent;
	
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 1000.0f;

	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);

	draw_boundingboxes = true;
	frustum_culling = false;
	active_camera = false;
}

ComponentCamera::~ComponentCamera() {}

void ComponentCamera::Enable() {}

void ComponentCamera::Disable() {}

void ComponentCamera::Update() {}

void ComponentCamera::DrawInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	
	if (ImGui::CollapsingHeader("Camera"), flags)
	{
		ImGui::Indent();
		float horizontalFOV = GetHorizontalFOV();
		float verticalFOV = GetVerticalFOV();
		float FOV = GetHorizontalFOV();

		if (ImGui::SliderFloat("Horizontal FOV", &horizontalFOV, 30, 120, "%0.2f", ImGuiSliderFlags_None))
		{
			SetHorizontalFOV(horizontalFOV);
		}

		if (ImGui::SliderFloat("Vertical FOV", &verticalFOV, 30, 120, "%0.2f", ImGuiSliderFlags_None))
		{
			SetVerticalFOV(verticalFOV);
		}

		if (ImGui::SliderFloat("Both FOV", &FOV, 30, 120, "%0.2f", ImGuiSliderFlags_None))
		{
			SetFOV(FOV);
		}

		ImGui::Unindent();
	}
}


void ComponentCamera::UpdatePos(float3 newPos)
{
	frustum.pos += newPos;
}


void ComponentCamera::DrawFrustumBox() const
{
	float3 vertices[8];
	frustum.GetCornerPoints(vertices);
	ExternalApp->renderer3D->DrawBox(vertices, float3(0, 255, 0));
}

// GETTERS -------------------------------
float ComponentCamera::GetHorizontalFOV() const
{
	return frustum.horizontalFov * RADTODEG;
}

float ComponentCamera::GetVerticalFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

float3 ComponentCamera::GetPos() const
{
	return frustum.pos;
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

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

float4x4 ComponentCamera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed();
}

float4x4 ComponentCamera::GetViewMatrix() const
{
	float4x4 matrix = frustum.ViewMatrix();
	return matrix;
}

// SETTERS -------------------------------
void ComponentCamera::SetHorizontalFOV(float horizontalfov) 
{
	frustum.horizontalFov = horizontalfov * RADTODEG;
}

void ComponentCamera::SetVerticalFOV(float verticalfov)
{
	frustum.verticalFov = verticalfov * RADTODEG;
}

void ComponentCamera::SetFOV(float FOV)
{
	frustum.horizontalFov = FOV * DEGTORAD;
	frustum.verticalFov = FOV * DEGTORAD;
}

void ComponentCamera::SetPos(float3 pos)
{
	frustum.pos = pos;
}

void ComponentCamera::SetPos(float x, float y, float z)
{
	frustum.pos = float3(x, y, z);
}

void ComponentCamera::SetFront(float3 front)
{
	frustum.front = front;
}

void ComponentCamera::SetUp(float3 up)
{
	frustum.up = up;
}

void ComponentCamera::SetAspectRatio(float aspectRatio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspectRatio);
}

void ComponentCamera::SetAspectRatio(int width, int height)
{
	float verticalAspectRatio = (float)width / (float)height;
	frustum.verticalFov = 2.f * atanf(tanf(frustum.horizontalFov * 0.5f) / verticalAspectRatio);
}

/*----------------------------------------------*/


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

void ComponentCamera::Move(float3& newPos, float speed)
{
	if (ExternalApp->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += GetFront() * speed;
	if (ExternalApp->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= GetFront() * speed;

	if (ExternalApp->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= GetRight() * speed;
	if (ExternalApp->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += GetRight() * speed;
}

void ComponentCamera::Zoom(float3& newPos, float speed)
{
	if (ExternalApp->input->GetMouseZ() > 0) 
		newPos += GetFront() * speed;
	if (ExternalApp->input->GetMouseZ() < 0) 
		newPos -= GetFront() * speed;
}

void ComponentCamera::Pan(float3& newPos, float speed, float dt)
{
	int dx = -ExternalApp->input->GetMouseXMotion();
	int dy = -ExternalApp->input->GetMouseYMotion();

	float sensitivity = 2.0f * dt;

	float deltaX = (float)dx * sensitivity;
	float deltaY = (float)dy * sensitivity;

	newPos -= GetUp() * speed * deltaY;
	newPos += GetRight() * speed * deltaX;
}

void ComponentCamera::LookAt(float3& Spot)
{
	float3 Z = (Spot - GetPos()).Normalized();
	SetFront(Z);

	float3 X = float3(0, 1, 0).Cross(GetFront()).Normalized();

	float3 Y = GetFront().Cross(X);
	SetUp(Y);
}