// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SinActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASinActor::ASinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

}

// Called when the game starts or when spawned
void ASinActor::BeginPlay()
{
	Super::BeginPlay();
	CurrentLocation = GetActorLocation();
}

// Called every frame
void ASinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	FVector NewLocation = CurrentLocation;
	CurrentLocation.Z += TransformSin();
	SetActorLocation(NewLocation);

}

float ASinActor::TransformSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float ASinActor::TransformCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}