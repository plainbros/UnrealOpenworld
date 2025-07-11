// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"       // UEnhancedInputComponent ���
#include "EnhancedInputSubsystems.h"    // UEnhancedInputLocalPlayerSubsystem ���
#include "Engine/LocalPlayer.h"         // ULocalPlayer ���
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
    // ī�޶� ��(��������) ����
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent()); // ĸ�� ������Ʈ�� ����
    SpringArm->TargetArmLength = 300.0f; // ī�޶�� ĳ���� ������ �Ÿ�
    SpringArm->bUsePawnControlRotation = true; // ��Ʈ�ѷ��� ȸ��(���콺 ������)�� �������Ͽ� ����

    // �ȷο� ī�޶� ����
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // �������� ���� ����
    FollowCamera->bUsePawnControlRotation = false; // ī�޶�� ���������� ȸ���� ���󰡹Ƿ�, ���� ��Ʈ�ѷ� ȸ���� ���� �ʵ��� ��

}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            // ������ ���� �⺻ ���� ���ؽ�Ʈ�� �߰��մϴ�.
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

    // PlayerInputComponent�� UEnhancedInputComponent�� ĳ�����մϴ�.
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // JumpAction�� ���ε��մϴ�.
        // ETriggerEvent::Triggered: ������ ����
        // ETriggerEvent::Completed: ���� ����
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartJump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::StopJump);

        // MoveAction�� ���ε��մϴ�.
        // ETriggerEvent::Triggered: Ű�� �����ִ� ���� �� ������ �߻�
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

        // LookAction�� ���ε��մϴ�.
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
    }
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
    // �Է� ���� FVector2D Ÿ���Դϴ�.
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // ĳ������ ������� ��������� �̵��� �߰��մϴ�.
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
    // �Է� ���� FVector2D Ÿ���Դϴ�.
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // ��Ʈ�ѷ��� Yaw, Pitch ȸ���� �߰��մϴ�.
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y * -1.0f);
    }
}

void AMyCharacter::StartJump(const FInputActionValue& Value)
{
    // ACharacter�� ����� Jump �Լ��� ȣ���մϴ�.
    Jump();
}

void AMyCharacter::StopJump(const FInputActionValue& Value)
{
    // ACharacter�� ����� StopJumping �Լ��� ȣ���մϴ�.
    StopJumping();
}