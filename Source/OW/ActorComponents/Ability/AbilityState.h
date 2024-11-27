
#pragma once

UENUM(BlueprintType)
enum class EAbilityState : uint8
{
	EAS_Available = 0 UMETA(DisplayName = "Available"),
	EAS_Active = 1 UMETA(DisplayName = "Active"),
	EAS_Cooldown = 2 UMETA(DisplayName = "Cooldown"),
	EAS_Unavailable = 3 UMETA(DisplayName = "Unavailable")
};