#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ElsPatternTypes.generated.h"

namespace ModAPI::ELS
{
	inline FString ColorToHex(const FColor& Color)
	{
		return FString::Printf(TEXT("rgb(%d, %d, %d)"), Color.R, Color.G, Color.B);
	}
}

USTRUCT(BlueprintType)
struct FElsFlasherLightPattern
{
	GENERATED_BODY()

public:
	FElsFlasherLightPattern() = default;
	explicit FElsFlasherLightPattern(FString InOverrideKey, FColor InColor) :
		OverrideKey(InOverrideKey)
	{
		ColorOverrideValue = ModAPI::ELS::ColorToHex(InColor);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString OverrideKey = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ColorOverrideValue = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> FrequencyOverrideValues = {};
};

USTRUCT(BlueprintType)
struct FElsFlasherLightPatternConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FlasherEmissionLevel = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FElsFlasherLightPattern> FlasherPatterns = {};
};

USTRUCT(BlueprintType)
struct FElsTurretLightPattern
{
	GENERATED_BODY()

public:
	FElsTurretLightPattern() = default;
	explicit FElsTurretLightPattern(FString InOverrideKey, FColor InColor) :
		OverrideKey(InOverrideKey)
	{
		ColorOverrideValue = ModAPI::ELS::ColorToHex(InColor);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString OverrideKey = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ColorOverrideValue = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int32> FrequencyOverrideValues = {};
};

USTRUCT(BlueprintType)
struct FElsLightPatternManifest
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FElsFlasherLightPatternConfig Flashers = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FElsTurretLightPattern> TurretLightPatterns = {};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPatternKeyFrameValueChanged, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPatternKeyFrameHighlightChanged, int32, HighlightCycle);

UCLASS(Blueprintable, BlueprintType)
class UPatternKeyFrameObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPatternKeyFrameValueChanged OnPatternKeyFrameValueChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPatternKeyFrameHighlightChanged OnPatternKeyFrameHighlightChanged;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 KeyFrameNumber = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Value = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 HighlightedInCycle = -1;

	UPROPERTY()
	TArray<UUserWidget*> WidgetRepresentations;

	UFUNCTION(BlueprintCallable)
	int32 RepresentNextCycle(UUserWidget* Widget)
	{
		return WidgetRepresentations.Add(Widget);
	}

	UFUNCTION(BlueprintCallable)
	void SetValue(int32 NewValue)
	{
		Value = NewValue;
		OnPatternKeyFrameValueChanged.Broadcast(NewValue);
	}

	UFUNCTION(BlueprintCallable)
	void SetBoolValue(bool bNewValue)
	{
		SetValue(bNewValue);
	}

	UFUNCTION(BlueprintCallable)
	void SetHighlighted(int32 Cycle = 0)
	{
		HighlightedInCycle = Cycle;
		OnPatternKeyFrameHighlightChanged.Broadcast(HighlightedInCycle);
	}
};

USTRUCT(BlueprintType)
struct FElsRecordingTrack
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> LightNames = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int32> KeyFrameValues = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor Color = FColor::White;
};