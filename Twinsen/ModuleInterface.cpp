#include "Application.h"
#include "ModuleInterface.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleInterface::ModuleInterface(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("interface");
}

ModuleInterface::~ModuleInterface(){}

// -----------------------------------------------------------------
bool ModuleInterface::Init()
{
	bool ret = true;

	glewInit();

	ImGui_ImplSdlGL3_Init(App->window->window);

	return ret;
}

// Pre update -------------------------------------------------------
update_status ModuleInterface::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return(UPDATE_CONTINUE);
}

// ------------------------------------------------------------------
update_status ModuleInterface::Update(float dt)
{
	static bool show_test_window = false;
	static bool show_about_window = false;
	static bool show_opengl_window = false;
	static bool quit = false;

	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	if (show_opengl_window)
	{
		ImGui::Begin("User's information");
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

		ImGui::Text("Glew version: %s", glewGetString(GLEW_VERSION));
		ImGui::Separator();
		ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::Text("OpenGL version supported: %s", glGetString(GL_VERSION));
		ImGui::Text("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		ImGui::End();
	}

	if (show_about_window)
	{
		ImGui::Begin("About");

		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

		ImGui::TextWrapped("Twinsen Engine - by Alexis Cosano Rodriguez");
		ImGui::Separator();
		ImGui::TextWrapped("3D Game Engine created as a project for college.");
		ImGui::Separator();
		ImGui::TextWrapped("Libraries used");
		ImGui::Text("- ImGui  v1.52");
		ImGui::SameLine();
		if (ImGui::MenuItem("Download here"))
		{
			App->RequestBrowser("https://github.com/ocornut/imgui");
		}
		ImGui::Text("- Parson");
		ImGui::SameLine();
		if (ImGui::MenuItem("Download here"))
		{
			App->RequestBrowser("http://kgabis.github.com/parson/");
		}

		ImGui::Text("- MathGeoLib  v2.0");
		ImGui::SameLine();
		if (ImGui::MenuItem("Download here"))
		{
			App->RequestBrowser("https://github.com/juj/MathGeoLib");
		}

		ImGui::Text("- Glew  v7.0");
		ImGui::SameLine();
		if (ImGui::MenuItem("Download here"))
		{
			App->RequestBrowser("http://glew.sourceforge.net/");
		}

		ImGui::Text("- SDL  v2.0");
		ImGui::SameLine();
		if (ImGui::MenuItem("Download here"))
		{
			App->RequestBrowser("https://www.libsdl.org/download-2.0.php");
		}
		ImGui::Separator();
		ImGui::Text("This engine is under the MIT license, to know more, click");
		ImGui::SameLine(); if (ImGui::MenuItem("here.")) { App->RequestBrowser("https://opensource.org/about"); }

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
			ImGui::Text("Save file");
			ImGui::Separator();
			if (ImGui::MenuItem("Save engine configuration"))
			{
				App->CallSave();
			}
			if (ImGui::MenuItem("Load engine configuration"))
			{
				App->CallLoad();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit"))
			{
				App->input->Quit();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Renderer options"))
		{
			if (ImGui::MenuItem("Depth test"))
			{
				GL_DEPTH_TEST;
			}

			if (ImGui::MenuItem("Cull face"))
			{
				GL_CULL_FACE;
			}

			if (ImGui::MenuItem("Lighting"))
			{
				GL_LIGHTING;
			}

			if (ImGui::MenuItem("Color material"))
			{
				GL_COLOR_MATERIAL;
			}

			if (ImGui::MenuItem("Texture 2"))
			{
				GL_TEXTURE_2D;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Wireframe mode"))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
			ImGui::Checkbox("User's information", &show_opengl_window);
				
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

		if (ImGui::BeginMenu("Loaded Textures"))
		{
			for (std::vector<MeshData>::iterator item = App->fbx_loader->meshes.begin(); item != App->fbx_loader->meshes.end(); ++item)
				ImGui::Image((ImTextureID)(*item).texture_id, ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, -1));

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
	LOG("Cleaning interface");

	ImGui_ImplSdlGL3_Shutdown();

	return true;
}

// Save & load ----------------------------------------------------------------------
bool ModuleInterface::Save()
{
	if (App->config != NULL)
	{
		if (json_object_has_value(App->modules_object, name._Get_data()._Myptr()) == false)
		{
			json_object_set_null(App->modules_object, name._Get_data()._Myptr());
			json_serialize_to_file_pretty(App->config, "config.json");
		}

		LOG("Saving module %s", name._Get_data()._Myptr());
	}
	else
	{
		json_object_set_null(App->modules_object, name._Get_data()._Myptr());

		LOG("Saving module %s", name._Get_data()._Myptr());
	}


	return(true);
}

bool ModuleInterface::Load()
{
	bool ret = false;

	if (App->config != NULL)
	{
		if (json_object_has_value(App->modules_object, name._Get_data()._Myptr()) != false)
		{
			LOG("Loading module %s", name._Get_data()._Myptr());
			ret = true;
		}
		else
		{
			LOG("Could not find the node named %s inside the file config.json", name._Get_data()._Myptr());
			ret = false;
		}
	}
	else
	{
		LOG("Document config.json not found.");
		ret = false;
	}

	return ret;
}