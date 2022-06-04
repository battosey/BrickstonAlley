
#include "ElsModExporter.h"

#include "JsonObjectConverter.h"

bool ImportExport::ELS::ExportElsLightPattern(FString DesiredFilePath, const TArray<FElsRecordingTrack>& RecordingTracks, float EmissionLevel, TArray<FString>& OutErrors)
{
	FString TurretLightNamePrefix = "EmergencyLight";
	GConfig->GetString(TEXT("ELS"), TEXT("TurretLightNamePrefix"), OUT TurretLightNamePrefix, GGameIni);

	FElsLightPatternManifest Manifest = {};
	TArray<FElsTurretLightPattern>& Turrets = Manifest.TurretLightPatterns;
	FElsFlasherLightPatternConfig& Flashers = Manifest.Flashers;
	Flashers.FlasherEmissionLevel = EmissionLevel;

	TArray<FName> ProcessedLights;
	for (const FElsRecordingTrack& Track : RecordingTracks)
	{
		for (const FName& LightName : Track.LightNames)
		{
			const bool bIsTurretLight = LightName.ToString().StartsWith(TurretLightNamePrefix);
			if (ProcessedLights.Contains(LightName))
			{
				OutErrors.Add(FString::Printf(TEXT("[Warning] Light \"%s\" is in multiple recording tracks. Only one track per light is supported!"), *LightName.ToString()));
				continue;
			}

			if (bIsTurretLight)
			{
				FElsTurretLightPattern& NewPattern = Turrets.Add_GetRef(FElsTurretLightPattern(LightName.ToString(), Track.Color));
				NewPattern.FrequencyOverrideValues.AddDefaulted(Track.KeyFrameValues.Num());
				for (int32 i = 0; i < Track.KeyFrameValues.Num(); i++)
				{
					NewPattern.FrequencyOverrideValues[i] = Track.KeyFrameValues[i];
				}
			}
			else
			{
				FElsFlasherLightPattern& NewPattern = Flashers.FlasherPatterns.Add_GetRef(FElsFlasherLightPattern(LightName.ToString(), Track.Color));
				NewPattern.FrequencyOverrideValues.AddDefaulted(Track.KeyFrameValues.Num());
				for (int32 i = 0; i < Track.KeyFrameValues.Num(); i++)
				{
					NewPattern.FrequencyOverrideValues[i] = (Track.KeyFrameValues[i] > 0) ? "True" : "False";
				}
			}
			ProcessedLights.Add(LightName);
		}
	}

	FString JsonString;
	if (!FJsonObjectConverter::UStructToJsonObjectString(Manifest, OUT JsonString))
	{
		OutErrors.Add("[Error] JSON Serialization failed!");
		return false;
	}

	if (!FFileHelper::SaveStringToFile(JsonString, *DesiredFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_EvenIfReadOnly))
	{
		OutErrors.Add(FString::Printf(TEXT("[Error] Could not save file to \"%s\""), *DesiredFilePath));
		return false;
	}
	return true;
}
