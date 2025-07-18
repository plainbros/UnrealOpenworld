// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
//3. 스피어 컴퍼넌트를 만들기 위한 헤더 인클루드
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//아이템메쉬 컴포넌트를 루트 컴포넌트로 지정
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	//스피어 생성하고 아이템 메쉬에 스피어 어태치
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

//사인 기능 함수 정의
float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

//코사인 기능 함수 정의
float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

//6. 헤더 파일에 선언된 OnSphereOverlap 함수의 정의
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		//오버랩된 액터의 이름을 가져옴.
		const FString OtherActorName = OtherActor->GetName();
		//엔진에서 오버랩 된 액터의 이름을 디버그 메세지로 출력
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		//오버랩된 액터의 이름을 가져옴.
		const FString OtherActorName = OtherActor->GetName();
		//엔진에서 오버랩 된 액터의 이름을 디버그 메세지로 출력
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, FString(TEXT("Ending overlap with")) + OtherActorName);
	}
}