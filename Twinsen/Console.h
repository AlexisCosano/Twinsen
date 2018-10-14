#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Globals.h"
#include "imGUI\imgui.h"

class Console
{
public:
	Console();
	~Console();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void AddLog(const char*, ...) IM_FMTARGS(2);
	void ClearLog();

	void Draw(const char* title);

	void ExecuteCommand(const char* command_line);

	static int TextEditCallbackStub(ImGuiTextEditCallbackData* data);
	int TextEditCallback(ImGuiTextEditCallbackData* data);
	bool active = true;
private:

	ImVector<const char*> commands;
	ImVector<char*>       history;
	ImVector<char*>       items;

	char                  input_buffer[256];
	bool                  scroll_to_bottom;
	int                   history_position;
	
	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
};























#endif
