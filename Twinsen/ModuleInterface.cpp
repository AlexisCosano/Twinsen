#include "Application.h"
#include "ModuleInterface.h"
#include "Glew\include\glew.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleInterface::ModuleInterface(Application* app, bool start_enabled) : Module(app, start_enabled){}

ModuleInterface::~ModuleInterface(){}

// -----------------------------------------------------------------
bool ModuleInterface::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// ------------------------------------------------------------------
update_status ModuleInterface::Update(float dt)
{
	static bool show_test_window = false;
	static bool show_about_window = false;
	static bool quit = false;

	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	if (show_about_window)
	{
		ImGui::Begin("About");

		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		static bool show_application = false;

		ImGui::TextWrapped("Twinsen Engine - by Alexis Cosano Rodriguez");
		ImGui::Separator();
		ImGui::TextWrapped("3D Game Engine created as a project for college.");
		
		ImGui::End();
	}
	else
	{
		show_about_window = false;
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

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Checkbox("Show test window", &show_test_window);

			ImGui::Separator();

			if (ImGui::MenuItem("Engine's wiki"))
				App->RequestBrowser("https://github.com/AlexisCosano/Twinsen/wiki");
			
			if (ImGui::MenuItem("Download the latest release"))
				App->RequestBrowser("https://github.com/AlexisCosano/Twinsen/releases");
			
			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/AlexisCosano/Twinsen/issues");
			
			ImGui::Checkbox("About", &show_about_window);
				
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
				if (sphere1.Intersects(sphere2))
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

	return (UPDATE_CONTINUE);
}

// ------------------------------------------------------------------
bool ModuleInterface::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}
