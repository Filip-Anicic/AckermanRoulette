#pragma once
#include "AckermanRouletteStyle.h"

class FAckermanRouletteCommands : public TCommands<FAckermanRouletteCommands>
{
public:

	FAckermanRouletteCommands() : TCommands<FAckermanRouletteCommands>(TEXT("AckermanRoulette"),
		NSLOCTEXT("Contexts", "AckermanRoulette", "AckermanRoulette Plugin"),
		NAME_None, FAckermanRouletteStyle::GetStyleSetName())
	{}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> PluginAction;
};
