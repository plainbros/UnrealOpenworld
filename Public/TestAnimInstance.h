#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimTest.h" // 1�ܰ迡�� ���� Enum ����� �����մϴ�.
#include "TestAnimInstance.generated.h"

UCLASS()
class UTestAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // AnimGraph���� ����� �� �ֵ��� Enum ������ �����ŵ�ϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
    ETestEnum TestState;
};