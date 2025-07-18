// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"       // UEnhancedInputComponent 사용
#include "EnhancedInputSubsystems.h"    // UEnhancedInputLocalPlayerSubsystem 사용
#include "Engine/LocalPlayer.h"         // ULocalPlayer 사용
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/ItemClass.h"
#include "Actor/Weapon/Weapon.h"
#include "Animation/AnimMontage.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
    // 카메라 붐(스프링암) 생성
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent()); // 캡슐 컴포넌트에 부착
    SpringArm->TargetArmLength = 300.0f; // 카메라와 캐릭터 사이의 거리
    SpringArm->bUsePawnControlRotation = true; // 컨트롤러의 회전(마우스 움직임)을 스프링암에 적용

    // 팔로우 카메라 생성
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // 스프링암 끝에 부착
    FollowCamera->bUsePawnControlRotation = false; // 카메라는 스프링암의 회전을 따라가므로, 직접 컨트롤러 회전을 받지 않도록 함

}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            // 위에서 만든 기본 매핑 컨텍스트를 추가합니다.
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // PlayerInputComponent를 UEnhancedInputComponent로 캐스팅합니다.
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // JumpAction을 바인딩합니다.
        // ETriggerEvent::Triggered: 누르는 순간
        // ETriggerEvent::Completed: 떼는 순간
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartJump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::StopJump);

        // MoveAction을 바인딩합니다.
        // ETriggerEvent::Triggered: 키가 눌려있는 동안 매 프레임 발생
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

        // LookAction을 바인딩합니다.
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

        EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMyCharacter::EPressed);

        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::Attack);
    }
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
    // 입력 값은 FVector2D 타입입니다.
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (ActionState == EActionState::EAS_Attacking) return;
    if (Controller != nullptr)
    {
        // 캐릭터의 전방향과 우방향으로 이동을 추가합니다.
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
    // 입력 값은 FVector2D 타입입니다.
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // 컨트롤러의 Yaw, Pitch 회전을 추가합니다.
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y * -1.0f);
    }
}

void AMyCharacter::StartJump(const FInputActionValue& Value)
{
    // ACharacter에 내장된 Jump 함수를 호출합니다.
    Jump();
}

void AMyCharacter::StopJump(const FInputActionValue& Value)
{
    // ACharacter에 내장된 StopJumping 함수를 호출합니다.
    StopJumping();
}

void AMyCharacter::Attack(const FInputActionValue& Value)
{

    //CanAttack 함수가 참일때 실행
    if (CanAttack())
    {
        PlayAttackMontage();
        ActionState = EActionState::EAS_Attacking;
        //현재 상황은 ActionState 의 상태는 EAS_Attacking 이기 때문에 다시 공격 할 수가 없는 상태임
    }
    
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Attack!!"));
}

bool AMyCharacter::CanAttack()
{
    //액션 상태가 대기중일때 && 캐릭터 상태가 무기를 착용 하고 있는 경우 함수 작동
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState != ECharacterState::ECS_Unequipped;
}

void AMyCharacter::AttackEnd()
{
    //공격이 끝나면 액션 상태를 다시 대기중으로 바꿔주는 함수
    //어택엔드 함수는 캐릭터 몽타주에서 노티파이를 생성 해서 호출 하는 방식이고
    //몽타주의 노티파이와 C++ 의 어택엔드 함수를 이어 줘야 하는데 애님블루프린트에서 두개를 연결 해줘야 함
    ActionState = EActionState::EAS_Unoccupied;
}

void AMyCharacter::EPressed(const FInputActionValue& Value)
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("1. E Key Pressed!"));
    AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
    if (OverlappingWeapon)
    {
        OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
        CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
    }
}

//리팩토링 함수
void AMyCharacter::PlayAttackMontage()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && AttackMontage)
    {
        AnimInstance->Montage_Play(AttackMontage);
        const int32 Selection = FMath::RandRange(0, 1);
        FName SectionName = FName();
        switch (Selection)
        {
        case 0:
            SectionName = FName("Attack1");
            break;
        case 1:
            SectionName = FName("Attack2");
            break;
        default:
            break;
        }
        AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
    }
}



