// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UEAssetStyle.h"

class FUEAssetCommands : public TCommands<FUEAssetCommands>
{
public:

	FUEAssetCommands()
		: TCommands<FUEAssetCommands>(TEXT("UEAsset"), NSLOCTEXT("Contexts", "UEAsset", "UEAsset Plugin"), NAME_None, FUEAssetStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};