#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "JSON/parson.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	// Save & load
	bool Save();
	bool Load();

	const char* GetTitle();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	const char* window_title = "Config not found.";
	int wwidth;
	int wheight;
	int wscale;
	bool wfullscreen;
	bool wresizable;
	bool wborderless;
	bool wwindowed_fullscreen;
	bool wvsync;

private:

	JSON_Object *window_object;
};

#endif // __ModuleWindow_H__