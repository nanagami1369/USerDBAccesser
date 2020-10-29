# c++でデータベースにアクセスする練習プログラム

## 概要

大学の講義で出た課題

ローカルでユーザーデータを作成、保存、閲覧できるコマンドラインアプリ

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
