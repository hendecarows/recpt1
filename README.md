# recpt1

[recpt1(STZ版)][link_recpt1]に対して以下の変更をしたものです。

* `driver/pt1_ioctl.h` からpx4_drvに付属の`ptx_ioctl.h`に変更
* チャンネル変更時に`ioctl(PTX_SET_SYSTEM_MODE)`を実行
* BS放送に対するTSID選局に対応
* libdvbv5形式のISDB-Sチャンネルファイルに対応

TSIDによる選局対応は[recpt1(otya128)][link_otya128]、libdvbv5形式への対応は[ini][link_inilib]で行いました。

変更の目的は以下です。

* [px4_drv(tsukumijima)][link_px4drv]でチャンネルC13-C24を使えるようにする
* BSに対して相対TS番号`BSxx_y`による選局ではなくTSIDによる選局を可能にする
* libdvbv5形式のISDB-Sチャンネルファイルに対応し、dvbv5-zapと共通化する
* BS放送におけるトランスポンダー移動やTSID変更時の対応を一本化する

libdvbv5形式のISDB-Sチャンネルファイルを使用する場合は、TSIDによる選局となるため、ドライバ側の対応が必要です。
動作確認は[px4_drv(tsukumijima)][link_px4drv]のみで、PT1,PT2,[PT3][link_pt3drv]のchardevドライバには対応していません。
また、libdvbv5形式のISDB-Sチャンネルファイルはrecpt1をコマンドラインからの起動した場合にのみ有効です。
httpサーバーやrecpt1ctlでは使用できません。

```console
recpt1 --device /dev/pxm1urvideo0 --dvbv5 /usr/local/etc/dvbv5/dvbv5_channels_isdbs.conf BS01_0 10 a.ts
```

-----

[recpt1(STZ版)][link_recpt1]のREADMEを以下に示します。

```txt
【recpt1 HTTPサーバ版RC4 + α（STZ版）】
Linux用PT1/PT2/PT3録画プログラムです。
こちらは亜流版ですのでご注意下さい。

本家はこちら→ http://hg.honeyplanet.jp/pt1/
※ドライバー部分は本家やその他分家（↓）のものをお使い下さい。
　PT1/PT2：http://sourceforge.jp/projects/pt1dvr/
　PT3：https://github.com/m-tsudo/pt3
※libarib25はこちら→ https://github.com/stz2012/libarib25

cd recpt1/recpt1
./autogen.sh
./configure --enable-b25
make
でビルドした後、
./recpt1 録画するチャンネル 録画秒数 出力先ファイル名
で録画されます。
詳しいオプションはrecpt1 --helpをご覧下さい。

Special Thanks:
・2chの「Linuxでテレビ総合」スレッドの皆様

動作確認環境:
  Debian 11 x86_64 GNU/Linux
  Linux 5.10.0 SMP
  Ubuntu 22.04 LTS x86_64 GNU/Linux
  Linux 5.15.0 SMP
```

[link_px4drv]: https://github.com/tsukumijima/px4_drv
[link_recpt1]: https://github.com/stz2012/recpt1
[link_otya128]: https://github.com/otya128/recpt1
[link_inilib]: https://github.com/ClecioJung/ini
[link_pt3drv]: https://github.com/m-tsudo/pt3