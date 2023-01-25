// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Tresure.h"
#include "Character/RPGCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATresure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ARPGCharacter* RPGCharacter = Cast<ARPGCharacter>(OtherActor);
		if (RPGCharacter)
		{
			if (PickupSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
				Destroy();
			}
		}
	}
}
