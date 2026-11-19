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
make
./SnakeArena input.txt
```

Bellek sızıntısı kontrolü (Linux, valgrind): `make mem INPUT=input.txt`

## Örnek tahta

```
....#..
.aaA#.*
.......
..Bbbb.
```

`#` duvar, `*` yem, büyük harf yılanın başı, küçük harf gövdesi.
