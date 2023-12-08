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

	FBO = 0; 
	TCB = 0; 
	RBO = 0;

	drawBoundingBoxes = true;
	frustumCulling = true;
	activeCamera = true;
}

void ComponentCamera::Enable() {}

void ComponentCamera::Disable() {}

void ComponentCamera::Update()
{
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(GetProjectionMatrix().ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(GetViewMatrix().ptr());

	if (parent != nullptr)
	{
		//Camera movement
		frustum.front = parent->transform->GetForward();
		frustum.up = parent->transform->GetUp();
		frustum.pos = parent->transform->GetPosition();
	}
}

float ComponentCamera::GetVerticalFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

float ComponentCamera::GetHorizontalFOV() const
{
	return frustum.horizontalFov * RADTODEG;
}

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void ComponentCamera::SetVerticalFOV(float fov)
{
	frustum.verticalFov = fov * DEGTORAD;
}

void ComponentCamera::SetHorizontalFOV(float fov)
{
	frustum.horizontalFov = fov * DEGTORAD;
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
	ExternalApp->renderer3D->DrawBox(frustumCorners, float3(0.0f, 1.0f, 0.0f));
}

float* ComponentCamera::GetRawViewMatrix()
{
	float4x4 tempMat4x4 = frustum.ViewMatrix();
	return tempMat4x4.Transposed().ptr();
}

float4x4 ComponentCamera::GetViewMatrix()
{
	float4x4 tempMat4x4 = frustum.ViewMatrix();
	return tempMat4x4.Transposed();
}

float4x4 ComponentCamera::GetProjectionMatrix()
{
	return frustum.ProjectionMatrix().Transposed();
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

	float DeltaX = (float)dx * speed;
	float DeltaY = (float)dy * speed;

	newPos -= GetUp() * DeltaY / 2;
	newPos += GetRight() * DeltaX / 2;
}

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

void ComponentCamera::Rotate(float dt)
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

void ComponentCamera::Orbit(float3& point, float speed, float dt)
{
	int motionX = -ExternalApp->input->GetMouseXMotion();
	int motionY = -ExternalApp->input->GetMouseYMotion();

	float3 focus = GetPos() - point;

	Quat y = Quat(GetUp(), motionX * speed / 2);
	Quat x = Quat(frustum.WorldRight(), motionY * speed / 2);

	focus = x.Transform(focus);
	focus = y.Transform(focus);

	SetPos(focus + point);

	LookAt(point);
}

void ComponentCamera::Focus(float3& center, float distance)
{
	float3 newPosition = center + float3(-distance, distance, distance);
	SetPos(newPosition);

	LookAt(center);
}

void ComponentCamera::DrawInspector()
{

	if (ImGui::CollapsingHeader("Component Camera"))
	{
		ImGui::Checkbox("Frustum Culling", &frustumCulling);

		//Set General FOV
		float FOV = GetVerticalFOV();
		if (ImGui::SliderFloat("FOV", &FOV, 30, 120, "%0.2f", ImGuiSliderFlags_None)) { SetFOV(FOV); }

		//Set NearPlane
		float NearPlane = GetNearPlane();
		ImGui::DragFloat("Near plane", &frustum.nearPlaneDistance, 0.1f, 0.01f, frustum.farPlaneDistance);

		//Set FarPlane
		float FarPlane = GetFarPlane();
		ImGui::DragFloat("Far plane", &frustum.farPlaneDistance, 0.1f, frustum.nearPlaneDistance, 10000.f);

		if (ImGui::BeginCombo("Frustrum Type", (frustum.type == FrustumType::PerspectiveFrustum) ? "Prespective" : "Orthographic"))
		{
			if (ImGui::Selectable("Perspective"))
				frustum.type = FrustumType::PerspectiveFrustum;

			if (ImGui::Selectable("Orthographic"))
				frustum.type = FrustumType::OrthographicFrustum;

			ImGui::EndCombo();
		}
	}
}

void ComponentCamera::OnUpdateTransform(const float4x4& global, const float4x4& parent_global)
{
	
}

void ComponentCamera::LoadBuffers(int width, int height)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &TCB);
	glBindTexture(GL_TEXTURE_2D, TCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TCB, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	SetAspectRatio((float)width / (float)height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ComponentCamera::RenderBuffers(bool active)
{
	if (active) {

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
	else {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
}

void ComponentCamera::UnloadBuffers()
{
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &TCB);
	glDeleteFramebuffers(1, &FBO);
}