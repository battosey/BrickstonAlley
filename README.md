![Preview](https://steamuserimages-a.akamaihd.net/ugc/1881954907577863838/559835FD8145D9A28B2B55D89869E358FCEB7F78/?imw=5000&imh=5000&ima=fit&impolicy=Letterbox&imcolor=#000000&letterbox=false)

# BrickstonAlley
Mod Editor Garage for [Police Simulator : Patrol Officers](https://steamcommunity.com/app/997010/workshop/)

This is a fan creation! All used assets from Police Simulator Patrol Officers were freely distributed by the publisher in the [Official Modding Kit](https://steamcommunity.com/sharedfiles/filedetails/?id=27824719059/).

I recommend reading the [Official Modding Guide: How to create ELS mods](https://steamcommunity.com/sharedfiles/filedetails/?id=28089240419/) first.

# How To

## Start BrickstonAlley
1. [Download BrickstonAlley](https://steamcommunity.com/sharedfiles/filedetails/?id=2816922331) by subscribing in the Steam Workshop
2. Navigate to the download location (PATH_TO_YOUR_STEAM/workshop/content/997010/2816922331)
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

## Understand UI Elements
#### Track Controls. Top-Left.
- Reset the selected track by clearing all keyframes
- Create a new track. There is no option to delete tracks for now, you can just leave them be
- Duplicate the current track preserving keyframes but not the targeted lights

#### Playback Controls. Top-Center.
- Start/Pause or Stop the playback. All tracks are played in parallel unless you pause them individually
- Playback speed can be changed, but this setting does not affect the exported track
- Press&Hold the REC button to set keyframes for the duration of the press. Only works in play mode

#### Keyframes. Five rows, three columns.
- Each strip of 30 keyframes represents one second of the track (= segment)
- Each segment is repeated three times (as dictated by Police Sim API)
- You can record five segments, resulting in 5 x 3 = 15 seconds to a total of 15 x 30 = 450 keyframes
- Clicking on a keyframe sets the value of the keyframe to the Default Brightness value
- The value of a keyframe can be dragged up (to a max of 100) and down (to a min of 1), representing the Brightness of the light. THIS ONLY AFFECTS THE TURRET LIGHTS! Flasher lights are either on or off, regardless of the Brightness value.
- Clicking on the (colored) bottom part of a keyframe removes it

#### Track Settings. Center-Right.
- Default Brightness: The Brightness value set for new keyframes. Changing this value only affects new keyframes
- Flasher Emission: Global value for the emission level of the flashers. Changing this affects all flashers and is important for the export. Note that a value above 0.1 will result in a VERY BRIGHT flasher..
- Track Light Color: HEX color value (RGB) of all selected lights in the current track. This setting can be changed per track

#### Light List
- All moddable lights as defined by the Police Sim API
- Hovering a light button will draw a line to the respective light object in the scene
- Clicking a button, thus selecting a light will assign that light to the current track. EACH LIGHT SHALL ONLY BE ASSIGNED TO ONE TRACK! Putting a light into multiple tracks will result in unwanted behaviour
- Lights whose name starts with "EmergencyLight" are turret lights that actually act as light emitters in-game. The rest are flashers that only represent the texture of the physical car components

#### Menu
- Reset Camera to the default position, shall you get lost trying to find the hidden easter egg
- Export Tracks to JSON for your mod pack. An exporer window opens automatically selecting the generated file when it is done. If an error occurs, a log file will be generated and opened in a similar fashion

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
