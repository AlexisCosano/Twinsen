#include "Application.h"
#include "ModuleFBXLoader.h"


ModuleFBXLoader::ModuleFBXLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("fbxloader");
}

ModuleFBXLoader::~ModuleFBXLoader() {}

// -----------------------------------------------------------------
bool ModuleFBXLoader::Start()
{
	bool ret = true;

	return ret;
}

// Pre update -------------------------------------------------------
update_status ModuleFBXLoader::PreUpdate(float dt)
{
	return(UPDATE_CONTINUE);
}

// ------------------------------------------------------------------
update_status ModuleFBXLoader::Update(float dt)
{
	return (UPDATE_CONTINUE);
}

// ------------------------------------------------------------------
bool ModuleFBXLoader::CleanUp()
{
	LOG("Cleaning fbx loader");

	return true;
}

// Save & load ----------------------------------------------------------------------
bool ModuleFBXLoader::Save()
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
	LOG("Saving module %s", name._Get_data()._Myptr());
	return(true);
}

bool ModuleFBXLoader::Load()
{
	LOG("Loading module %s", name._Get_data()._Myptr());
	return(true);
}