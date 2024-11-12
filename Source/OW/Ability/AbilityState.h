
#pragma once

UENUM(BlueprintType)
enum class EAbiltiyState : uint8
{
	EAS_Ready = 0 UMETA(DisplayName = "Ready"),
	EAS_Active = 1 UMETA(DisplayName = "Active"),
	EAS_Cooldown = 2 UMETA(DisplayName = "Cooldown"),
	EAS_Unavailable = 3 UMETA(DisplayName = "Unavailable")
};