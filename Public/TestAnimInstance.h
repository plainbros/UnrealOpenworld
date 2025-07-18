#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimTest.h" // 1단계에서 만든 Enum 헤더를 포함합니다.
#include "TestAnimInstance.generated.h"

UCLASS()
class UTestAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // AnimGraph에서 사용할 수 있도록 Enum 변수를 노출시킵니다.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
    ETestEnum TestState;
};