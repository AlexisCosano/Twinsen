#include "Application.h"
#include "ModuleFBXLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#include "Console.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


ModuleFBXLoader::ModuleFBXLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("fbxloader");
}

ModuleFBXLoader::~ModuleFBXLoader() {}

// -----------------------------------------------------------------
bool ModuleFBXLoader::Init()
{
	bool ret = true;

	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

void ModuleFBXLoader::SetTexture(const char* path)
{
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);
	last_texture_id = ilutGLBindTexImage();
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
void ModuleFBXLoader::LoadFile(std::string file_path)
{
	const aiScene* scene = aiImportFile(file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* root = scene->mRootNode;

		for (int i = 0; i < root->mNumChildren; i++)
		{
			LoadMesh(scene, root->mChildren[i]);
		}

		aiReleaseImport(scene);

		LOG("Loading scene with path: %s", file_path.c_str());
		App->ui->console->AddLog("Loading scene with path: %s", file_path.c_str());
	}
	else
	{
		LOG("Error loading scene %s", file_path.c_str());
		App->ui->console->AddLog("Error loading scene %s", file_path.c_str());
	}
}

// ------------------------------------------------------------------
void ModuleFBXLoader::LoadMesh(const aiScene* scene, aiNode* children_node)
{
	if (children_node->mNumMeshes >= 0)
	{
		for (int i = 0; i < children_node->mNumMeshes; ++i)
		{
			aiMesh* current_mesh = scene->mMeshes[children_node->mMeshes[i]];
			mesh_to_load = MeshData();

			mesh_to_load.num_vertex = current_mesh->mNumVertices;
			mesh_to_load.vertex = new uint[mesh_to_load.num_vertex * 3];

			memcpy(mesh_to_load.vertex, current_mesh->mVertices, sizeof(float) * mesh_to_load.num_vertex * 3);

			glGenBuffers(1, (GLuint*)&(mesh_to_load.id_vertex));
			glBindBuffer(GL_ARRAY_BUFFER, mesh_to_load.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh_to_load.num_vertex, mesh_to_load.vertex, GL_STATIC_DRAW);
			LOG("Mesh with %d vertices.", mesh_to_load.num_vertex);
			App->ui->console->AddLog("Mesh with %d vertices.", mesh_to_load.num_vertex);

			if (current_mesh->HasFaces())
			{
				mesh_to_load.num_index = current_mesh->mNumFaces * 3;
				mesh_to_load.index = new uint[mesh_to_load.num_index];

				for (uint j = 0; j < current_mesh->mNumFaces; ++j)
				{
					if (current_mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING: geometry face with != 3 index!");
						App->ui->console->AddLog("WARNING: geometry face with != 3 index!");
					}
					else
					{
						memcpy(&mesh_to_load.index[j * 3], current_mesh->mFaces[j].mIndices, 3 * sizeof(float));
					}
				}

				glGenBuffers(1, (GLuint*)&(mesh_to_load.id_index));
				glBindBuffer(GL_ARRAY_BUFFER, mesh_to_load.id_index);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_to_load.num_index, mesh_to_load.index, GL_STATIC_DRAW);
			}

			if (current_mesh->HasNormals())
			{
				mesh_to_load.num_normals = current_mesh->mNumVertices;
				mesh_to_load.normals = new float[mesh_to_load.num_normals * 3];
				memcpy(mesh_to_load.normals, current_mesh->mNormals, sizeof(float) * mesh_to_load.num_normals * 3);

				glGenBuffers(1, (GLuint*)&(mesh_to_load.id_normals));
				glBindBuffer(GL_ARRAY_BUFFER, mesh_to_load.id_normals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh_to_load.num_normals, mesh_to_load.normals, GL_STATIC_DRAW);
			}

			if (current_mesh->HasTextureCoords(mesh_to_load.id_uvs))
			{
				mesh_to_load.num_uvs = current_mesh->mNumVertices;
				mesh_to_load.uvs = new float[mesh_to_load.num_uvs * 2];

				for (int i = 0; i < current_mesh->mNumVertices; ++i)
				{
					memcpy(&mesh_to_load.uvs[i * 2], &current_mesh->mTextureCoords[0][i].x, sizeof(float));
					memcpy(&mesh_to_load.uvs[(i * 2) + 1], &current_mesh->mTextureCoords[0][i].y, sizeof(float));
				}

				glGenBuffers(1, (GLuint*)&(mesh_to_load.id_uvs));
				glBindBuffer(GL_ARRAY_BUFFER, mesh_to_load.id_uvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh_to_load.num_uvs, mesh_to_load.uvs, GL_STATIC_DRAW);
			}

			aiMaterial* texture = scene->mMaterials[current_mesh->mMaterialIndex];

			if (texture == nullptr)
			{
				LOG("This model has no texture or its texture cannot be found.");
				App->ui->console->AddLog("This model has no texture or its texture cannot be found.");
			}
			else
			{
				aiString texture_path;
				texture->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);

				if (texture_path.length > 0)
				{
					std::string base_path = "Models/";
					std::string final_path = texture_path.data;
					final_path.erase(0, final_path.find_last_of("\\") + 1);
					base_path += final_path;

					mesh_to_load.texture_id = GenerateTextureId(base_path.c_str());

					final_path.clear();
					base_path.clear();
				}

				LOG("Loading this mesh's texture.");
				App->ui->console->AddLog("Loading this mesh's texture.");
			}

			meshes.push_back(mesh_to_load);
			LOG("Loaded mesh with %i vertices.", mesh_to_load.num_vertex);
			LOG("Loaded mesh with %i indices.", mesh_to_load.num_index);
			LOG("Loaded mesh with %i triangles.", mesh_to_load.num_vertex / 3);
			App->ui->console->AddLog("Loaded mesh with %i vertices.", mesh_to_load.num_vertex);
			App->ui->console->AddLog("Loaded mesh with %i indices.", mesh_to_load.num_index);
			App->ui->console->AddLog("Loaded mesh with %i triangles.", mesh_to_load.num_vertex / 3);
		}
	}
	else
	{
		LOG("This node has no meshes.");
		App->ui->console->AddLog("This node has no meshes.");
	}
	
	for (int k = 0; k < children_node->mNumChildren; k++)
	{
		LoadMesh(scene, children_node->mChildren[k]);
	}
}

// Generate texture id ----------------------------------------------
uint ModuleFBXLoader::GenerateTextureId(const char* texture_path)
{
	uint result;
	ILuint tmp_id;

	ilGenImages(1, &tmp_id);
	ilBindImage(tmp_id);
	ilLoadImage(texture_path);

	result = ilutGLBindTexImage();
	
	return result;
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

bool ModuleFBXLoader::Load()
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