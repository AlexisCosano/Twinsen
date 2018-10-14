#ifndef __MODULEINTERFACE_H__
#define __MODULEINTERFACE_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI/imgui.h"
#include "MathGeoLib/Geometry/Sphere.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Algorithm/Random/LCG.h"

#define GRAPH_ARRAY_SIZE 70

class Console;

class ModuleInterface : public Module
{
private:
	int random_number = 0;
	int width = 1280;
	int height = 1024;
	float brightness = 1.0;
	float fps_array[GRAPH_ARRAY_SIZE];
	float ms_array[GRAPH_ARRAY_SIZE];

public:
	ModuleInterface(Application* app, bool start_enabled = true);
	~ModuleInterface();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	// Save & load
	bool Save();
	bool Load();

	Console* console;

};
#endif