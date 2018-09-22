#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = math::float3(1.0f, 0.0f, 0.0f);
	Y = math::float3(0.0f, 1.0f, 0.0f);
	Z = math::float3(0.0f, 0.0f, 1.0f);

	Position = math::float3(0.0f, 0.0f, 5.0f);
	Reference = math::float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	math::float3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		/*/
		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, math::float3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, math::float3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, math::float3(0.0f, 1.0f, 0.0f));
		}
		*/
		/*
		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = math::float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = math::Cross(Z, X);
			}
		}
		*/

		Position = Reference + Z * math::float3(Position).Length();
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const math::float3 &Position, const math::float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = math::float3(Position - Reference).Normalized();
	X = (math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = math::Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const math::float3 &Spot)
{
	Reference = Spot;

	Z = math::float3(Position - Reference).Normalized();
	X = (math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = math::Cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const math::float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
/*
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}
*/

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = math::float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -math::Dot(X, Position), -math::Dot(Y, Position), -math::Dot(Z, Position), 1.0f);
	ViewMatrixInverse = math::float4x4(ViewMatrix).Inverted();
}