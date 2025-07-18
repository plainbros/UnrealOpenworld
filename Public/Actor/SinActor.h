// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SinActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class MOONSEONGHAE_API ASinActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASinActor();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sin Movement")
	float Amplitude = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sin Movement")
	float TimeConstant = 5.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sin Movement")
	float RunningTime;
	UFUNCTION(BlueprintPure)
	float TransformSin();
	UFUNCTION(BlueprintPure)
	float TransformCos();
	UPROPERTY(VisibleAnywhere)
	FVector CurrentLocation;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
};
