// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Tresure.h"
#include "Interfaces/PickupInterface.h"

void ATresure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
		if (PickupInterface)
		{
			PickupInterface->AddGold(this);

			SpawnPickupSound();
			Destroy();
		}
	}
}
