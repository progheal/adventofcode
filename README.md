Advent of Code 解題程式碼

我的 Blog 文章：

* [2022 文章](https://blog.cruciferslab.net/?p=1153)
* [2023 文章](https://blog.cruciferslab.net/?p=1587)

`aoclib` 是一些題目之間的公用函式：

* `aoc.h`：處理輸入
* `aocmachine.h`：處理簡單組合語言分析執行
* `bfs.h`：泛用 BFS 及 A* 演算法；這個檔案有英文註解是為了[貼去 Reddit 上](https://redd.it/zu7kq8)
* `ocr.h`：參照其他 AoC 的 OCR 函式庫實作的我流版本
* `util.h`：其他公用函數

其中 `ocr.h` 及 `util.h` 有原始碼實作，需連結 `libaoc.a`；其餘則是泛型 header-only 函式庫。

各年度的程式碼以年度為資料夾分類；主目錄的 empty.cpp 則是空模板，只包含標頭檔和 B 部份判斷。

大部份程式會將 A B 部份合在一支程式當中，以叫用時的程式名表示是何部份；少部份則會有另外的 B 部份程式碼存在。程式由標準輸入讀取，輸出至標準輸出。

使用的 C++ 標準預設是 C++17，所以公用函式也以 C++17 為標準來寫，不會使用到 C++20 的新功能。(特別是泛型程式**不會**使用 C++20 concept) 各題的程式本身則視狀況而定，會寫在 `Makefile` 裡。

====

[English]

This is my AoC (Advent of Code) solutions.

Comments and documents will mostly in Traditional Chinese; the link at the top
of the document is a blog post by me (also in Traditional Chinese) describing
some thoughts and processes when solving the problems in 2022 and 2023. 

An exception is that there is some English comment in `aoclib/include/aoc/bfs.h`
so I can post this as a support to this
[excellent Reddit post about abstracting search algorithms](https://redd.it/zu7kq8).
