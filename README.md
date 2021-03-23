# MyCobot Controller using ESP-NOW

[日本語版](README_ja_JP.md)

Everytime you press the button on ATOM Lite, you can send commands to MyCobot with ESP-NOW broadcast.
You can also synchronize all MyCobots installed the ESP-NOW enabled Transponder.

<div align="center">
<a href="https://www.youtube.com/watch?feature=player_embedded&v=9meBfmLa9Z8
" target="_blank"><img src="https://img.youtube.com/vi/9meBfmLa9Z8/0.jpg"
alt="How it works" width="360" height="270" /></a><br>
How it works
</div>

## Installation

You need to write [the Unofficial Transponder](https://github.com/3110/mycobot-transponder) into MyCobot Basic to support ESP-NOW.

The program can be compiled on the [PlatformIO](https://platformio.org/) environment.  I have tested the program with [PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode) on Windows 10.

```
$ git clone https://github.com/3110/mycobot-atom-controller
$ cd mycobot-atom-controller
$ platformio run --target=upload
```

## Usage

If ESP-NOW is enabled at startup, the LED on ATOM Lite turns yellow.

When the button on ATOM Lite is pressed, MyCobot commands are sent to MyCobot running the ESP-NOW enabled Transponder.

The LED on ATOM Lite turns blue when the gripper opens and red when it closes.
