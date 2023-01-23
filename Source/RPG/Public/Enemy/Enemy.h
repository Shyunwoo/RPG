// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class RPG_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	//Play animation function
	void PlayHitReactMontage(const FName& SectionName);

private:	
	//Animation montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* HitReactMontage;

};
