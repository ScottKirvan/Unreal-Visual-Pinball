
#include "VPinMAMEMenu.h"
#include "UE4_VPinMAME.h"

#define LOCTEXT_NAMESPACE "VPinMAMEMenu"

class VPinMAMEMenuCommands : public TCommands<VPinMAMEMenuCommands>
{
public:

	VPinMAMEMenuCommands()
		: TCommands<VPinMAMEMenuCommands>(
		TEXT("VPinMAMEMenu"), // Context name for fast lookup
		FText::FromString("Example Menu tool"), // Context name for displaying
		NAME_None,	 // No parent context
		FEditorStyle::GetStyleSetName() // Icon Style Set
		)
	{
	}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(MenuCommand1, "Options...", "Show the VPinMAME Options Dialog.", EUserInterfaceActionType::Button, FInputChord());
		UI_COMMAND(MenuCommand2, "Paths...", "Show the VPinMAME Options Dialog.", EUserInterfaceActionType::Button, FInputChord());
		UI_COMMAND(MenuCommand3, "About...", "Show the VPinMAME Options Dialog.", EUserInterfaceActionType::Button, FInputChord());
	}

public:
	TSharedPtr<FUICommandInfo> MenuCommand1;
	TSharedPtr<FUICommandInfo> MenuCommand2;
	TSharedPtr<FUICommandInfo> MenuCommand3;
};

void VPinMAMEMenu::MapCommands()
{
	const auto& Commands = VPinMAMEMenuCommands::Get();

	CommandList->MapAction( Commands.MenuCommand1, FExecuteAction::CreateSP(this, &VPinMAMEMenu::MenuCommand1), FCanExecuteAction());
	CommandList->MapAction( Commands.MenuCommand2, FExecuteAction::CreateSP(this, &VPinMAMEMenu::MenuCommand2), FCanExecuteAction());
	CommandList->MapAction( Commands.MenuCommand3, FExecuteAction::CreateSP(this, &VPinMAMEMenu::MenuCommand3), FCanExecuteAction());

}

void VPinMAMEMenu::OnStartupModule()
{
	CommandList = MakeShareable(new FUICommandList);
	VPinMAMEMenuCommands::Register();
	MapCommands();
	FUE4_VPinMAMEModule::Get().AddMenuExtension(
		FMenuExtensionDelegate::CreateRaw(this, &VPinMAMEMenu::MakeMenuEntry),
		FName("Section_1"),
		CommandList);
}

void VPinMAMEMenu::OnShutdownModule()
{
	VPinMAMEMenuCommands::Unregister();
}

void VPinMAMEMenu::MakeMenuEntry(FMenuBuilder &menuBuilder)
{
	menuBuilder.AddMenuEntry(VPinMAMEMenuCommands::Get().MenuCommand1);
	menuBuilder.AddMenuEntry(VPinMAMEMenuCommands::Get().MenuCommand2);
	menuBuilder.AddMenuEntry(VPinMAMEMenuCommands::Get().MenuCommand3);
}

void VPinMAMEMenu::MenuCommand1() { FUE4_VPinMAMEModule::Get().GetPinMAME()->ShowOptsDialog(); }
void VPinMAMEMenu::MenuCommand2() { FUE4_VPinMAMEModule::Get().GetPinMAME()->ShowPathsDialog(); }
void VPinMAMEMenu::MenuCommand3() { FUE4_VPinMAMEModule::Get().GetPinMAME()->ShowAboutDialog(); }

#undef LOCTEXT_NAMESPACE