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
	/*AViewerHUD* hud = OwnerHUD.Get();
	auto map = hud->getCatsSubcatsNodes();*/

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
				[
					SNew(STextBlock)
						.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Verdana.ttf"), 18))
						.Text(LOCTEXT("SCheckBoxLabel", "SCheckBox"))
				]

				+ SGridPanel::Slot(0, 2)
					.HAlign(HAlign_Center)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
						.AutoHeight()
					[
						CreateCheckBox(LOCTEXT("SCheckBoxItemLabel01", "Option 1"))
					]

					+ SVerticalBox::Slot()
						.AutoHeight()
					[
						CreateCheckBox(LOCTEXT("SCheckBoxItemLabel02", "Option 2"))
					]

					+ SVerticalBox::Slot()
						.AutoHeight()
					[
						CreateCheckBox(LOCTEXT("SCheckBoxItemLabel03", "Option 3"))
					]
				]
			]
		]
	];
}

TSharedRef<SWidget> SSGameMenuWidget::CreateCheckBox(const FText& CheckBoxText)
{
	return SNew(SCheckBox)
		.ClickMethod(EButtonClickMethod::DownAndUp)
		//.IsChecked(this, &SSGameMenuWidget::HandleCheckBoxChecked)
		.OnCheckStateChanged(this, &SSGameMenuWidget::HandleCheckBoxCheckedStateChanged)
		[
			SNew(STextBlock)
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Verdana.ttf"), 18))
			.Text(CheckBoxText)
		];
}

ECheckBoxState SSGameMenuWidget::HandleCheckBoxChecked() const
{
	return ECheckBoxState::Checked;
}

void SSGameMenuWidget::HandleCheckBoxCheckedStateChanged(ECheckBoxState NewState)
{
	if (NewState == ECheckBoxState::Checked) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Checked."));
	}
	else { //Unchecked
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Unchecked."));
	}
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
