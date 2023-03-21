#include "AckermanRouletteCommands.h"

#define LOCTEXT_NAMESPACE "FAckermanRouletteModule"

void FAckermanRouletteCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Ackerman's Roulette", "Open Ackerman's Roulette window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE