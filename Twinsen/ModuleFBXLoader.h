#ifndef __MODULEFBXLOADER_H__
#define __MODULEFBXLOADER_H__

#include "Module.h"
#include "Globals.h"
#include "glew\include\glew.h"
#include <vector>
#include <string>


struct aiScene;
struct aiNode;

struct MeshData
{
	uint texture_id = 0;

	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	uint* vertex = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* uvs = nullptr;
};

class ModuleFBXLoader : public Module
{
private:
	MeshData mesh_to_load;

public:
	uint last_texture_id = 0;
	
	std::vector<MeshData> meshes;

public:
	ModuleFBXLoader(Application* app, bool start_enabled = true);
	~ModuleFBXLoader();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void LoadFile(std::string file_path);
	void LoadMesh(const aiScene* scene, aiNode* children_node);
	uint GenerateTextureId(const char* texture_path);
	void SetTexture(const char* path);

	// Save & load
	bool Save();
	bool Load();

};
#endif
