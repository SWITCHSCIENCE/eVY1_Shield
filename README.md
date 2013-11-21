# Sample code for eVY1 Shield
----

## Arduino
### mml_play
歌詞とMMLを配列に格納して再生するサンプルスケッチです。MMLは一般的なコマンドにほぼ対応しています。

### evocaloid_smf
スタンダードMIDIファイルをスケッチ中に埋め込んで再生するサンプルスケッチです。
PROGMEM(AVRマイコン内蔵フラッシュメモリ)にスタンダードMIDIファイルを格納します。  
([Timer1ライブラリ](http://playground.arduino.cc/Code/Timer1)をインストールしてください。)

## Processing
### eVY1_example.pde
ProcessingによるeVY1制御のサンプルプログラムです。Midibusライブラリを使っています。  
スペースバーを押すと歌うプログラムです。押している間だけ歌います。
マウスポインタの横方向でピッチを、縦方向でベロシティを変化させます。  
(デフォルトで入っている「あ」でないと綺麗に音が出ないのですがご了承ください)
