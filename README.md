# lifegame

ライフゲームを作成しました。
- FreeBSD
- C言語

参照:
- [「ライフゲーム」(Wikipedia)](https://ja.wikipedia.org/wiki/%E3%83%A9%E3%82%A4%E3%83%95%E3%82%B2%E3%83%BC%E3%83%A0)

## コンパイル

```sh
cd src/
make
```

`/usr/local/`に置きたい場合:
```sh
sudo cp bin/lifegame /usr/local/bin/
```

自分専用に置きたい場合: (`$HOME/.diy/`)
```sh
pushd ..
csh ./set-my-diy.csh
popd

# csh/tcsh なら .cshrc に追加
vi ~/.cshrc
--------------------
set DIY = $HOME/.diy
if (-d $DIY) then
        # bin
        setenv PATH $DIY/bin:$PATH
        # lib
        if ($?LD_LIBRARY_PATH) then
                setenv LD_LIBRARY_PATH $DIY/lib:$LD_LIBRARY_PATH
        else
                setenv LD_LIBRARY_PATH $DIY/lib
        endif
        # pkgconf
        alias pkgconf   pkgconf --define-prefix
        if ($?PKG_CONFIG_PATH) then
                setenv PKG_CONFIG_PATH $DIY/libdata/pkgconfig:$PKG_CONFIG_PATH
        else
                setenv PKG_CONFIG_PATH $DIY/libdata/pkgconfig
        endif
endif
--------------------
```

削除するなら:
```sh
make clean
# もしくは
make cleandir
```

## 実行

```
% lifegame
```
![Demo](demo.gif)

