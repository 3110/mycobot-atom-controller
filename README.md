# ESP-NOWによるMyCobot同期コントローラ

ESP-NOWのブロードキャストを使用して，複数のMyCobotを同期して動かすコントローラです。

M5Atomのボタンを押すとESP-NOWに対応した非公式Transponderにコマンドのバイト列をブロードキャストするという単純な仕組みで動いています。

<div align="center">
<a href="https://www.youtube.com/watch?feature=player_embedded&v=9meBfmLa9Z8
" target="_blank"><img src="https://img.youtube.com/vi/9meBfmLa9Z8/0.jpg"
alt="動作の様子" width="360" height="270" /></a><br>
動作の様子
</div>

## インストール方法

※MyCobotのBasicにはESP-NOWを有効にした[非公式Transponder](https://github.com/3110/mycobot-transponder)を書き込んでおく必要があります。

[PlatformIO](https://platformio.org/)環境に対応しています。動作確認はWindows 10上で[PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode)を使用して実施しています。

```
$ git clone https://github.com/3110/mycobot-atom-controller
$ cd mycobot-atom-controller
$ platformio run --target=upload
```

## 使用方法

ESP-NOWが有効になっていると，M5AtomのLEDが黄色に光ります。

M5Atomのボタンを押すとESP-NOWが有効になったTransponderが動いているMyCobotに向けてコマンドが送信されます。

グリッパーが開くときにM5AtomのLEDが青に，閉じるときに赤に変わります。
