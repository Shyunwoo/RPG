// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RPGOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void URPGOverlay::SetHealthBarPercnet(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void URPGOverlay::SetStaminaBarPercnet(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void URPGOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
}

void URPGOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
