# ESP-NOWを利用したMyCobotコントローラ

[英語版][README.md]

ATOM Liteのボタンを押すたびにESP-NOWのブロードキャストを使用してMyCobotにコマンドを送信します。複数のMyCobotを同期して動かすことも可能です。

<div align="center">
<a href="https://www.youtube.com/watch?feature=player_embedded&v=9meBfmLa9Z8
" target="_blank"><img src="https://img.youtube.com/vi/9meBfmLa9Z8/0.jpg"
alt="動作の様子" width="360" height="270" /></a><br>
動作の様子
</div>

## インストール方法

※MyCobotのM5Stack BasicにはESP-NOWを有効にした[非公式Transponder](https://github.com/3110/mycobot-transponder)を書き込んでおく必要があります。

[PlatformIO](https://platformio.org/)環境に対応しています。動作確認はWindows 10上で[PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode)を使用して実施しています。

```
$ git clone https://github.com/3110/mycobot-atom-controller
$ cd mycobot-atom-controller
$ platformio run --target=upload
```

## 使用方法

起動時にESP-NOWが有効になっていると，ATOM LiteのLEDが黄色に光ります。

ATOM Liteのボタンを押すとESP-NOWが有効になったTransponderが動いているMyCobotに向けてコマンドが送信されます。

グリッパーが開くときにATOM LiteのLEDが青に，閉じるときに赤に変わります。
