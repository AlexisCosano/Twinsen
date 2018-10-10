#include "Application.h"
#include "ModuleFBXLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib");


ModuleFBXLoader::ModuleFBXLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("fbxloader");
}

ModuleFBXLoader::~ModuleFBXLoader() {}

// -----------------------------------------------------------------
bool ModuleFBXLoader::Start()
{
	bool ret = true;

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

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
void ModuleFBXLoader::LoadFile(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiNode* root = scene->mRootNode;

			LoadMesh(scene, root);
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", file_path);
	}
}

// ------------------------------------------------------------------
void ModuleFBXLoader::LoadMesh(const aiScene* scene, aiNode* children_node)
{
	for (int i = 0; i < children_node->mNumChildren; ++i)
	{
		for (int j = 0; j < children_node->mChildren[i]->mNumMeshes; j++)
		{
			aiMesh* current_mesh = scene->mMeshes[children_node->mChildren[i]->mMeshes[j]];
			mesh_to_load = MeshData();

			mesh_to_load.num_vertex = current_mesh->mNumVertices;
			mesh_to_load.vertex = new uint[mesh_to_load.num_vertex * 3];

			memcpy(mesh_to_load.vertex, current_mesh->mVertices, sizeof(float) * mesh_to_load.num_vertex * 3);

			glGenBuffers(1, (GLuint*)&(mesh_to_load.id_vertex));
			glBindBuffer(GL_ARRAY_BUFFER, mesh_to_load.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh_to_load.num_vertex, mesh_to_load.vertex, GL_STATIC_DRAW);
			LOG("Mesh with %d vertices.", mesh_to_load.num_vertex);

			if (current_mesh->HasFaces())
			{
				mesh_to_load.num_index = current_mesh->mNumFaces * 3;
				mesh_to_load.index = new uint[mesh_to_load.num_index];

				for (uint k = 0; k < current_mesh->mNumFaces; ++k)
				{
					if (current_mesh->mFaces[k].mNumIndices != 3)
					{
						LOG("WARNING: geometry face with != 3 index!");
					}
					else
					{
						memcpy(&mesh_to_load.index[k * 3], current_mesh->mFaces[k].mIndices, 3 * sizeof(float));
					}
				}

				glGenBuffers(1, (GLuint*)&(mesh_to_load.id_index));
				glBindBuffer(GL_ARRAY_BUFFER, mesh_to_load.id_index);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh_to_load.num_index, mesh_to_load.index, GL_STATIC_DRAW);
			}

			meshes.push_back(mesh_to_load);
			LOG("Loaded mesh with %i vertices.", mesh_to_load.num_vertex);
			LOG("Loaded mesh with %i indices.", mesh_to_load.num_index);
			LOG("Loaded mesh with %i triangles.", mesh_to_load.num_vertex / 3);
		}

		LoadMesh(scene, children_node->mChildren[i]);
	}
}

// ------------------------------------------------------------------
bool ModuleFBXLoader::CleanUp()
{
	LOG("Cleaning fbx loader");

	aiDetachAllLogStreams();

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