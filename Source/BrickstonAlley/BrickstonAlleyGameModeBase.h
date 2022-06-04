// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Data/ElsPatternTypes.h"

#include "GameFramework/GameModeBase.h"
#include "BrickstonAlleyGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecordingTracksChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyKeyFrameChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedLightsChanged, const TArray<FName>&, SelectedLightNames);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedRecordingTrackChanged, FName, TrackName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlaybackFrameChanged, int32, CurrentFrame, int32, CurrentCycle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTrackColorOrEmissionChanged, FName, TrackName, FColor, Color, float, Emission);

UCLASS()
class BRICKSTONALLEY_API ABrickstonAlleyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnRecordingTracksChanged OnRecordingTracksChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSelectedRecordingTrackChanged OnSelectedRecordingTrackChanged;

	UPROPERTY(BlueprintAssignable)
	FOnTrackColorOrEmissionChanged OnTrackColorOrEmissionChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSelectedLightsChanged OnSelectedLightsChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAnyKeyFrameChanged OnAnyKeyFrameChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlaybackFrameChanged OnPlaybackFrameChanged;

	UFUNCTION(BlueprintPure)
	float GetElsFlasherLightPatternDuration() const;

	UFUNCTION(BlueprintPure)
	int32 GetElsFlasherLightPatternFPS() const;

	UFUNCTION(BlueprintCallable)
	FName CreateNewRecordingTrack(FElsRecordingTrack& OutTrack);

	UFUNCTION(BlueprintCallable)
	FName DuplicateSelectedRecordingTrack(FElsRecordingTrack& OutTrack);

	UFUNCTION(BlueprintPure)
	bool GetRecordingTrack(FName TrackName, FElsRecordingTrack& OutTrack) const;

	UFUNCTION(BlueprintPure)
	TMap<FName, FElsRecordingTrack> GetAllRecordingTracks() const;

	UFUNCTION(BlueprintCallable)
	void SetSelectedRecordingTrack(FName TrackName);

	UFUNCTION(BlueprintPure)
	FName GetSelectedRecordingTrack() const;

	UFUNCTION(BlueprintPure)
	TArray<FName> GetNonSelectedRecordingTracks() const;

	UFUNCTION(BlueprintCallable)
	void DeleteRecordingTrack(FName TrackName);

	UFUNCTION(BlueprintCallable)
	void ApplyKeyFramesToRecordingTrack(FName TrackName);

	UFUNCTION(BlueprintCallable)
	void LoadRecordingTrackIntoKeyFrames(FName TrackName);

	UFUNCTION(BlueprintCallable)
	void ClearRecordingData(bool bClearKeyFrames, bool bClearSelectedLights);

	UFUNCTION(BlueprintPure)
	TArray<UPatternKeyFrameObject*> GetKeyFrames() const;

	UFUNCTION(BlueprintCallable)
	void SetLightSelected(FName LightName, bool bSetSelected);

	UFUNCTION(BlueprintCallable)
	void SetSelectedLights(TArray<FName> NewSelectedLightNames);

	UFUNCTION(BlueprintPure)
	TArray<FName> GetSelectedLights() const;

	UFUNCTION(BlueprintPure)
	TArray<FName> GetTrackNamesThatSelectLight(FName LightName) const;

	UFUNCTION(BlueprintPure)
	bool IsLightSelected(FName LightName) const;

	UFUNCTION(BlueprintCallable)
	void SetDemoCar(AActor* NewDemoCarActor);

	UFUNCTION(BlueprintPure)
	AActor* GetDemoCar() const;

	UFUNCTION(BlueprintPure)
	static FColor MakeColorFromHex(const FString& HexString);

	UFUNCTION(BlueprintPure)
	static FString MakeHexStringFromColor(const FColor& Color);

	UFUNCTION(BlueprintCallable)
	void SetTrackColorAndEmission(FName TrackName, FColor Color, float Emission);

	UFUNCTION(BlueprintPure)
	float GetEmissionLevel() const;

	UFUNCTION(BlueprintCallable)
	bool ExportElsLightPatternsToFile(TArray<FString>& OutErrors, FString& OutFilePath);

	UFUNCTION(BlueprintCallable)
	void StopPlayback();

	UFUNCTION(BlueprintCallable)
	void PlaybackNextFrame();

	UFUNCTION(BlueprintPure)
	void GetCurrentPlaybackFrame(int32& OutFrame, int32& OutCycle) const;

	UFUNCTION(BlueprintCallable)
	void SetDefaultLightIntensity(float Intensity);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, FElsRecordingTrack> ElsRecordingTracksByName = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UPatternKeyFrameObject*> KeyFrames = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FName> SelectedLightNames = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName SelectedTrackName = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* DemoCarActor = nullptr;

	// 0..1
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FlasherEmissionLevel = 0.1f;

	// 0..100
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DefaultLightIntensity = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentPlaybackFrame = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentPlaybackCycle = 0;

private:
	UFUNCTION()
	void NotifyAnyKeyFrameChanged(int32 NewValue);

	FName MakeUniqueElsTrackName() const;
};
