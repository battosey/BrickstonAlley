
#pragma once

#include "CoreMinimal.h"

#include "BrickstonAlley/Data/ElsPatternTypes.h"

namespace ImportExport::ELS
{
	bool ExportElsLightPattern(FString DesiredFilePath, const TArray<FElsRecordingTrack>& RecordingTracks, float EmissionLevel, TArray<FString>& OutErrors);
}
