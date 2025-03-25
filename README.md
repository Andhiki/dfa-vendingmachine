# DFA Vending Machine

## Nama Anggota Kelompok
1. Andreandhiki Riyanta Putra (23/517511/PA/22191)
2. Andrian Danar Perdana (NIM)

## Deskripsi Program
Program ini merupakan simulasi vending machine berbasis Deterministic Finite Automaton (DFA). Pengguna dapat memasukkan uang dan memilih minuman yang tersedia. Jika saldo mencukupi, minuman dapat dibeli dan kembalian diberikan jika ada.

## How to Run

### Langkah-langkah
1. Simpan file program sebagai `vm.cpp`
2. Buka terminal atau command prompt, lalu navigasikan ke direktori tempat file disimpan.
3. Jalankan perintah berikut untuk mengompilasi program:
   ```sh
   g++ vm.cpp -o vm
   ```
4. Jalankan program dengan perintah:
   ```sh
   ./vm
   ```
5. Masukkan nominal uang atau pilih minuman sesuai pilihan yang tersedia:
   - Uang: `1000`, `2000`, `5000`, `10000`
   - Minuman: `A`, `B`, `C`
6. Ketik `exit` untuk keluar dari program.

## Contoh Interaksi
```
Masukkan uang atau pilih minuman (1000, 2000, 5000, 10000, A, B, C): 2000
Minuman yang bisa dibeli:
Saldo saat ini: 2000
Masukkan uang atau pilih minuman (1000, 2000, 5000, 10000, A, B, C): 1000
Minuman yang bisa dibeli: A (3000)
Saldo saat ini: 3000
Masukkan uang atau pilih minuman (1000, 2000, 5000, 10000, A, B, C): A
Minuman A dapat dibeli dengan saldo 3000. Status: ACCEPTED.
Kembalian: 0
```

## Catatan
- Program akan mereset saldo setelah pembelian berhasil.
- Jika saldo tidak mencukupi, pengguna harus menambahkan uang sebelum membeli minuman.
- Input yang tidak valid akan ditolak oleh sistem.

