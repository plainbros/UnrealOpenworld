#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETestEnum : uint8
{
    StateA   UMETA(DisplayName = "State A"),
    StateB   UMETA(DisplayName = "State B"),
    StateC   UMETA(DisplayName = "State C")
};