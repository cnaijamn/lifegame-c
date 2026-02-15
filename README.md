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
sudo cp lifegame /usr/local/bin/
```

自分専用に置きたい場合: (`$HOME/wrk/` etc.)
```sh
mkdir -p ~/wrk/bin

# csh/tcsh なら .cshrc に追加
vi ~/.cshrc
--------------------
setenv PATH $HOME/wrk/bin:$PATH
--------------------

cp lifegame ~/wrk/bin/
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

