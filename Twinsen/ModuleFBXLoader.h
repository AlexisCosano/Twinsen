#ifndef __MODULEFBXLOADER_H__
#define __MODULEFBXLOADER_H__

#include "Module.h"
#include "Globals.h"


class ModuleFBXLoader : public Module
{
private:
	int random_number = 0;

public:
	ModuleFBXLoader(Application* app, bool start_enabled = true);
	~ModuleFBXLoader();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	// Save & load
	bool Save();
	bool Load();

};
#endif
