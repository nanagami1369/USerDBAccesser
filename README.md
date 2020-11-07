# c++でデータベースにアクセスする練習プログラム

## 概要

大学の講義で出た課題

ローカルでユーザーデータを作成、保存、閲覧、削除、無効化、有効化できるコマンドラインアプリ

## 使用したコマンド

* git 2.20.1
* sqlite3 3.27.2 2019-02-25 16:06:06
* g++ (Debian 8.3.0-6) 8.3.0
* cmake version 3.13.4
* make GNU Make 4.2.1

## ビルド

```bash
  git clone https://github.com/nanagami1369/UserDBAccesser
  cd UserDBAccesser
  git submodule init
  git submodule update
  mkdir build
  cd build
  # DBにメモリーを使う場合 (default)
  cmake -DDB_TYPE=memory ../
  # DBにsqliteを使う場合
  cmake -DDB_TYPE=sqlite3 ../
  make
```

## 実行

```bash
    ./build/UserDBAccesser
```

## テスト

```bash
    make test
```

## 使用したライブラリー

* SQLiteCpp
  * Copyright (c) 2012-2020 Sebastien Rombauts (sebastien.rombauts@gmail.com)
  * [MIT License](https://github.com/SRombauts/SQLiteCpp/blob/master/LICENSE.txt)

* Google Test
  * Copyright 2008, Google Inc. All rights reserved.
  * [BSD-3-Clause License](https://github.com/google/googletest/blob/master/LICENSE)
