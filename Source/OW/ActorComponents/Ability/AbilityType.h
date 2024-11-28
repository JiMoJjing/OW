
#pragma once

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAbilityType : uint8
{
	EAT_None = 0 UMETA(Hidden, DisplayName = "None"),
	EAT_AbilityOne = 1 << 1 UMETA(DisplayName = "AbilityOne"),
	EAT_AbilityTwo = 1 << 2 UMETA(DisplayName = "AbilityTwo"),
	EAT_AbilityThree = 1 << 3 UMETA(DisplayName = "AbilityThree"),
	EAT_Max = 1 << 7 UMETA(Hidden, DisplayName = "Max")
};
ENUM_CLASS_FLAGS(EAbilityType);