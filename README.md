# BrickstonAlley
Mod Editor Garage for [Police Simulator : Patrol Officers](https://steamcommunity.com/app/997010/workshop/)

This is a fan creation! All used assets from Police Simulator Patrol Officers were freely distributed by the publisher in the [Official Modding Kit](https://steamcommunity.com/sharedfiles/filedetails/?id=27824719059/).

I recommend reading the [Official Modding Guide: How to create ELS mods](https://steamcommunity.com/sharedfiles/filedetails/?id=28089240419/) first.

# How To

## Start BrickstonAlley
1. [Download BrickstonAlley](https://steamcommunity.com/sharedfiles/filedetails/?id=2816922331) by subscribing in the Steam Workshop
2. Navigate to the download location (PATH_TO_YOUR_STEAM**/workshop/content/997010/2816922331**)
3. Start the **BrickstonAlley.exe** executable

## Create ELS Tracks
1. Top-Left: Create a new track - or use the empty track provided when starting the tool
2. Right: Select the lights you want to create a track for (by just clicking the buttons)
3. Top-Right: Change light settings like Color, Brightness, Emission (explained further down..)
4. Top-Center: Press **PLAY** and either put down keyframes manually by clicking them, or press&hold the **REC** button to place keyframes for the duration
5. Top-Center: **STOP** the playback and create more tracks with the PLUS button on the left
6. Create more tracks and repeat for the remaining lights until you are happy
7. Left: If you have more than one track, you can toggle the playback of the other tracks with the **PLAY/PAUSE** button
8. Bottom: When you are done, **EXPORT** the sequence to an **ElsLightPattern.json** file that you can use in your mod pack to upload

## Account for ELS Blip Sequence Time Limit
Note that for Blip sequences, only the first second of the tracks will be loaded into the game!

## Debug
When something went wrong, an explorer window should pop up after **EXPORT** focusing a Logfile that you can open with any text editor.

## ADVANCED: Change Settings
1. Find the Config folder of BrickstonAlley
2. Open or create the file "Config/DefaultGame.ini"
3. Copy&Paste and modify one or more of the following settings:

```
[ELS]
ExportFilePath="C:/YourCustomExportPath/ElsLightPattern.json"
```
