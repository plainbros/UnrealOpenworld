


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
	OverlapSphere->SetupAttachment(RootComponent); // ������ ��Ʈ ������Ʈ�� ���� (�ʿ信 ���� �ٸ� ������Ʈ�� ���� �� ����)
	OverlapSphere->SetSphereRadius(100.f); // �ݸ��� �ݰ� ����
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // ���� ���� �ݸ��� Ȱ��ȭ (������ �浹 ����, �������� ����)
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore); // ��� ä�ο� ���� ���� ����
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn ä�ο� ���ؼ��� ������ ����

	//��Ʈ������Ʈ�� ���� ��� OverlapSphere�� ��Ʈ������Ʈ�� ����
	if (!RootComponent)
	{
		RootComponent = OverlapSphere;
	}
}

void AItemClass::BeginPlay()
{
	Super::BeginPlay();

	CurrentLocation = GetActorLocation();
	if (OverlapSphere) //Nullptr Ȯ��
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

//����� �׽�Ʈ