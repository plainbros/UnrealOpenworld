// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ItemClass.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class MOONSEONGHAE_API AWeapon : public AItemClass //이 구문에서 이파일은 ItemClass 를 상속 받았다는것을 알수 있음
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
