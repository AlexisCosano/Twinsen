#include "Application.h"
#include "ModuleInterface.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "Console.h"
#include "mmgr/mmgr.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


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

	console = new Console();

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
	static bool show_console =false;
	static bool show_engine_configuration = false;
	static bool show_mesh_properties = false;
	static bool quit = false;
	
	if(show_console)
	{ 
		console->active = true;
		console->Draw("Twinsen console");
	}

	if (show_mesh_properties)
	{
		ImGui::Begin("Mesh properties");
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

		ImGui::Text("");

		ImGui::Text("Trtansformation");
		ImGui::Separator();
		ImGui::Text("Position: (%f, %f, %f)", 0.0f, 0.0f, 0.0f);
		ImGui::Text("Rotation: (%f, %f, %f)", 0.0f, 0.0f, 0.0f);
		ImGui::Text("Scale: (%f, %f, %f)", 1.0f, 1.0f, 1.0f);

		ImGui::Text("");

		ImGui::Text("Geometry");
		ImGui::Separator();
		for (std::vector<MeshData>::iterator item = App->fbx_loader->meshes.begin(); item != App->fbx_loader->meshes.end(); ++item)
		{
			ImGui::Text("Mesh's number of uvs: %i", item->num_uvs);
			ImGui::Text("Mesh's number of normals: %i", item->num_normals);
			ImGui::Text("Mesh's number of index: %i", item->num_index);
			ImGui::Text("Mesh's number of vertex: %i", item->num_vertex);
			ImGui::Text("Mesh's triangles: %i", item->num_vertex/3);
		}

		ImGui::Text("");

		ImGui::Text("Textures");
		ImGui::Separator();
		for (std::vector<MeshData>::iterator item = App->fbx_loader->meshes.begin(); item != App->fbx_loader->meshes.end(); ++item)
			ImGui::Image((ImTextureID)(*item).texture_id, ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, -1));

		ImGui::End();
	}

	if (show_engine_configuration)
	{
		ImGui::Begin("Engine configuration");
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::TextWrapped("Engine name: Twinsen");

			ImGui::Separator();
		
			for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
			{
				fps_array[i] = fps_array[i + 1];
			}
			fps_array[GRAPH_ARRAY_SIZE - 1] = ImGui::GetIO().Framerate;
			char fps_title[25];
			sprintf_s(fps_title, 25, "Framerate %.1f", fps_array[GRAPH_ARRAY_SIZE - 1]);
			ImGui::PlotHistogram("", fps_array, IM_ARRAYSIZE(fps_array), 30, fps_title, 0.0f, 130.0f, ImVec2(0, 80));

			for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
			{
				ms_array[i] = ms_array[i + 1];
			}
			ms_array[GRAPH_ARRAY_SIZE - 1] = 1000.0f / ImGui::GetIO().Framerate;
			char ms_title[25];
			sprintf_s(ms_title, 25, "Milliseconds %.1f", ms_array[GRAPH_ARRAY_SIZE - 1]);
			ImGui::PlotHistogram("", ms_array, IM_ARRAYSIZE(ms_array), 30, ms_title, 0.0f, 130.0f, ImVec2(0, 80));

			sMStats smstats = m_getMemoryStatistics();
			ImGui::Text("Accumulated actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.accumulatedActualMemory);
			ImGui::Text("Current memory peak: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.peakActualMemory);
			ImGui::Text("Total actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.totalActualMemory);
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImGui::DragFloat("Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");

			ImGui::SliderFloat("Brightness", &brightness, 0, 2, NULL);

			ImGui::SliderInt("Width", &width, 0, 1920, NULL);

			ImGui::SliderInt("Height", &height, 0, 1080, NULL);

			if (ImGui::Checkbox("Fullscreen", &App->window->wfullscreen))
			{
				App->window->SetFullScreen(&App->window->wfullscreen);
			}

			if (ImGui::Checkbox("Windowed", &App->window->wwindowed_fullscreen))
			{
				App->window->SetWindowed(&App->window->wwindowed_fullscreen);
			}

			if (ImGui::Checkbox("Borderless", &App->window->wborderless))
			{
				App->window->SetWindowFullDesktop();
				width = 1920;
				height = 1080;
			}

			if (ImGui::Button("Apply"))
			{
				App->window->SetWindowSize(width, height);
				App->window->SetWindowBrigthness(brightness);
			}
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::TextWrapped("Mouse position:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "X: %i ", App->input->GetMouseX());
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "Y: %i", App->input->GetMouseY());
		}

		if (ImGui::CollapsingHeader("User's information"))
		{
			ImGui::Text("Glew version: %s", glewGetString(GLEW_VERSION));
			ImGui::Separator();
			ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
			ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
			ImGui::Text("OpenGL version supported: %s", glGetString(GL_VERSION));
			ImGui::Text("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		}

		ImGui::End();
	}

	if (show_test_window)
	{
		ImGui::ShowTestWindow();
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

		if (ImGui::BeginMenu("Utilities"))
		{
			ImGui::Checkbox("Open console", &show_console);
			ImGui::Checkbox("Engine configuration", &show_engine_configuration);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Mesh"))
		{
			ImGui::Checkbox("Mesh properties", &show_mesh_properties);

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