// Copyright Epic Games, Inc. All Rights Reserved.

#include "BrickstonAlleyGameModeBase.h"

#include "ImportExport/ElsModExporter.h"

float ABrickstonAlleyGameModeBase::GetElsFlasherLightPatternDuration() const
{
	float Duration = 5.f;
	//#todo actually support different values here :)
	//GConfig->GetFloat(TEXT("ELS"), TEXT("ElsFlasherLightPatternDuration"), OUT Duration, GGameIni);
	return Duration;
}

int32 ABrickstonAlleyGameModeBase::GetElsFlasherLightPatternFPS() const
{
	int32 FramesPerSecond = 30;
	//#todo actually support different values here :)
	//GConfig->GetInt(TEXT("ELS"), TEXT("ElsFlasherLightPatternFPS"), OUT FramesPerSecond, GGameIni);
	return FramesPerSecond;
}

FName ABrickstonAlleyGameModeBase::CreateNewRecordingTrack(FElsRecordingTrack& OutTrack)
{
	const int32 NumFramesTotal = GetElsFlasherLightPatternDuration() * GetElsFlasherLightPatternFPS();

	OutTrack = {};
	OutTrack.KeyFrameValues.AddDefaulted(NumFramesTotal);

	if (ElsRecordingTracksByName.Contains(SelectedTrackName))
	{
		OutTrack.Color = ElsRecordingTracksByName[SelectedTrackName].Color;
	}

	if (KeyFrames.Num() == 0)
	{
		for (int32 i = 0; i < NumFramesTotal; i++)
		{
			UPatternKeyFrameObject* NewKeyFrame = NewObject<UPatternKeyFrameObject>(this);
			NewKeyFrame->KeyFrameNumber = i;
			NewKeyFrame->OnPatternKeyFrameValueChanged.AddDynamic(this, &ThisClass::NotifyAnyKeyFrameChanged);
			KeyFrames.Add(NewKeyFrame);
		}
	}

	const FName TrackName = MakeUniqueElsTrackName();
	ElsRecordingTracksByName.Add(TrackName, OutTrack);
	OnRecordingTracksChanged.Broadcast();

	return TrackName;
}

FName ABrickstonAlleyGameModeBase::DuplicateSelectedRecordingTrack(FElsRecordingTrack& OutTrack)
{
	ApplyKeyFramesToRecordingTrack(SelectedTrackName);

	const FElsRecordingTrack SelectedTrackDuplicate = ElsRecordingTracksByName[SelectedTrackName];
	OutTrack = SelectedTrackDuplicate;
	OutTrack.LightNames.Empty();

	const FName TrackName = MakeUniqueElsTrackName();
	ElsRecordingTracksByName.Add(TrackName, OutTrack);
	OnRecordingTracksChanged.Broadcast();

	return TrackName;
}

bool ABrickstonAlleyGameModeBase::GetRecordingTrack(FName TrackName, FElsRecordingTrack& OutTrack) const
{
	if (!ElsRecordingTracksByName.Contains(TrackName))
		return false;

	OutTrack = ElsRecordingTracksByName[TrackName];
	return true;
}

TMap<FName, FElsRecordingTrack> ABrickstonAlleyGameModeBase::GetAllRecordingTracks() const
{
	return ElsRecordingTracksByName;
}

void ABrickstonAlleyGameModeBase::SetSelectedRecordingTrack(FName TrackName)
{
	SelectedTrackName = TrackName;
	OnSelectedRecordingTrackChanged.Broadcast(TrackName);
}

FName ABrickstonAlleyGameModeBase::GetSelectedRecordingTrack() const
{
	return SelectedTrackName;
}

TArray<FName> ABrickstonAlleyGameModeBase::GetNonSelectedRecordingTracks() const
{
	TArray<FName> Result;
	ElsRecordingTracksByName.GetKeys(OUT Result);
	return Result.FilterByPredicate([this](const FName& NameItr) { return NameItr != SelectedTrackName; });
}

void ABrickstonAlleyGameModeBase::DeleteRecordingTrack(FName TrackName)
{
	if (!ElsRecordingTracksByName.Contains(TrackName))
		return;

	ElsRecordingTracksByName.Remove(TrackName);
	OnRecordingTracksChanged.Broadcast();
}

