// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/RPGHUD.h"
#include "HUD/RPGOverlay.h"
#include "Items/Soul.h"
#include "Items/Tresure.h"

ARPGCharacter::ARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject < UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), FName("HeadSocket"));

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), FName("HeadSocket"));
}

void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Attributes && RPGOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		RPGOverlay->SetStaminaBarPercnet(Attributes->GetStaminaPercent());
	}
}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("EngageableTarget"));
	InitializeRPGOverlay();
}

void ARPGCharacter::InitializeRPGOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ARPGHUD* RPGHUD = Cast<ARPGHUD>(PlayerController->GetHUD());
		if (RPGHUD)
		{
			RPGOverlay = RPGHUD->GetRPGOverlay();
			if (RPGOverlay && Attributes)
			{
				RPGOverlay->SetHealthBarPercnet(Attributes->GetHealthPercent());
				RPGOverlay->SetStaminaBarPercnet(1.f);
				RPGOverlay->SetGold(0);
				RPGOverlay->SetSouls(0);
			}
		}
	}
}

void ARPGCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

bool ARPGCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ARPGCharacter::MoveForward);
		PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ARPGCharacter::MoveRight);
		PlayerInputComponent->BindAxis(FName("LookUp"), this, &ARPGCharacter::LookUp);
		PlayerInputComponent->BindAxis(FName("Turn"), this, &ARPGCharacter::Turn);

		PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ARPGCharacter::Jump);
		PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ARPGCharacter::EKeyPressed);
		PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ARPGCharacter::Attack);
		PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &ARPGCharacter::Dodge);
	}
}

void ARPGCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

float ARPGCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ARPGCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ARPGCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && RPGOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		RPGOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ARPGCharacter::AddGold(ATresure* Gold)
{
	if (Attributes && RPGOverlay)
	{
		Attributes->AddGold(Gold->GetGold());
		RPGOverlay->SetGold(Attributes->GetGold());
	}
}

void ARPGCharacter::SetHUDHealth()
{
	if (RPGOverlay && Attributes)
	{
		RPGOverlay->SetHealthBarPercnet(Attributes->GetHealthPercent());
	}
}

void ARPGCharacter::MoveForward(float Value)
{
	if (!IsUnoccupied()) return;

	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::MoveRight(float Value)
{
	if (!IsUnoccupied()) return;

	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::LookUp(float Value)
{
	if (Controller && (Value != 0.f))
	{
		AddControllerPitchInput(Value);
	}
}

void ARPGCharacter::Turn(float Value)
{
	if (Controller && (Value != 0.f))
	{
		AddControllerYawInput(Value);
	}
}

void ARPGCharacter::EKeyPressed()
{
	if (Controller)
	{
		AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
		if (OverlappingWeapon)
		{
			if (EquippedWeapon)
			{
				EquippedWeapon->Destroy();
			}
			EquipWeapon(OverlappingWeapon);
		}
		else
		{
			if (CasDisarm())
			{
				DisArm();
			}
			else if(CanArm())
			{
				Arm();
			}
		}
	}
}

void ARPGCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ARPGCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

void ARPGCharacter::Attack()
{
	Super::Attack();
	if (Controller)
	{
		if (CanAttack())
		{
			PlayAttackMontage();
			ActionState = EActionState::EAS_Attcking;
		}
	}
}

void ARPGCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;
	
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && RPGOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		RPGOverlay->SetStaminaBarPercnet(Attributes->GetStaminaPercent());
	}
}

bool ARPGCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool ARPGCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void ARPGCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ARPGCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

void ARPGCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

bool ARPGCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ARPGCharacter::CasDisarm()
{
	return CharacterState != ECharacterState::ECS_Unequipped &&
		ActionState == EActionState::EAS_Unoccupied;
}

bool ARPGCharacter::CanArm()
{
	return CharacterState == ECharacterState::ECS_Unequipped &&
		ActionState == EActionState::EAS_Unoccupied &&
		EquippedWeapon;
}

void ARPGCharacter::DisArm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ARPGCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ARPGCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ARPGCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ARPGCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ARPGCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
