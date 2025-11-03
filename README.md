Remove-Item build -Recurse -Force → Xóa thư mục build cũ (nếu có).

mkdir build → Tạo thư mục build mới.

cd build → Di chuyển vào thư mục build.

cmake -G "Visual Studio 17 2022" -A x64 .. → Dùng CMake để tạo project Visual Studio 2022 (64-bit) từ mã nguồn ở thư mục cha.

cmake --build . --config Release → Biên dịch project vừa tạo ở chế độ Release.
