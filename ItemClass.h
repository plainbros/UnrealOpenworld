// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemClass.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class MOONSEONGHAE_API AItemClass : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemClass();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* OverlapSphere;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sin Movement")
	float Amplitude = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sin Movement")
	float TimeConstant = 5.0;
	UPROPERTY(VisibleAnywhere)
	float RunningTime;
	UPROPERTY(VisibleAnywhere)
	FVector CurrentLocation;
	UFUNCTION(BlueprintPure)
	float TransformSin();
	UFUNCTION(BlueprintPure)
	float TransformCos();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
private:

};
