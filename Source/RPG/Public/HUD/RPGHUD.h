// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPGHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGHUD : public AHUD
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = RPG)
	TSubclassOf<class URPGOverlay> RPGOverlayWidget;

	UPROPERTY()
	URPGOverlay* RPGOverlay;

public:
	FORCEINLINE URPGOverlay* GetRPGOverlay() { return RPGOverlay; }
};
