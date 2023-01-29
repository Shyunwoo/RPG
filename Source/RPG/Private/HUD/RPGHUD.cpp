// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RPGHUD.h"
#include "HUD/RPGOverlay.h"

void ARPGHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && RPGOverlayWidget)
		{
			RPGOverlay = CreateWidget<URPGOverlay>(Controller, RPGOverlayWidget);
			RPGOverlay->AddToViewport();
		}
	}
}
