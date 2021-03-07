#pragma once

#include "UE4_VPinMAME.h"

class VPinMAMEMenu : public IVPinMAMEModuleListenerInterface, public TSharedFromThis<VPinMAMEMenu>
{
public:
	virtual ~VPinMAMEMenu() {}

	virtual void OnStartupModule() override;
	virtual void OnShutdownModule() override;

	void MakeMenuEntry(FMenuBuilder &menuBuilder);


protected:
	TSharedPtr<FUICommandList> CommandList;

	void MapCommands();

	//************************
	// UI Command functions
	void MenuCommand1();
};