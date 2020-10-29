# c++でデータベースにアクセスする練習プログラム

## 概要

大学の講義で出た課題

ローカルでユーザーデータを作成、保存、閲覧できるコマンドラインアプリ

## ビルド

```bash
  git clone https://github.com/nanagami1369/UserDBAccesser
  mkdir build
  cd build
  cmake ../
  make
```

## 実行

```bash
    ./build/UserDBAccesser
```

## 使用したライブラリー

- SQLiteCpp
  - Copyright (c) 2012-2020 Sebastien Rombauts (sebastien.rombauts@gmail.com)
  - [MIT License](https://github.com/SRombauts/SQLiteCpp/blob/master/LICENSE.txt)
