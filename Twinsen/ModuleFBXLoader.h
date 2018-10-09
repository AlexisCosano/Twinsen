#ifndef __MODULEFBXLOADER_H__
#define __MODULEFBXLOADER_H__

#include "Module.h"
#include "Globals.h"
#include "glew\include\glew.h"
#include <vector>


struct MeshData
{
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex = nullptr;
};

class ModuleFBXLoader : public Module
{
private:
	int random_number = 0;

	MeshData mesh_to_load;

public:
	std::vector<MeshData> meshes;

public:
	ModuleFBXLoader(Application* app, bool start_enabled = true);
	~ModuleFBXLoader();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void LoadFile(const char* file_path);

	// Save & load
	bool Save();
	bool Load();

};
#endif
