#include "sudoku.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <numeric>
using namespace std;

// kiểm tra số hợp lệ
bool so_hople(const vector<vector<int>>& bang, int hang, int cot, int so) {
    int n = bang.size();
    int k = sqrt(n); // kích thước ô vuông con

    for (int i = 0; i < n; i++) {
        if (bang[hang][i] == so || bang[i][cot] == so)
            return false;
    }

    int dongbatdau = hang / k * k;
    int cotbatdau = cot / k * k;
    for (int i = dongbatdau; i < dongbatdau + k; i++)
        for (int j = cotbatdau; j < cotbatdau + k; j++)
            if (bang[i][j] == so)
                return false;

    return true;
}

// điền số vào bảng
bool dien_vao_bang(vector<vector<int>>& bang, int hang, int cot) {
    int n = bang.size();
    if (hang == n) return true;

    int dong_tiep;
    if (cot == n - 1) {
        dong_tiep = hang + 1;
    } else {
        dong_tiep = hang;
    }
    int cot_tiep = (cot + 1) % n;

    vector<int> danh_sach(n);
    iota(danh_sach.begin(), danh_sach.end(), 1);
    shuffle(danh_sach.begin(), danh_sach.end(), mt19937{ random_device{}() });

    for (int so : danh_sach) {
        if (so_hople(bang, hang, cot, so)) {
            bang[hang][cot] = so;
            if (dien_vao_bang(bang, dong_tiep, cot_tiep))
                return true;
            bang[hang][cot] = 0;
        }
    }
    return false;
}

// tạo bảng sudoku hoàn chỉnh
vector<vector<int>> taoBangDayDu(int size) {
    vector<vector<int>> bang(size, vector<int>(size, 0));
    dien_vao_bang(bang, 0, 0);
    return bang;
}

// xoá ngẫu nhiên n ô
void xoaOngaunhien(vector<vector<int>>& bang, int so_o_trong) {
    int n = bang.size();
    vector<pair<int, int>> vitri;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            vitri.emplace_back(i, j);
    shuffle(vitri.begin(), vitri.end(), mt19937{ random_device{}() });
    for (int i = 0; i < so_o_trong && i < (int)vitri.size(); ++i)
        bang[vitri[i].first][vitri[i].second] = 0;
}

// kiểm tra bảng người chơi có đúng không
bool kiemtrabang_dung(const vector<vector<int>>& bang) {
    int n = bang.size();
    int k = sqrt(n);

    // hàng
    for (int i = 0; i < n; i++) {
        vector<int> check(n + 1, 0);
        for (int j = 0; j < n; j++) {
            int val = bang[i][j];
            if (val < 1 || val > n || check[val]) return false;
            check[val] = 1;
        }
    }

    // cột
    for (int j = 0; j < n; j++) {
        vector<int> check(n + 1, 0);
        for (int i = 0; i < n; i++) {
            int val = bang[i][j];
            if (val < 1 || val > n || check[val]) return false;
            check[val] = 1;
        }
    }

    // vùng vuông nhỏ
    for (int r = 0; r < n; r += k)
        for (int c = 0; c < n; c += k) {
            vector<int> check(n + 1, 0);
            for (int i = 0; i < k; i++)
                for (int j = 0; j < k; j++) {
                    int val = bang[r + i][c + j];
                    if (val < 1 || val > n || check[val]) {
                        return false;
                    }
                    check[val] = 1;
                }
        }

    return true;
}