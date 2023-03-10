// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPG_API ARPGCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ARPGCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void Jump() override;
	bool IsUnoccupied();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetOverlappingItem(class AItem* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;
	virtual void AddGold(class ATresure* Gold) override;

protected:
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void EKeyPressed();
	virtual void Attack() override;
	void Dodge();

	//Combat
	void EquipWeapon(AWeapon* Weapon);
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	bool CasDisarm();
	bool CanArm();
	void DisArm();
	void Arm();
	void PlayEquipMontage(FName SectionName);
	virtual void Die_Implementation() override;
	bool HasEnoughStamina();
	bool IsOccupied();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hair)
	class UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hair)
	UGroomComponent* Eyebrows;

private:
	void InitializeRPGOverlay();
	void SetHUDHealth();

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

	UPROPERTY()
	class URPGOverlay* RPGOverlay;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
