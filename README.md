# App1
## DirectXToolKitXaml12 Sample Library Link App1


## This is a sample App1 for linking to the library of the [DirectXToolKitXaml12](https://github.com/hot3dx/DirectXToolKitXaml) Library

![App1_Screenshot](https://github.com/hot3dx/App1/blob/master/Assets/App1-DirectXToolKitXaml12-Sample-App.png)

# Requirements

### 1) Nuget Package <package id="WinPixEventRuntime" version="1.0.200127001" targetFramework="native" /> required

### 2) DirectXToolKitXaml12.lib is required first!

### 3) In the PCH.H you must change the four paths at the bottom for your 
location of the DirectXToolKitXaml.lib on your computer in the
#pragma comment(lib, "...") 
## or, you could install the library at C:\DirectXToolKitXaml12 folder
and have absolutely no problems at all! The project files are set for that location!

### 4) The paths in App1 Property Pages must be changed to the locations of the output 
DirectXToolKitXaml12.lib for each Release/Debug x86/x64 combinations

### Project->Properties->Configuration Properties->Linker->All Options->Additional Options

/ Leave /WHOLEARCHIVE:C://.... the must match each path in the pch,h file!

### 5) Batch Build fro the build menu selection Relase/Debug x86/x64 checkboxes

# That's it!

# To start the 3D in the blank App1 
Click the bottom App Bar three dots (...) in the lower right hand corner
Click the "Start button on the left of the AppBottomBar

##Sound auto starts 12-31-2020  Happy New Year!

#Keys:

q,w,a,s,e,d move the camera X, Y, Z directions

# click the mouse to draw points 
# then hold down the mouse button as you move the mouse they draw

## Windows 10, Visual Studio 2019 tools v142 C++17 Target Platform Version 10.0.18362.0 VS Version 16.8.3

It will stick the first time in x86 or x64 debug. Hit restart after everything is loaded

# BTC Donations Greatly Accepted!

BTC 33DN8TgAe879tDkhsCEpneoVrydHbasuLm 

BCHABC qruhxl8y2753fg0qfx7t8zfqzw92rvaxussk7fcnke

LTC MEQVCCxa5ZZkmGqAfYfRvipNcY4BaTRrFm

By:
![Ico](https://github.com/hot3dx/AppXamlDX12/blob/master/Assets/AutoDraw2.ico) Jeff Kubitz


# Errors

In the event of an error,
1) tiny.sdkmesh did not copy to AppX\\Assets\\
for each Debug/Release x86/x64 combination
2) SegoeUI_18.spritefont and segoeui.ttf
did not copy to the AppX folder for
Debug/Release x64.
3) SegoeUI_18.spritefont and segoeui.ttf
did not copy to the Debug/Release folder for
Debug/Release x86.

Manually copy those folders from App1\\Assets to 
these above locations and it is magic!


