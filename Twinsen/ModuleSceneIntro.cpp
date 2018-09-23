#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	App->camera->Move(math::float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(math::float3(0.0f, 0.0f, 0.0f));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

// PreUpdate
update_status ModuleSceneIntro::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return(UPDATE_CONTINUE);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	/*
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	*/

	static bool show_test_window = false;
	static bool quit = false;

	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Text("New file");
			ImGui::Text("Open file");
			ImGui::Separator();
			ImGui::Text("Save file");
			ImGui::Separator();
			if (ImGui::MenuItem("Exit"))
			{
				App->input->Quit();
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Checkbox("Show test window", &show_test_window);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("MathGeoLib testing"))
		{
			ImGui::Text("Sphere 1 position and radius: ");    ImGui::SameLine(250); ImGui::Text("(0, 0, 0) // 5");
			ImGui::Text("Sphere 2 position and radius: ");    ImGui::SameLine(250); ImGui::Text("(12, 12, 12) // 5");

			static int clicked1 = 0;

			if (ImGui::Button("Check intersection"))
				clicked1++;

			if (clicked1 & 1)
			{
				math::Sphere sphere1(math::float3(0.0f, 0.0f, 0.0f), 5.0f);
				math::Sphere sphere2(math::float3(12.0f, 12.0f, 12.0f), 5.0f);
				

				ImGui::SameLine();
				if(sphere1.Intersects(sphere2))
				{
					ImGui::Text("The spheres intersect.");
				}
				else
				{
					ImGui::Text("The spheres do not intersect.");
				}
					
			}

			ImGui::Separator();

			ImGui::Text("Sphere 1 position and radius: ");    ImGui::SameLine(250); ImGui::Text("(0, 0, 0) // 5");
			ImGui::Text("Sphere 3 position and radius: ");    ImGui::SameLine(250); ImGui::Text("(4, 4, 4) // 5");

			static int clicked2 = 0;

			if (ImGui::Button("Check intersection 2"))
				clicked2++;

			if (clicked2 & 1)
			{
				math::Sphere sphere1(math::float3(0.0f, 0.0f, 0.0f), 5.0f);
				math::Sphere sphere3(math::float3(4.0f, 4.0f, 4.0f), 5.0f);

				ImGui::SameLine();
				if (sphere1.Intersects(sphere3))
				{
					ImGui::Text("The spheres intersect.");
				}
				else
				{
					ImGui::Text("The spheres do not intersect.");
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("RNG"))
		{
			ImGui::Text("Press to generate a random number between 0 and 100.");
			
			LCG random;
			if (ImGui::Button("Generate"))
			{
				random_number = random.Int(0, 100);
			}

			ImGui::SameLine();
			ImGui::Text("%i", random_number);
			

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	
	ImGui::Render();

	return UPDATE_CONTINUE;
}