void ABrickstonAlleyGameModeBase::ApplyKeyFramesToRecordingTrack(FName TrackName)
{
	if (!ElsRecordingTracksByName.Contains(TrackName))
		return;

	ElsRecordingTracksByName[TrackName].LightNames = SelectedLightNames;
	TArray<int32>& KeyFrameValues = ElsRecordingTracksByName[TrackName].KeyFrameValues;
	for (int32 i = 0; i < KeyFrames.Num(); i++)
	{
		const UPatternKeyFrameObject* KeyFrameObject = KeyFrames[i];
		KeyFrameValues[i] = KeyFrameObject->Value;
	}
}

void ABrickstonAlleyGameModeBase::LoadRecordingTrackIntoKeyFrames(FName TrackName)
{
	if (!ElsRecordingTracksByName.Contains(TrackName))
		return;

	SetSelectedLights(ElsRecordingTracksByName[TrackName].LightNames);
	const TArray<int32>& KeyFrameValues = ElsRecordingTracksByName[TrackName].KeyFrameValues;
	for (int32 i = 0; i < KeyFrames.Num(); i++)
	{
		UPatternKeyFrameObject* KeyFrameObject = KeyFrames[i];
		KeyFrameObject->SetValue(KeyFrameValues[i]);
		KeyFrameObject->SetHighlighted(-1);
	}
}

void ABrickstonAlleyGameModeBase::ClearRecordingData(bool bClearKeyFrames, bool bClearSelectedLights)
{
	if (bClearSelectedLights)
	{
		SelectedLightNames.Empty();
		OnSelectedLightsChanged.Broadcast({});
	}

	if (!bClearKeyFrames)
		return;

	for (UPatternKeyFrameObject* KeyFrameObject : KeyFrames)
	{
		KeyFrameObject->SetValue(0);
		KeyFrameObject->SetHighlighted(false);
	}
}

TArray<UPatternKeyFrameObject*> ABrickstonAlleyGameModeBase::GetKeyFrames() const
{
	return KeyFrames;
}

void ABrickstonAlleyGameModeBase::SetLightSelected(FName LightName, bool bSetSelected)
{
	const bool bIsSelected = IsLightSelected(LightName);
	if (!bIsSelected && bSetSelected)
		SelectedLightNames.Add(LightName);
	else if (bIsSelected && !bSetSelected)
		SelectedLightNames.Remove(LightName);
	else return;

	OnSelectedLightsChanged.Broadcast(SelectedLightNames);
}

void ABrickstonAlleyGameModeBase::SetSelectedLights(TArray<FName> NewSelectedLightNames)
{
	SelectedLightNames = NewSelectedLightNames;
	OnSelectedLightsChanged.Broadcast(SelectedLightNames);
}

TArray<FName> ABrickstonAlleyGameModeBase::GetSelectedLights() const
{
	return SelectedLightNames;
}

TArray<FName> ABrickstonAlleyGameModeBase::GetTrackNamesThatSelectLight(FName LightName) const
{
	TArray<FName> Result;
	for (auto Itr = ElsRecordingTracksByName.CreateConstIterator(); Itr; ++Itr)
	{
		const FName& TrackNameItr = Itr.Key();
		const FElsRecordingTrack& RecordingTrackItr = Itr.Value();
		if (!RecordingTrackItr.LightNames.Contains(LightName))
			continue;

		Result.Add(TrackNameItr);
	}
	return Result;
}

bool ABrickstonAlleyGameModeBase::IsLightSelected(FName LightName) const
{
	return SelectedLightNames.Contains(LightName);
}

void ABrickstonAlleyGameModeBase::SetDemoCar(AActor* NewDemoCarActor)
{
	DemoCarActor = NewDemoCarActor;
}

AActor* ABrickstonAlleyGameModeBase::GetDemoCar() const
{
	return DemoCarActor;
}

FColor ABrickstonAlleyGameModeBase::MakeColorFromHex(const FString& HexString)
{
	return FColor::FromHex(HexString.Left(7) + "FF");
}

FString ABrickstonAlleyGameModeBase::MakeHexStringFromColor(const FColor& Color)
{
	return "#" + Color.ToHex().Left(6);
}

