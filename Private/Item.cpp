// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
//3. ���Ǿ� ���۳�Ʈ�� ����� ���� ��� ��Ŭ���
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�����۸޽� ������Ʈ�� ��Ʈ ������Ʈ�� ����
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	//���Ǿ� �����ϰ� ������ �޽��� ���Ǿ� ����ġ
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

}

//���� ��� �Լ� ����
float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

//�ڻ��� ��� �Լ� ����
float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

//6. ��� ���Ͽ� ����� OnSphereOverlap �Լ��� ����
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		//�������� ������ �̸��� ������.
		const FString OtherActorName = OtherActor->GetName();
		//�������� ������ �� ������ �̸��� ����� �޼����� ���
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		//�������� ������ �̸��� ������.
		const FString OtherActorName = OtherActor->GetName();
		//�������� ������ �� ������ �̸��� ����� �޼����� ���
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, FString(TEXT("Ending overlap with")) + OtherActorName);
	}
}