#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("input");

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	file_path.clear();

	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (App->config != NULL)
		{
			input_object = json_object_dotget_object(App->modules_object, "input");
		}
	}

	return ret;
}

FILE_TYPE ModuleInput::GetFileType(std::string file_name)
{
	std::string type = file_name.substr(file_name.find_last_of("."));

	if (type == ".fbx" || type == ".obj" ||
		type == ".FBX" || type == ".OBJ")
	{
		return MODEL;
	}

	else if (type == ".png" || type == ".jpg" || type == ".bmp" || type == ".dds" ||
		type == ".PNG" || type == ".JPG" || type == ".BMP" || type == ".DDS")
	{
		return TEXTURE;
	}
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= App->window->wscale;
	mouse_y /= App->window->wscale;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			FILE_TYPE type;

			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / App->window->wscale;
			mouse_y = e.motion.y / App->window->wscale;

			mouse_x_motion = e.motion.xrel / App->window->wscale;
			mouse_y_motion = e.motion.yrel / App->window->wscale;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_DROPFILE:
				file_path = e.drop.file;
				type = GetFileType(file_path.c_str());
				if (type == MODEL)
				{
					if(App->fbx_loader->meshes.size() != 0)
					{
						App->fbx_loader->meshes.clear();
						App->fbx_loader->LoadFile(file_path.c_str());
					}
					else
						App->fbx_loader->LoadFile(file_path.c_str());

					App->fbx_loader->Focus();
				}
				else if (type == TEXTURE)
				{
					App->fbx_loader->SetTexture(file_path.c_str());
				}

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP || quit_engine == true)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

bool ModuleInput::Quit()
{
	quit_engine = true;

	return(quit_engine);
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// Save & load ----------------------------------------------------------------------
bool ModuleInput::Save()
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

bool ModuleInput::Load()
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