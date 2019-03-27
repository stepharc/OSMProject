// Fill out your copyright notice in the Description page of Project Settings.

#include "SSGameMenuWidget.h"
#include "Engine/GameEngine.h"
#include "SlateOptMacros.h"
#include "SScrollBox.h"
#include "SGridPanel.h"
#include "STextBlock.h"
#include "SCheckBox.h"
#include "ViewerHUD.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "SSGameMenuWidget"

void SSGameMenuWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUDArg;
	AViewerHUD* hud = OwnerHUD.Get();

	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SScrollBox)

			+ SScrollBox::Slot()
			.Padding(5.0f)
			[
				SNew(SGridPanel)
					.FillColumn(0, 0.33f)
					.FillColumn(1, 0.33f)
					.FillColumn(2, 0.33f)

				+ SGridPanel::Slot(0, 0)
				[
					SNew(STextBlock)
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Verdana.ttf"), 18))
						.Text(LOCTEXT("SFirstColumnLabel", "NODES"))
				]

				+ SGridPanel::Slot(1, 0)
				[
					SNew(STextBlock)
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Verdana.ttf"), 18))
						.Text(LOCTEXT("SSecondColumnLabel", "WAYS"))
				]

				+ SGridPanel::Slot(2, 0)
				[
					SNew(STextBlock)
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Verdana.ttf"), 18))
						.Text(LOCTEXT("SThirdColumnLabel", "RELATIONS"))
				]

				+ SGridPanel::Slot(0, 1)
					.HAlign(HAlign_Center)
				[
					CreateSubcategoryCheckList(hud->getCatsSubcatsNodes())
				]

				+ SGridPanel::Slot(1, 1)
					.HAlign(HAlign_Center)
				[
					CreateSubcategoryCheckList(hud->getCatsSubcatsWays())
				]

				+ SGridPanel::Slot(2, 1)
					.HAlign(HAlign_Center)
				[
					CreateSubcategoryCheckList(hud->getCatsSubcatsRelations())
				]
			]
		]
	];
}

TSharedRef<SVerticalBox> SSGameMenuWidget::CreateSubcategoryCheckList(std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> hashmap) {
	TSharedRef<SVerticalBox> Box = SNew(SVerticalBox);
	for (auto it = hashmap.begin(); it != hashmap.end(); it++) {
		std::string category = it->first;
		for (int i = 0; i < it->second.size(); i++) {
			std::string subcategory = it->second[i].first;
			std::string textCheckBox = category + std::string(" - ") + subcategory;
			FText text = FText::FromString(FString(textCheckBox.c_str()));
			Box->AddSlot()
				.AutoHeight()
				[
					CreateCheckBox(text, category, subcategory, hashmap)
				];
		}
	}
	return Box;
}

TSharedRef<SWidget> SSGameMenuWidget::CreateCheckBox(const FText& CheckBoxText, std::string cat, std::string subcat, std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> hashmap)
{
	return SNew(SCheckBox)
		.IsEnabled(true)
		.OnCheckStateChanged(this, &SSGameMenuWidget::HandleCheckBoxCheckedStateChanged, cat, subcat, hashmap)
		[
			SNew(STextBlock)
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Verdana.ttf"), 20))
			.Text(CheckBoxText)
		];
}

void SSGameMenuWidget::HandleCheckBoxCheckedStateChanged(ECheckBoxState NewState, std::string cat, std::string subcat, std::map<std::string, std::vector<std::pair<std::string, AViewerHUD::DrawStatus>>> hashmap)
{
	AViewerHUD* hud = OwnerHUD.Get();
	if (NewState == ECheckBoxState::Checked) {
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Checked. : " + FString(cat.c_str()) + " " + FString(subcat.c_str()));
		hud->requestShowOSMElement(cat, subcat, hashmap);
	}
	else { //Unchecked
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Unchecked."));
		hud->requestHideOSMElement(cat, subcat, hashmap);
	}
}

void SSGameMenuWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// Call parent implementation
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	//can do things here every tick
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
