#include "sudoku.h"
#include <algorithm>
#include <random>
using namespace std;

// kiểm tra số hợp lệ
bool so_hople(const vector<vector<int>>& bang, int hang, int cot, int so) {
    for (int i = 0; i < 4; i++) {
        if (bang[hang][i] == so || bang[i][cot] == so)
            return false;
    }
    int dongbatdau = hang / 2 * 2, cotbatdau = cot / 2 * 2;
    for (int i = dongbatdau; i < dongbatdau + 2; i++)
        for (int j = cotbatdau; j < cotbatdau + 2; j++)
            if (bang[i][j] == so)
                return false;
    return true;
}

// điền số để tạo bảng sudoku hoàn chỉnh
bool dien_vao_bang(vector<vector<int>>& bang, int hang, int cot) {
    if (hang == 4) return true;

    // tính vị trí ô tiếp
    int dong_tiep = (cot == 3) ? hang + 1 : hang;
    int cot_tiep = (cot + 1) % 4;
    vector<int> danh_sach = {1,2,3,4};
    shuffle(danh_sach.begin(), danh_sach.end(), mt19937{random_device{}()});
    for (int so : danh_sach) {
        if (so_hople(bang, hang, cot, so)) {
            bang[hang][cot] = so;
            if (dien_vao_bang(bang, dong_tiep, cot_tiep)) {
                return true;
            }
            bang[hang][cot] = 0;
        }
    }
    return false; // khoong tÌm đc hợp lệ
}

//tạo bảng hoàn chỉnh
vector<vector<int>> taoBangDayDu() {
    vector<vector<int>> board(4, vector<int>(4, 0));

    dien_vao_bang(board, 0, 0);
    return board;
}


//xoá ngẫu nhiên n ô trong bảng
void xoaOngaunhien(vector<vector<int>>& bang, int so_o_trong) {
    vector<pair<int,int>> positions;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            positions.emplace_back(i, j);
   shuffle(positions.begin(), positions.end(), mt19937{random_device{}()});
    for (int i = 0; i < so_o_trong; ++i)
        bang[positions[i].first][positions[i].second] = 0;
}


//kiểm tra người chơi đã điền đúng chưa
bool kiemtrabang_dung(const vector<vector<int>>& bang) {
    
    // Kiểm tra từng cột
    for (int j = 0; j < 4; j++) {
        vector<int> cot_hople;
        for (int i = 0; i < 4; i++) {
            if (find(cot_hople.begin(), cot_hople.end(), bang[i][j]) != cot_hople.end()) {
                return false; // Trùng lặp trong cột
            }
            cot_hople.push_back(bang[i][j]);
        }
    }

    // Kiểm tra từng hàng
    for (int i = 0; i < 4; ++i) {
        vector<int> hang_hople;
        for (int j = 0; j < 4; ++j) {
            if (find(hang_hople.begin(), hang_hople.end(), bang[i][j]) != hang_hople.end()) {
                return false; // Trùng lặp trong hàng
            }
            hang_hople.push_back(bang[i][j]);
        }
    }

    // Kiểm tra từng tiểu vùng 2x2
    for (int hang_batdau = 0; hang_batdau < 4; hang_batdau += 2) {
        for (int cot_batdau = 0; cot_batdau < 4; cot_batdau += 2) {
            vector<int> block_values;
            for (int i = hang_batdau; i < hang_batdau + 2; ++i) {
                for (int j = cot_batdau; j < cot_batdau + 2; ++j) {
                    if (find(block_values.begin(), block_values.end(), bang[i][j]) != block_values.end()) {
                        return false; // Trùng lặp trong tiểu vùng xác định
                    }
                    block_values.push_back(bang[i][j]);
                    int c = 0;
                }
            }
        }
    }

    return true; // Nếu không có trùng lặp ở hàng, cột, tiểu vùng
}
