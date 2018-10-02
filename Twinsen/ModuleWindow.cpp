#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;

	//default variables in case config.xml is not loaded
	window_title = "ERROR: window's title could not be loaded.";
	wwidth = 1280;
	wheight = 1024;
	wscale = 1;
	wfullscreen = false;
	wresizable = true;
	wborderless = false;
	wwindowed_fullscreen = false;
	wvsync = true;

	name.assign("window");
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (App->config != NULL)
		{
			window_object = json_object_dotget_object(App->modules_object, "window");

			window_title = json_object_dotget_string(window_object, "title");
			SetTitle(window_title);
			LOG("Title: %s\n", window_title);

			wwidth = json_object_dotget_number(window_object, "width");
			wheight = json_object_dotget_number(window_object, "height");
			wscale = json_object_dotget_number(window_object, "scale");
			wfullscreen = json_object_dotget_boolean(window_object, "fullscreen");
			wresizable = json_object_dotget_boolean(window_object, "resizable");
			wborderless = json_object_dotget_boolean(window_object, "borderless");
			wwindowed_fullscreen = json_object_dotget_boolean(window_object, "windowed_fullscreen");
			wvsync = json_object_dotget_boolean(window_object, "vsync");
		}

		//Create window
		int width = wwidth * wscale;
		int height = wheight * wscale;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(wfullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(wresizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(wborderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(wwindowed_fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

const char* ModuleWindow::GetTitle()
{
	return(window_title);
}

// Save & load ----------------------------------------------------------------------
bool ModuleWindow::Save()
{
	/*
	if (App->savefile_node.child(name.GetString()) == NULL)
	{
		App->savefile_node.append_child(name.GetString());
		App->savefile_document.save_file("savefile.xml");
	}
	else
	{
		LOG("Nothing to save yet.");
	}
	*/

	if (json_object_has_value(App->modules_object, "window"))
	{
		json_object_set_number(window_object, "width", 1024);
		json_serialize_to_file_pretty(App->config, "config.json");
	}
	LOG("Saving module %s", name._Get_data()._Myptr());
	return(true);
}

bool ModuleWindow::Load()
{
	if (App->config != NULL)
	{
		window_title = json_object_dotget_string(window_object, "title");

		wwidth = json_object_dotget_number(window_object, "width");
		wheight = json_object_dotget_number(window_object, "height");
		wscale = json_object_dotget_number(window_object, "scale");
		wfullscreen = json_object_dotget_boolean(window_object, "fullscreen");
		wresizable = json_object_dotget_boolean(window_object, "resizable");
		wborderless = json_object_dotget_boolean(window_object, "borderless");
		wwindowed_fullscreen = json_object_dotget_boolean(window_object, "windowed_fullscreen");
		wvsync = json_object_dotget_boolean(window_object, "vsync");

		LOG("Loading module %s", name._Get_data()._Myptr());
	}
	else
	{
		LOG("Document not found.");
	}
	
	return(true);
}