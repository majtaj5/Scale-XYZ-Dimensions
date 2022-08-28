#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>
#include <iostream>
#include <format>
#include <string>
#include <Fusion/BRep/BRepBody.h>


using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

Ptr<Application> app;
Ptr<UserInterface> ui;


// CommandCreated event handler.
class ScaleCommandCreatedEventHandler : public adsk::core::CommandCreatedEventHandler
{
public:
	void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override
	{
		app->log("crev");
	}

private:
	
}_cmdCreated;


bool checkReturn(Ptr<Base> returnObj)
{
	if (returnObj)
		return true;
	else
		if (app && ui)
		{
			std::string errDesc;
			app->getLastError(&errDesc);
			ui->messageBox(errDesc);
			return false;
		}
		else
			return false;

	}

extern "C" XI_EXPORT bool run(const char* context)
{
	
	app = Application::get();
	if (!app)
		return false;

	ui = app->userInterface();
	if (!ui)
		return false;


	// Create a command definition and add a button to the CREATE panel.
	Ptr<CommandDefinition> cmdDef = ui->commandDefinitions()->addButtonDefinition("ScaleXYZCPPAddIn", "Scale XYZ Dimensions", "a Mill relief component", "Resources");
	if (!checkReturn(cmdDef))
		return false;

	Ptr<ToolbarPanel> createPanel = ui->allToolbarPanels()->itemById("SolidModifyPanel");
	if (!checkReturn(createPanel))
		return false;

	Ptr<CommandControl> scaleXYZButton = createPanel->controls()->addCommand(cmdDef);
	if (!checkReturn(scaleXYZButton))
		return false;

	// Connect to the Command Created event.
	Ptr<CommandCreatedEvent> commandCreatedEvent = cmdDef->commandCreated();
	commandCreatedEvent->add(&_cmdCreated);
	if (!checkReturn(commandCreatedEvent))
		return false;

	return true;
}

extern "C" XI_EXPORT bool stop(const char* context)
{
	Ptr<ToolbarPanel> createPanel = ui->allToolbarPanels()->itemById("SolidModifyPanel");
	if (!checkReturn(createPanel))
		return false;

	Ptr<CommandControl> scaleXYZ = createPanel->controls()->itemById("ScaleXYZCPPAddIn");
	if (checkReturn(scaleXYZ))
		scaleXYZ->deleteMe();

	Ptr<CommandDefinition> cmdDef = ui->commandDefinitions()->itemById("ScaleXYZCPPAddIn");
	if (checkReturn(cmdDef))
		cmdDef->deleteMe();

	return true;
}


#ifdef XI_WIN

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif // XI_WIN
