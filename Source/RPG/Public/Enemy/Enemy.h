// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterTypes.h"
#include "Enemy.generated.h"

UCLASS()
class RPG_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	//AActor
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

	//IHitInterface
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	//AActor
	virtual void BeginPlay() override;

	//ABaseCharacter
	virtual void Die_Implementation() override;
	void SpawnSoul();
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual	void AttackEnd() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	//AI behavior
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 200.f;

	UPROPERTY(EditAnywhere)
	double AcceptanceRadius = 50.f;

	UPROPERTY()
	class AAIController* EnemyController;

	//Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMinTime = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMaxTime = 10.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMinTime = 0.1f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMaxTime = 0.4f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> SoulClass;
};
