
#include "VPinMAMEMenu.h"
#include "UE4_VPinMAME.h"

#include "ScopedTransaction.h"

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
		UI_COMMAND(MenuCommand1, "Options...", "Show the VPinMAME Options Dialog.", EUserInterfaceActionType::Button, FInputGesture());
	}

public:
	TSharedPtr<FUICommandInfo> MenuCommand1;
};

void VPinMAMEMenu::MapCommands()
{
	const auto& Commands = VPinMAMEMenuCommands::Get();

	CommandList->MapAction(
		Commands.MenuCommand1,
		FExecuteAction::CreateSP(this, &VPinMAMEMenu::MenuCommand1),
		FCanExecuteAction());

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
}

void VPinMAMEMenu::MenuCommand1()
{
	FUE4_VPinMAMEModule::Get().GetPinMAME()->ShowOptsDialog();
}

#undef LOCTEXT_NAMESPACE