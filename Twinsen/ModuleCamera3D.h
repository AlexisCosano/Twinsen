#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/Math/float3x3.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float3.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const math::float3 &Position, const math::float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const math::float3 &Spot);
	void Move(const math::float3 &Movement);
	math::float4x4* GetViewMatrix();

	// Save & load
	bool Save();
	bool Load();
private:

	void CalculateViewMatrix();

public:
	
	math::float3 X, Y, Z, Position, Reference;

private:

	math::float4x4 ViewMatrix, ViewMatrixInverse;
};