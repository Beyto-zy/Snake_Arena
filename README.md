# 🐍 Snake Arena

Çok oyunculu yılan oyununun **komut tabanlı oyun motoru**: birden fazla yılan aynı tahtada hareket eder, yem yer, büyür, birbirini ısırır ve skor tablosunda yarışır.

C++ ile, hazır kapsayıcı (STL `list`, `vector` vb.) kullanmadan, tüm veri yapıları elle yazılarak geliştirildi.

## Veri yapıları

| Parça | Yapı |
|---|---|
| Tahta | Dinamik olarak ayrılan 2B `char` matrisi |
| Yılan gövdesi | Çift yönlü bağlı liste (baş → kuyruk) |
| Oyuncu sırası | Tek yönlü bağlı liste (doğma sırası = hamle sırası) |
| Skor tablosu | Her an sıralı tutulan tek yönlü bağlı liste |

## Öne çıkanlar

- **Move semantics:** Tahta ve yılan gövdesi kopyalanamaz, yalnız taşınabilir (move constructor ve move assignment).
- **Bağlı liste işlemleri:** Başa/sona ekleme, yerinde ters çevirme (yılanın kuyruğu başı olur) ve bir hücreden ikiye bölme (ısırılan yılanın kopan parçası).
- **Çarpışma kuralları:** Duvar, kendini ısırma, başka yılanı ısırma ve kafa kafaya çarpışma (uzun olan kazanır).
- **Bellek güvenliği:** Her nesnenin sahibi belli; ölen yılanlar tur sonunda silinir, sızıntı yok.

## Derleme ve çalıştırma

```bash
make                         # Windows/MinGW: mingw32-make
./SnakeArena input.txt
```

`make` yoksa: `g++ -std=c++11 *.cpp -o SnakeArena`

Bellek sızıntısı kontrolü (Linux, valgrind): `make mem INPUT=input.txt`

## Örnek

Girdi dosyası her satırda bir komut içerir:

```
INIT_BOARD 4 7
WALL 0 4
FOOD 1 5
SPAWN A 1 3 RIGHT 3
SPAWN B 3 2 LEFT 3
PRINT_BOARD
TICK 2
PRINT_BOARD
PRINT_LEADERBOARD
```

Çıktı:

```
Board initialized with 4 rows and 7 columns.
Wall placed at (0,4).
Food placed at (1,5).
Snake A spawned at (1,3) facing RIGHT with length 3.
Snake B spawned at (3,2) facing LEFT with length 3.
--- Board 4x7 ---
....#..
.aaA.*.
.......
..Bbb..
--- Tick 1 ---
--- Tick 2 ---
Snake A ate food at (1,5).
--- Board 4x7 ---
....#..
..aaaA.
.......
Bbb....
--- Leaderboard ---
1. A score=1 length=4
2. B score=0 length=3
```

`#` duvar, `*` yem, büyük harf yılanın başı, küçük harf gövdesi.
