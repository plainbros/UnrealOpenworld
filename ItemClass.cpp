


#include "Actor/ItemClass.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h" 
#include "MyCharacter.h"

AItemClass::AItemClass()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(RootComponent); // 액터의 루트 컴포넌트에 붙임 (필요에 따라 다른 컴포넌트에 붙일 수 있음)
	OverlapSphere->SetSphereRadius(100.f); // 콜리전 반경 설정
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 쿼리 전용 콜리전 활성화 (물리적 충돌 없음, 오버랩만 감지)
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 채널에 대해 응답 무시
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn 채널에 대해서만 오버랩 응답

	//루트컴포넌트가 없는 경우 OverlapSphere를 루트컴포넌트로 지정
	if (!RootComponent)
	{
		RootComponent = OverlapSphere;
	}
}

void AItemClass::BeginPlay()
{
	Super::BeginPlay();

	CurrentLocation = GetActorLocation();
	if (OverlapSphere) //Nullptr 확인
	{
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemClass::OnSphereOverlap);
		OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AItemClass::OnSphereEndOverlap);
	}

}

void AItemClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Sin Movement Set Location
	RunningTime += DeltaTime;
	FVector NewLocation = CurrentLocation;
	CurrentLocation.Z += TransformSin();
	SetActorLocation(NewLocation);

}

float AItemClass::TransformSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItemClass::TransformCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}


void AItemClass::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->SetOverlappingItem(this);
	}
}


void AItemClass:: OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->SetOverlappingItem(nullptr);
		}
	}
	
}

//깃허브 테스트