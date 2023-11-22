#pragma once
#include "Component.h"

#include "Application.h"
#include "Globals.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"

#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "../External/MathGeoLib/include/Math/float4x4.h"
#include "../External/MathGeoLib/include/Geometry/Frustum.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Geometry/LineSegment.h"

class GameObject;

class ComponentCamera : public Component {
public:

	ComponentCamera(GameObject* parent);
	~ComponentCamera();

	void Enable() override;
	void Disable() override;

	void Update() override;
	void DrawInspector() override;

	void UpdatePos(float3 newPos);

	// ----------------- GETTERS -----------------

	float3 GetPos() const;

	float3 GetFront();
	float3 GetUp();
	float3 GetRight();

	float GetHorizontalFOV() const;
	float GetVerticalFOV() const;

	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix() const;

	float GetAspectRatio() const;

	// ----------------- SETTERS ----------------- 

	void SetFront(float3 front);
	void SetUp(float3 up);

	void SetPos(float3 pos);
	void SetPos(float x, float y, float z);

	void SetHorizontalFOV(float hfov);
	void SetVerticalFOV(float vfov);
	void SetFOV(float fov);

	void SetAspectRatio(float aspectRatio);
	void SetAspectRatio(int width, int height);

	/*--------------------------------------------------------------*/

	//Movement controls
	void Move(float3& newPos, float speed);

	//Zoom control
	void Zoom(float3& newPos, float speed);

	void Rotate(float speed, float dt);
	void Pan(float3& newPos, float speed, float dt);
	void LookAt(float3& Spot);

	void DrawFrustumBox() const;

public:

	Frustum frustum;

	bool active_camera;
	bool frustum_culling;
	bool draw_boundingboxes;

};
