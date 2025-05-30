# Proyek2-Kel1
*pengembangan Aplikasi berbasis library menggunakan bahasa C*
_Aplikasi Gim Tetromania_

Anggota:
- Hafizh Andika
- M. Naufal Alfarizky
- M. Naufal Nurmaryadi
- Micky Ridho
- Rifky Hermawan

## Deskripsi Game

Tetromania adalah game klasik berbasis bahasa C yang terinspirasi dari permainan Tetris, dikembangkan menggunakan library graphics.h. Sama dengan tetris game ini bertujuan untuk menyusun block-block yang jatuh agar dapat memenuhi area grid agar mendapatkan skor sebanyak-banyaknya. Game ini dibuat sebagai bagian dari tugas mata kuliah Proyek 2: Pembuatan Aplikasi Berbasis Library dengan memanfaatkan konsep array dan linked list dalam pengembangannya.

## Instalasi

Untuk memainkan game ini dapat dengan mengunduh file tetromania.exe nya langsung atau dengan melakukan clone pada repositori ini.

Berikut langkah-langkah jika ingin melakukan clone pada repositori ini:
* copy link repositori berikut [https://github.com/harifky/Proyek2-Kel1.git]
* buka file manager
* cari dan pilih folder yang akan digunakan untuk menyimpan hasil clone repositori ini
* buka folder yang telah dipilih menggunakan git bash atau cmd
* ketikkan git clone [link]
* tunggu hingga selesai
* Jika sudah selesai buka folder repositori menggunakan VS Code
* Lakukan compile dengan menekan win + shift + B
* Jika tidak bisa, copy perintah berikut pada terminal:
    ```` g++ main.c body/game.c body/Hafizh.c body/Micky.c body/Naufal_A.c body/Naufal_N.c body/Rifky.c -o output/tetris.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -lwinmm ````
* Setelah selesai maka file .exe akan terbuat pada folder _/output_
* Klik kanan pada file tersebut dan Run Executable atau double klik pada file .exe di file manager


## Cara Bermain

### Tujuan

menyusun block-block tetromino yang jatuh dari atas agar membentuk baris penuh secara horizontal. Baris yang penuh akan menghilang, dan akan memberikan skor. Permainan berakhir jika blok menumpuk hingga ke atas grid. Dapatkan Skor setinggi-tingginya agar kamu dapat berada pada top leaderboard!

### Control Permainan

| Tombol               | Fungsi                                              |
| -------------------- | --------------------------------------------------- |
| 🔼 (Arrow Up)        | Memutar tetromino (rotate)                          |
| 🔽 (Arrow Down)      | Mempercepat jatuhnya tetromino                      |
| ◀️ (Arrow Left)      | Menggeser tetromino ke kiri                         |
| ▶️ (Arrow Right)     | Menggeser tetromino ke kanan                        |
| **Spasi**            | Menjatuhkan tetromino langsung ke bawah (hard drop) |
| **Enter**            | Menahan (hold) tetromino aktif                      |


