// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AckermanRouletteToolSettings.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class ACKERMANROULETTEEDITOR_API SAckermanRouletteWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAckermanRouletteWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	TSharedPtr<SButton> SpinButton;
	
private:
	UAckermanRouletteToolSettings* CustomSettingsObject = nullptr;

public:
	UAckermanRouletteToolSettings* GetCustomSettings() const;
};
