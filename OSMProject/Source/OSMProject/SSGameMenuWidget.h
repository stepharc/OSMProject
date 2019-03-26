// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class OSMPROJECT_API SSGameMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSGameMenuWidget)
	{}

	SLATE_ARGUMENT(TWeakObjectPtr<class AViewerHUD>, OwnerHUDArg);

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
private:
	TWeakObjectPtr<class AViewerHUD> OwnerHUD;
	TSharedRef<SWidget> CreateCheckBox(const FText& CheckBoxText);
	ECheckBoxState HandleCheckBoxChecked() const;
	void HandleCheckBoxCheckedStateChanged(ECheckBoxState NewState);
};
