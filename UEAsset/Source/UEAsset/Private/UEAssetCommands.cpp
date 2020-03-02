// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UEAssetCommands.h"

#define LOCTEXT_NAMESPACE "FUEAssetModule"

void FUEAssetCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "UEAsset", "Bring up UEAsset window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
