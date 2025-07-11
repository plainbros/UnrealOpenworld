// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"       // UEnhancedInputComponent 사용
#include "EnhancedInputSubsystems.h"    // UEnhancedInputLocalPlayerSubsystem 사용
#include "Engine/LocalPlayer.h"         // ULocalPlayer 사용
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
    }
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
    // 입력 값은 FVector2D 타입입니다.
    FVector2D MovementVector = Value.Get<FVector2D>();

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