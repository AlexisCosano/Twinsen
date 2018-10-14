#include "Console.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

Console::Console()
{
	ClearLog();
	memset(input_buffer, 0, sizeof(input_buffer));
	history_position = -1;
	commands.push_back("HELP");
	commands.push_back("history");
	commands.push_back("CLEAR");
	commands.push_back("CLASSIFY");
	AddLog("Add a model to see the loading process.");
}

Console::~Console()
{}

bool Console::Start()
{
	active = true;
	return true;
}

update_status Console::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool Console::CleanUp()
{
	ClearLog();
	for (int i = 0; i < history.Size; i++)
		free(history[i]);

	return true;
}

void Console::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	items.push_back(Strdup(buf));
	scroll_to_bottom = true;
}

void Console::ExecuteCommand(const char * command_line)
{
	AddLog("# %s\n", command_line);

	history_position = -1;
	for (int i = history.Size - 1; i >= 0; i--)
		if (Stricmp(history[i], command_line) == 0)
		{
			free(history[i]);
			history.erase(history.begin() + i);
			break;
		}
	history.push_back(Strdup(command_line));

	if (Stricmp(command_line, "CLEAR") == 0)
	{
		ClearLog();
	}
	else if (Stricmp(command_line, "HELP") == 0)
	{
		AddLog("commands:");
		for (int i = 0; i < commands.Size; i++)
			AddLog("- %s", commands[i]);
	}
	else if (Stricmp(command_line, "history") == 0)
	{
		int first = history.Size - 10;
		for (int i = first > 0 ? first : 0; i < history.Size; i++)
			AddLog("%3d: %s\n", i, history[i]);
	}
	else
	{
		AddLog("Unknown command: '%s'\n", command_line);
	}
}

int Console::TextEditCallbackStub(ImGuiTextEditCallbackData * data)
{
	Console* console = (Console*)data->UserData;
	return console->TextEditCallback(data);
}

int Console::TextEditCallback(ImGuiTextEditCallbackData * data)
{
	Console* console = (Console*)data->UserData;
	return console->TextEditCallback(data);
}

void Console::ClearLog()
{
	for (int i = 0; i < items.Size; i++)
		free(items[i]);
	items.clear();
	scroll_to_bottom = true;
}

void Console::Draw(const char* title)
{
	ImGui::Begin(title);

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (ImGui::SmallButton("Clear")) ClearLog(); ImGui::SameLine();
	bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
	if (ImGui::SmallButton("Scroll to bottom")) scroll_to_bottom = true;

	ImGui::Separator();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	static ImGuiTextFilter filter;
	ImGui::PopStyleVar();
	ImGui::Separator();

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) ClearLog();
		ImGui::EndPopup();
	}
	
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	if (copy_to_clipboard)
		ImGui::LogToClipboard();
	for (int i = 0; i < items.Size; i++)
	{
		const char* item = items[i];
		if (!filter.PassFilter(item))
			continue;
		ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(item);
		ImGui::PopStyleColor();
	}
	if (copy_to_clipboard)
		ImGui::LogFinish();
	if (scroll_to_bottom)
		ImGui::SetScrollHere();

	scroll_to_bottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
		ImGui::SetKeyboardFocusHere(-1);

	ImGui::End();
}