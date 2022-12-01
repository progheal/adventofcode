Advent of Code 解題程式碼

[文章連結] (https://blog.cruciferslab.net/?p=1153)

`aoclib` 是一些題目之間的公用函式：

* `aoc.h`：處理輸入
* `aocmachine.h`：處理簡單組合語言分析執行
* `bfs.h`：泛用 BFS 及 A* 演算法
* `ocr.h`：參照其他 AoC 的 OCR 函式庫實作的我流版本
* `util.h`：其他公用函數

其中 `ocr.h` 及 `util.h` 有原始碼實作，需連結 `libaoc.a`；其餘則是泛型 header-only 函式庫。

各年度的程式碼以年度為資料夾分類。

部份程式會將 A B 部份合在一支程式當中，以叫用時的程式名表示是何部份；程式由標準輸入讀取，輸出至標準輸出。
