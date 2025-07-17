// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Character/CharacterType.h"
#include "MyCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItemClass;


UCLASS()
class MOONSEONGHAE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

protected:
	virtual void BeginPlay() override;

private:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(VisibleInstanceOnly)
	AItemClass* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	//추가로 몽타주 변수를 추가 하면 여러개의 몽타주 블루프린트에서 할당 가능 함
	//액션 몽타주를 캐릭터 블루프린트에 할당을 해야 하는데 그걸 위한 변수
	//추가로 여러 가지 몽타주를 할당 하고 싶으면 몽타주 변수를 추가로 더 만들면 됨
	UAnimMontage* AttackMontage;

public:
	FORCEINLINE void SetOverlappingItem(AItemClass* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }


protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void EPressed(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();
};
