// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewerHUD.h"
#include <map>
#include <vector>
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
	TSharedRef<SWidget> CreateCheckBox(const FText& CheckBoxText, std::string cat, std::string subcat, std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> hashmap);
	TSharedRef<SVerticalBox> CreateSubcategoryCheckList(std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> hashmap);
	void HandleCheckBoxCheckedStateChanged(ECheckBoxState NewState, std::string cat, std::string subcat, std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> hashmap);
	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);
};