void ABrickstonAlleyGameModeBase::SetTrackColorAndEmission(FName TrackName, FColor Color, float Emission)
{
	if (!ElsRecordingTracksByName.Contains(TrackName))
		return;

	ElsRecordingTracksByName[TrackName].Color = Color;
	FlasherEmissionLevel = Emission;
	OnTrackColorOrEmissionChanged.Broadcast(TrackName, Color, Emission);
}

float ABrickstonAlleyGameModeBase::GetEmissionLevel() const
{
	return FlasherEmissionLevel;
}

bool ABrickstonAlleyGameModeBase::ExportElsLightPatternsToFile(TArray<FString>& OutErrors, FString& OutFilePath)
{
	const FString TargetDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / "ModExport");
	OutFilePath = TargetDir / "ElsLightPattern.json";
	GConfig->GetString(TEXT("ELS"), TEXT("ExportFilePath"), OUT OutFilePath, GGameIni);

	TArray<FElsRecordingTrack> ElsRecordingTracksToExport;
	ElsRecordingTracksByName.GenerateValueArray(OUT ElsRecordingTracksToExport);

	const bool bSuccess = ImportExport::ELS::ExportElsLightPattern(OutFilePath, ElsRecordingTracksToExport, FlasherEmissionLevel, OUT OutErrors);
	if (OutErrors.Num() > 0)
	{
		const FString ErrorLogPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir() / "ModExport-" + FDateTime::Now().ToString() + ".log");
		TArray<FString> LoggedErrors = OutErrors;
		LoggedErrors.Insert("---------------------------------------", 0);
		LoggedErrors.Insert(FString::Printf(TEXT("Attempted ExportElsLightPatternsToFile: \"%s\""), *OutFilePath), 0);
		if (bSuccess)
			LoggedErrors.Add("\nSuccess.");
		FFileHelper::SaveStringArrayToFile(LoggedErrors, *ErrorLogPath);
		FWindowsPlatformProcess::ExploreFolder(*ErrorLogPath);
	}

	if (bSuccess)
		FWindowsPlatformProcess::ExploreFolder(*OutFilePath);
	return bSuccess;
}

void ABrickstonAlleyGameModeBase::StopPlayback()
{
	CurrentPlaybackCycle = 0;
	CurrentPlaybackFrame = 0;
	OnPlaybackFrameChanged.Broadcast(CurrentPlaybackFrame, CurrentPlaybackCycle);
}

void ABrickstonAlleyGameModeBase::PlaybackNextFrame()
{
	static constexpr int32 RepetitionsPerSecondSegment = 3;
	const int32 FramesPerSecond = GetElsFlasherLightPatternFPS();
	const int32 NumFramesTotal = GetElsFlasherLightPatternDuration() * FramesPerSecond;

	CurrentPlaybackFrame++;
	if ((CurrentPlaybackFrame % FramesPerSecond) == 0)
	{
		CurrentPlaybackCycle++;
		CurrentPlaybackFrame -= FramesPerSecond;
	}
	if (CurrentPlaybackCycle >= RepetitionsPerSecondSegment)
	{
		CurrentPlaybackCycle = 0;
		CurrentPlaybackFrame += FramesPerSecond;
	}
	if (CurrentPlaybackFrame >= NumFramesTotal)
	{
		CurrentPlaybackCycle = 0;
		CurrentPlaybackFrame = 0;
	}

	OnPlaybackFrameChanged.Broadcast(CurrentPlaybackFrame, CurrentPlaybackCycle);
}

void ABrickstonAlleyGameModeBase::GetCurrentPlaybackFrame(int32& OutFrame, int32& OutCycle) const
{
	OutFrame = CurrentPlaybackFrame;
	OutCycle = CurrentPlaybackCycle;
}

void ABrickstonAlleyGameModeBase::SetDefaultLightIntensity(float Intensity)
{
	DefaultLightIntensity = Intensity;
}

void ABrickstonAlleyGameModeBase::NotifyAnyKeyFrameChanged(int32)
{
	OnAnyKeyFrameChanged.Broadcast();
}

FName ABrickstonAlleyGameModeBase::MakeUniqueElsTrackName() const
{
	FName TrackName;
	int32 i = 1;
	do
	{
		TrackName = *FString("ELS Track" + FString::FromInt(i));
		i++;
	}
	while (ElsRecordingTracksByName.Contains(TrackName));
	return TrackName;
}
