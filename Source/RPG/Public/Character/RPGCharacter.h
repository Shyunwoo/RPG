// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPG_API ARPGCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ARPGCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void EKeyPressed();
	virtual void Attack() override;

	//Combat
	void EquipWeapon(AWeapon* Weapon);
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	bool CasDisarm();
	bool CanArm();
	void DisArm();
	void Arm();
	void PlayEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hair)
	class UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hair)
	UGroomComponent* Eyebrows;

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	//Components
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	class AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
