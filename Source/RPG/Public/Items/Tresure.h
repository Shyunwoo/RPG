// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Tresure.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ATresure : public AItem
{
	GENERATED_BODY()
	
public:


protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold = 5;

public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
