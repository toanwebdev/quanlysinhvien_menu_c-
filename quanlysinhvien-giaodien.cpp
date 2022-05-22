#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include<sstream>
#define Max 1000

using namespace std;

struct sinhVien{
  int maSV;
  string hoDem, ten, queQuan, khoa, lop, khoaHoc;
};

struct DSSV {
	sinhVien dssv[Max];
	int count;
};

struct index{
	int maSV;
	string lop;
	int id;
};

struct DSINDEX {
	index dsIndex[Max];
	int count;
};

// config
void resizeConsole(int width, int height);
void DisableResizeWindow();
void ShowCur(bool CursorVisibility);

void SetColor(int backgound_color, int text_color);
void gotoXY(int x, int y);
int wherex();
int wherey();

// thanh phan va chuc nang
void box(int x, int y, int w, int h, int t_color, int b_color, string text, bool center, bool border, int border_color, int type);
void n_box(int x, int y, int w, int h, int t_color, int b_color, string *texts, bool center, bool border, int border_color, int n);
void inVien();
string convertString(int x);
int convertInt(string s);

void init(DSSV &ds);
void nhapInfo(sinhVien &sv, string action);
void inTieuDe();
void inTieuDeIndex();
void inTieuDeToFile(ofstream &fileOut);

void xuatInfo(sinhVien sv, int stt);
void xuatInfoToFile(ofstream &fileOut, sinhVien sv);
void xuat(DSSV ds);
void xuat(DSINDEX dsId);
void xuat(DSSV ds, DSINDEX dsId);

void swap(index &x, index &y);
DSINDEX idTable(DSSV ds);
void bubbleSort(DSINDEX &dsId);
int binarySearch(DSINDEX dsId, int maSV);

void mainMenu(DSSV &ds, DSINDEX &dsId);
void docFile(DSSV &ds);
void addSV(DSSV &ds, DSINDEX &dsId);
void createAndSortIdTable(DSSV ds, DSINDEX &dsId);
void delSV(DSSV &ds, DSINDEX &dsId);

void editSV(DSSV &ds, DSINDEX &dsId);
void searchSV(DSSV ds, DSINDEX dsId);
void filterSV(DSSV ds, DSINDEX dsId);
void exitAndSave(DSSV ds, bool checkUpdated);

int main(){
	// thay doi kich thuoc console
	resizeConsole(1500, 780);
	
	// vo hieu hoa thay doi kich thuoc man hinh
	DisableResizeWindow();
	
	DSSV ds;
	init(ds);
	DSINDEX dsId;
	mainMenu(ds, dsId);
	
	return 0;
}

void mainMenu(DSSV &ds, DSINDEX &dsId){
	// cai dat box
	int x = 53;
	int y = 8;
	int w = 70;
	int h = 1;
	int t_color = 15;
	int t_color_active = 10;
	int b_color = 0;
	int b_color_active = 0;
	bool border = true;
	int border_color = 15;
	int border_color_active = 10;
	int n = 8;
	int type = 0;
	bool center = true;
	
	// chuc nang
	string texts[n];
	texts[0] = "1. Doc file danh sach sinh vien.";
	texts[1] = "2. Tao va sap xep bang chi muc.";
	texts[2] = "3. Them mot sinh vien.";
	texts[3] = "4. Xoa mot sinh vien.";
	texts[4] = "5. Chinh sua mot sinh vien.";
	texts[5] = "6. Tim kiem mot sinh vien theo ma sinh vien.";
	texts[6] = "7. Loc va hien thi mot danh sach sinh vien theo lop.";
	texts[7] = "8. Thoat.";
	
	int choose;
	int chooseActive = 0;
	bool checkUpdated = false;
	
	do{
		inVien();
		
		// luu y
		SetColor(0, 14);
		gotoXY(51, h + 2 + n *3);
		cout << "Luu y:" << endl;
		gotoXY(51, h + 3 + n *3);
		cout << "       + Bam phim mui ten len, xuong hoac phim so (1 -> 8) de chon chuc nang." << endl;
		gotoXY(51, h + 4 + n *3);
		cout << "       + Bam phim enter de thuc hien chuc nang." << endl;
		SetColor(0, 15);
		
		// box chuc nang
		n_box(x, y, w, h, t_color, b_color, texts, center, border, border_color, n);
		
		// active
		box(x, y + (chooseActive * (h + 1)), w, h + 1, t_color_active, b_color_active, texts[chooseActive], center, border, border_color_active,  chooseActive == 0 ? 1 : (chooseActive == n - 1 ? 3 : 2));
		
		// toa do active
		int xp = x;
		int yp = y + (chooseActive * (h + 1));
		
		// toa do cu
		int xcu = xp;
		int ycu = yp;
		
		while(true){
			// an nhap lieu
			ShowCur(false);
			
			// index
			int box_index_cu = (ycu - y)/(h + 1);
			int box_index_active = (yp - y)/(h + 1);
			
			// deactive
			box(xcu, ycu, w, h + 1, t_color, b_color, texts[box_index_cu], center, border, border_color, box_index_cu == 0 ? 1 : (box_index_cu == n - 1 ? 3 : 2));
			xcu = xp;
			ycu = yp;
			
			// active
			box(xp, yp, w, h + 1, t_color_active, b_color, texts[box_index_active], center, border, border_color_active, box_index_active == 0 ? 1 : (box_index_active == n - 1 ? 3 : 2));
			
			// dieu khien
			char key_press = getch();
			int key_value = (int)key_press;
			if(key_value == 72){ // len
				if(yp != y){
					yp -= h + 1;
				}else {
					yp = y + (h + 1) * (n - 1);
				}
			} else if(key_value == 80){ // xuong
				if(yp != y + (h + 1) * (n - 1)){
					yp += h + 1;
				}else {
					yp = y;
				}
			} else if(key_value >= 49 && key_value <= (n + 48)){ // 1 -> 8
				yp = y + (h + 1) * (key_value - 49);
			} else if(key_value == 13){ // enter
				choose = box_index_active;
				chooseActive = box_index_active;
				break;
			}
		}
				
		system("cls"); // xoa man hinh console		
		switch(choose){
			case 0: // doc file
				box(72, 2, 40, 1, 15, 0, "DOC FILE DANH SACH SINH VIEN", true, false, 0, 0);
				docFile(ds);
				cout << "\t\t\t " << "Danh sach sinh vien: " << "\n\n\n";
				xuat(ds);
				break;
			case 1: // tao va sap xep bang chi muc
				box(72, 2, 40, 1, 15, 0, "TAO VA SAP XEP BANG CHI MUC", true, false, 0, 0);
				cout << "\n\n\n\n\n\t\t\t " << "Danh sach sinh vien: " << "\n\n\n";
				xuat(ds);
				
				if(ds.count == 0){
					break;
				}
				
				createAndSortIdTable(ds, dsId);
				break;
			case 2: // them sinh vien
				box(72, 2, 40, 1, 15, 0, "THEM MOT SINH VIEN", true, false, 0, 0);
				cout << "\n\n\n\n\n\t\t\t " << "Danh sach sinh vien: " << "\n\n\n";
				xuat(ds);
				cout << "\n\n\n\t\t\t " << "Bang chi muc: " << "\n\n\n";
				xuat(dsId);
				
				addSV(ds, dsId);
				checkUpdated = true;
				
				cout << "\n\n\n\t\t\t " << "Danh sach sinh vien sau khi them: " << "\n\n\n";
				xuat(ds);
				cout << "\n\n\n\t\t\t " << "Bang chi muc sau khi them: " << "\n\n\n";
				xuat(dsId);
				break;
			case 3: // xoa sinh vien
				box(72, 2, 40, 1, 15, 0, "XOA MOT SINH VIEN", true, false, 0, 0);
				cout << "\n\n\n\n\n\t\t\t " << "Danh sach sinh vien: " << "\n\n\n";
				xuat(ds);
				cout << "\n\n\n\t\t\t " << "Bang chi muc: " << "\n\n\n";
				xuat(dsId);
				
				if(ds.count == 0 || dsId.count == 0){
					break;
				}
				
				delSV(ds, dsId);
				checkUpdated = true;
				
				cout << "\n\n\n\t\t\t " << "Danh sach sinh vien sau khi xoa: " << "\n\n\n";
				xuat(ds);
				cout << "\n\n\n\t\t\t " << "Bang chi muc sau khi xoa: " << "\n\n\n";
				xuat(dsId);
				break;
			case 4: // chinh sua thong tin sinh vien
				box(72, 2, 40, 1, 15, 0, "CHINH SUA THONG TIN CUA MOT SINH VIEN", true, false, 0, 0);
				cout << "\n\n\n\n\n\t\t\t " << "Danh sach sinh vien: " << "\n\n\n";
				xuat(ds);
				cout << "\n\n\n\t\t\t " << "Bang chi muc: " << "\n\n\n";
				xuat(dsId);
				
				if(ds.count == 0 || dsId.count == 0){
					break;
				}
				
				editSV(ds, dsId);
				checkUpdated = true;
				
				cout << "\n\n\n\t\t\t " << "Danh sach sinh vien sau khi chinh sua: " << "\n\n\n";
				xuat(ds);
				cout << "\n\n\n\t\t\t " << "Bang chi muc sau khi chinh sua: " << "\n\n\n";
				xuat(dsId);
				break;
			case 5: // tim kiem sinh vien
				box(72, 2, 40, 1, 15, 0, "TIM KIEM MOT SINH VIEN THEO MA SINH VIEN", true, false, 0, 0);
				cout << "\n\n\n\n\n\t\t\t " << "Danh sach sinh vien: " << "\n\n\n";
				xuat(ds);
				cout << "\n\n\n\t\t\t " << "Bang chi muc: " << "\n\n\n";
				xuat(dsId);
				
				if(ds.count == 0 || dsId.count == 0){
					break;
				}
				
				searchSV(ds, dsId);
				break;
			case 6: // loc sinh vien theo lop va luu ket qua ra file
				box(68, 2, 40, 1, 15, 0, "LOC VA HIEN THI MOT DANH SACH SINH VIEN THEO LOP", true, false, 0, 0);
				cout << "\n\n\n\n\n\t\t\t " << "Danh sach sinh vien: " << "\n\n\n";
				xuat(ds);
				cout << "\n\n\n\t\t\t " << "Bang chi muc: " << "\n\n\n";
				xuat(dsId);
				
				if(ds.count == 0 || dsId.count == 0){
					break;
				}
			
				filterSV(ds, dsId);
				break;
			case 7: // ket thuc
				exitAndSave(ds, checkUpdated);
				break;
		}
		
		if(choose != n - 1){
			cout << "\n\n\n\n\n\t\t\t\t\t\t\t\t\t   " << "Nhan phim bat ky de tiep tuc ..." << "\n\n\n";
			getch();
			system("cls"); // xoa man hinh console
		}
	}while(choose != n - 1);
}

void docFile(DSSV &ds){
	string linkFile = "E:/PT-TKGT/cuoiky/quanlysinhvien-giaodien/sinhVien_input.csv";
	cout << "\n\n\n\n\n\t\t\t\t\t" << "Duong dan den File can doc mac dinh la: " << linkFile << endl;
	
	ifstream fileIn; // chi doc file
	fileIn.open(linkFile.c_str()); // mo file va chuyen kieu string link file qua const char* ham .c_str()
	if(fileIn.fail()){
		cout << "\n\n\t\t\t\t\t" << "Khong tim thay File :( \n\n" << endl;
	}else{
		init(ds);
		sinhVien T;
		string line;
		char phay;
		getline(fileIn, line);
		while(true)
		{
			fileIn >> T.maSV;
			fileIn >> phay; // bo dau ','
			
			getline(fileIn, T.hoDem, ','); // lay den truoc dau ','
			getline(fileIn, T.ten, ',');
			getline(fileIn, T.queQuan, ',');
			
			getline(fileIn, T.khoa, ',');
			getline(fileIn, T.lop, ',');
			getline(fileIn, T.khoaHoc);
			
			if(fileIn.eof()){ // kiem tra trong file rong hay ko
				break;
			}
			
			ds.dssv[ds.count++] = T;
		}
		
		cout << "\n\n\t\t\t\t\t" << "Doc file thanh cong. \n\n" << endl;
		//	end
	}
	// dong file
	
	fileIn.close();
}

void createAndSortIdTable(DSSV ds, DSINDEX &dsId){
	dsId = idTable(ds); // tao bang index
	cout << "\n\n\n\t\t\t " << "Bang chi muc sau khi tao: " << "\n\n\n";
	xuat(dsId);
	
	bubbleSort(dsId); // sap xep tang dan (noi bot)
	cout << "\n\n\n\t\t\t " << "Bang chi muc sau khi sap xep: " << "\n\n\n";
	xuat(dsId);
}

void addSV(DSSV &ds, DSINDEX &dsId){
	sinhVien T;
	bool check;
	do {
		cout << "\n\n\t\t\t\t\t\t" << "Nhap thong tin cua sinh vien can them:" << "\n\n\n\n";
		nhapInfo(T, "add");
		check = false;
		for(int i = 0; i < ds.count; i++){ // kiem tra trung ma sinh vien
			if(ds.dssv[i].maSV == T.maSV){
				check = true;
				break;
			}
		}
		
		if(check){
			SetColor(0, 12);
			cout << "\n\n\t\t\t\t\t\t" << "Sinh vien da co trong danh sach. Moi ban nhap lai!" << "\n\n";
			SetColor(0, 15);
		}
	}while(check);
	
	ds.dssv[ds.count++] = T; // them sinh vien vao danh sach goc va tang count len
	
	int i = dsId.count - 1;
	while(dsId.dsIndex[i].maSV > T.maSV && i >= 0){ // tim kiem vi tri maSV phu hop trong bang index
		dsId.dsIndex[i + 1] = dsId.dsIndex[i];
		i--;
	}
	// them sinh vien vao bang index
	dsId.dsIndex[i + 1].maSV = T.maSV;
	dsId.dsIndex[i + 1].lop = T.lop;
	dsId.dsIndex[i + 1].id = ds.count - 1;
	dsId.count++;
}

void delSV(DSSV &ds, DSINDEX &dsId){
	int maSV;
	int vt;
	ShowCur(true);
	do {
		cout << "\n\n\n\n\n\t\t\t\t\t\t" << "Nhap ma sinh vien can xoa: ";
		int yMa = wherey() - 1;
		box(75, yMa, 30, 2, 15, 0, "", false, true, 15, 0);
		gotoXY(77, yMa + 1);
		cin >> maSV;
		
		vt = binarySearch(dsId, maSV);
		if(vt == -1){
			SetColor(0, 12);
			cout << "\n\n\t\t\t\t\t\t" << "Khong tim thay sinh vien co ma " << maSV << ". Moi ban nhap lai!" << "\n\n";
			SetColor(0, 15);
		}
	}while(vt == -1);
	ShowCur(false);
	
	cout << "\n\n\t\t\t" << "Sinh vien duoc tim thay: " << "\n\n";
	inTieuDe();
	xuatInfo(ds.dssv[dsId.dsIndex[vt].id], dsId.dsIndex[vt].id + 1);
	
	int x = 25;
	int y = wherey();
	
	// fix box
	gotoXY(x + 7, y);
	cout << char(193);
	
	gotoXY(x + 22, y);
	cout << char(193);
	
	gotoXY(x + 47, y);
	cout << char(193);
	
	gotoXY(x + 57, y);
	cout << char(193);
	
	gotoXY(x + 77, y);
	cout << char(193);
	
	gotoXY(x + 107, y);
	cout << char(193);
	
	gotoXY(x + 117, y);
	cout << char(193);
	
	for(int i = dsId.dsIndex[vt].id; i < ds.count ; i++){ // xoa sinh vien trong danh sach goc
		ds.dssv[i] = ds.dssv[i + 1];
	}
	ds.count--;
	
	int id = dsId.dsIndex[vt].id;
	for(int i = vt; i < dsId.count ; i++){ // xoa sinh vien trong bang index
		dsId.dsIndex[i] = dsId.dsIndex[i + 1];
	}
	dsId.count--;
	
	for(int i = 0; i < dsId.count; i++){ // cap nhat lai id cho cac sinh vien co id > id sinh vien da xoa
		if(dsId.dsIndex[i].id > id){
			dsId.dsIndex[i].id--;
		}
	}
}

void editSV(DSSV &ds, DSINDEX &dsId){
	int maSV;
	int vt;
	ShowCur(true);
	do {
		cout << "\n\n\n\n\n\t\t\t\t\t\t" << "Nhap ma sinh vien can chinh sua: ";
		int yMa = wherey() - 1;
		box(85, yMa, 30, 2, 15, 0, "", false, true, 15, 0);
		gotoXY(87, yMa + 1);
		cin >> maSV;
		
		vt = binarySearch(dsId, maSV);
		if(vt == -1){
			SetColor(0, 12);
			cout << "\n\n\t\t\t\t\t\t" << "Khong tim thay sinh vien co ma " << maSV << ". Moi ban nhap lai!" << "\n\n";
			SetColor(0, 15);
		}
	}while(vt == -1);
	ShowCur(false);
	
	cout << "\n\n\t\t\t" << "Sinh vien duoc tim thay: " << "\n\n";
	inTieuDe();
	xuatInfo(ds.dssv[dsId.dsIndex[vt].id], dsId.dsIndex[vt].id + 1);
	
	int x = 25;
	int y = wherey();
	
	// fix box
	gotoXY(x + 7, y);
	cout << char(193);
	
	gotoXY(x + 22, y);
	cout << char(193);
	
	gotoXY(x + 47, y);
	cout << char(193);
	
	gotoXY(x + 57, y);
	cout << char(193);
	
	gotoXY(x + 77, y);
	cout << char(193);
	
	gotoXY(x + 107, y);
	cout << char(193);
	
	gotoXY(x + 117, y);
	cout << char(193);
	
	sinhVien T = ds.dssv[dsId.dsIndex[vt].id];
	bool check;
	do {
		cout << "\n\n\n\n\n\t\t\t\t\t\t" << "Nhap thong tin moi cua sinh vien can chinh sua:" << "\n\n\n\n";
		nhapInfo(T, "edit");
		check = false;
		for(int i = 0; i < ds.count; i++){ // kiem tra trung ma sinh vien (tru ma sinh vien dang chinh sua)
			if(ds.dssv[i].maSV == T.maSV && i != dsId.dsIndex[vt].id){
				check = true;
				break;
			}
		}
		
		if(check){
			SetColor(0, 12);
			cout << "\n\n\t\t\t\t\t\t" << "Sinh vien da co trong danh sach. Moi ban nhap lai!" << "\n\n";
			SetColor(0, 15);
		}
	}while(check);
	
	ds.dssv[dsId.dsIndex[vt].id] = T; // cap nhat sinh vien trong danh sach goc
	
	int id = dsId.dsIndex[vt].id;
	for(int i = vt; i < dsId.count - 1; i++){ // xoa sinh vien da chinh sua
		dsId.dsIndex[i] = dsId.dsIndex[i + 1];
	}
	dsId.count--;
	
	int i = dsId.count - 1;
	while(dsId.dsIndex[i].maSV > T.maSV && i >= 0){ // tim kiem vi tri maSV phu hop trong bang index
		dsId.dsIndex[i + 1] = dsId.dsIndex[i];
		i--;
	}
	dsId.count++;
	
	// cap nhat sinh vien trong bang index
	dsId.dsIndex[i + 1].maSV = T.maSV;
	dsId.dsIndex[i + 1].lop = T.lop;
	dsId.dsIndex[i + 1].id = id;
}

void searchSV(DSSV ds, DSINDEX dsId){
	int maSV;
	int vt;
	ShowCur(true);
	do {
		cout << "\n\n\n\n\n\t\t\t\t\t\t" << "Nhap ma sinh vien can tim: ";
		int yMa = wherey() - 1;
		box(80, yMa, 30, 2, 15, 0, "", false, true, 15, 0);
		gotoXY(82, yMa + 1);
		cin >> maSV;
		
		vt = binarySearch(dsId, maSV);
		if(vt == -1){
			SetColor(0, 12);
			cout << "\n\n\t\t\t\t\t\t" << "Khong tim thay sinh vien co ma " << maSV << ". Moi ban nhap lai!" << "\n\n";
			SetColor(0, 15);
		}
	}while(vt == -1);
	ShowCur(false);
	
	cout << "\n\n\t\t\t" << "Sinh vien duoc tim thay: " << "\n\n";
	inTieuDe();
	xuatInfo(ds.dssv[dsId.dsIndex[vt].id], dsId.dsIndex[vt].id + 1);
	
	int x = 25;
	int y = wherey();
	
	// fix box
	gotoXY(x + 7, y);
	cout << char(193);
	
	gotoXY(x + 22, y);
	cout << char(193);
	
	gotoXY(x + 47, y);
	cout << char(193);
	
	gotoXY(x + 57, y);
	cout << char(193);
	
	gotoXY(x + 77, y);
	cout << char(193);
	
	gotoXY(x + 107, y);
	cout << char(193);
	
	gotoXY(x + 117, y);
	cout << char(193);
}

void filterSV(DSSV ds, DSINDEX dsId){
	string lop;
	ShowCur(true);
	cout << "\n\n\n\n\n\t\t\t\t\t\t" << "Nhap ten lop can loc: ";
	int yLop = wherey() - 1;
	box(73, yLop, 30, 2, 15, 0, "", false, true, 15, 0);
	gotoXY(75, yLop + 1);
	fflush(stdin);
	getline(cin, lop);
	ShowCur(false);
	
	string linkFile = "E:/PT-TKGT/cuoiky/quanlysinhvien-giaodien/sinhVien_output.txt";
	cout << "\n\n\n\t\t\t\t\t\t" << "Duong dan den File can ghi mac dinh la: " << linkFile << endl;
	
	ofstream fileOut;; // chi ghi file
	fileOut.open(linkFile.c_str()); // mo file va chuyen kieu string link file qua const char* ham .c_str()
	
	int checkEmpty = true; // kiem tra rong
	cout << "\n\n\t\t\t" << "Danh sach sau khi loc theo lop " << lop << " :" << "\n\n\n";
	inTieuDe();
	
	// ghi vao file
	fileOut << "\n\t" << "Danh sach sau khi loc theo lop " << lop << " :" << "\n\n\n";
	inTieuDeToFile(fileOut);
	
	for(int i = 0; i < dsId.count; i++){
		if(dsId.dsIndex[i].lop == lop){
			xuatInfo(ds.dssv[dsId.dsIndex[i].id], dsId.dsIndex[i].id + 1);
			// ghi vao file
			xuatInfoToFile(fileOut, ds.dssv[dsId.dsIndex[i].id]);
			checkEmpty = false;
		}
	}
	
	if(checkEmpty){
		cout << "\n\n\t\t\t\t\t\t\t\t\t" << "Khong co sinh vien nao trong danh sach" << endl;
		fileOut << "\n\n\t\t\t\t\t\t\t\t\t\t\t" << "Khong co sinh vien nao trong danh sach" << endl;
	}
	
	int x = 25;
	int y = wherey();
	
	// fix box
	gotoXY(x + 7, y);
	cout << char(193);
	
	gotoXY(x + 22, y);
	cout << char(193);
	
	gotoXY(x + 47, y);
	cout << char(193);
	
	gotoXY(x + 57, y);
	cout << char(193);
	
	gotoXY(x + 77, y);
	cout << char(193);
	
	gotoXY(x + 107, y);
	cout << char(193);
	
	gotoXY(x + 117, y);
	cout << char(193);
	
	SetColor(0, 10);
	cout << "\n\n\n\n\t\t\t\t\t\t\t\t\t\t" << "Ghi file thanh cong." << endl;
	SetColor(0, 15);
	
	fileOut.close();
}

void exitAndSave(DSSV ds, bool checkUpdated){
	if(checkUpdated){
		string linkFile = "E:/PT-TKGT/cuoiky/quanlysinhvien-giaodien/sinhVien_input.csv";
		
		ofstream fileOut;; // chi ghi file
		fileOut.open(linkFile.c_str()); // mo file va chuyen kieu string link file qua const char* ham .c_str()
		
		for(int i = 0; i < ds.count; i++){
			fileOut << ds.dssv[i].maSV << "," << ds.dssv[i].hoDem << "," << ds.dssv[i].ten << "," << ds.dssv[i].queQuan
					<< "," << ds.dssv[i].khoa << "," << ds.dssv[i].lop << "," << ds.dssv[i].khoaHoc << endl;
		}
		
		fileOut.close();
		
		int x = 63;
		int y = 15;
		
		box(x, y, 60, 6, 15, 10, "Danh sach da duoc cap nhat va luu vao file Excel !", true, true, 15, 0);
		box(x + 24, y, 15, 1, 15, 12, "Thong bao", true, false, 0, 0);
		box(x + 19, y + 6, 25, 1, 15, 12, "Dong sau 2 giay", true, false, 0, 0);
		Sleep(2000);
	}
	
	box(1, 1, 182, 40, 12, 0, "CHUONG TRINH KET THUC ...", true, false, 0, 0);
	gotoXY(10, 36);
}

void box(int x, int y, int w, int h, int t_color, int b_color, string text, bool center, bool border, int border_color, int type){
	if(border && h > 0 && w > 0){ // tao border
		// doi mau border
		SetColor(b_color, border_color);
		
		// in chieu ngang
		for(int ix = x; ix <= x + w; ix++){
			gotoXY(ix, y);
			cout << char(196);
			gotoXY(ix, y + h);
			cout << char(196);
		}
		
		// in chieu doc
		for(int iy = y; iy <= y + h; iy++){
			gotoXY(x, iy);
			cout << char(179);
			gotoXY(x + w, iy);
			cout << char(179);
		}
		
		// in cac goc
		if(type == 0 || type == 1){
			gotoXY(x, y);
			cout << char(218); // trai tren
			gotoXY(x + w, y);
			cout << char(191); // phai tren
		}else{
			gotoXY(x, y); // trai tren
			cout << char(195);
			gotoXY(x + w, y); // phai tren
			cout << char(180);
		}
		
		if(type == 0 || type == 3){
			gotoXY(x, y + h);
			cout << char(192); // trai duoi
			gotoXY(x + w, y + h);
			cout << char(217); // phai duoi
		}else{
			gotoXY(x, y + h); // trai duoi
			cout << char(195);
			gotoXY(x + w, y + h); // phai duoi
			cout << char(180);
		}
		
		// tang vi tri
		x++;
		y++;
	}
	
	// doi mau background, text
	SetColor(b_color, t_color);
	
	int textLength = text.length();
	if(center){
		string newText = "";
		if(textLength != 0){
			for(int i = 0; i < (w - textLength - 2)/2; i++){
				newText += " ";
				text += " ";
			}
			
			if((w - textLength - 2) % 2 == 1){
				text += " ";
			}
		}
		
		string space = "";
		for(int i = 0; i < w - 1; i++){
			space += " ";
		}
		
		for(int i = 0; i < h / 2; i++){
			gotoXY(x, y + i);
			cout << space;
		}
		
		int h_bonus = h%2 == 1 ? h / 2 : h / 2 - 1;
		if(textLength != 0){
			gotoXY(x, y + h_bonus);
			cout << newText + text;
		}else {
			gotoXY(x, y + h_bonus);
			cout << space;
		}
		
		for(int i = h / 2; i < h - 1; i++){
			gotoXY(x, y + i);
			cout << space;
		}
	}else {
		if(textLength != 0){
			for(int i = textLength; i < w - 2; i++){
				text += " ";
			}
			
			gotoXY(x, y);
			cout << " " << text;
		}else {
			gotoXY(x, y);
			for(int i = 0; i < w - 1; i++){
				cout << " ";
			}
		}
	}
	// doi mau ve mac dinh
	SetColor(0, 15);
}

void n_box(int x, int y, int w, int h, int t_color, int b_color, string *texts, bool center, bool border, int border_color, int n){
	h++;
	for(int i = 0; i < n; i++){
		box(x, y + (i * h), w, h, t_color, b_color, texts[i], center, border, border_color, i == 0 ? 1 : (i == n - 1 ? 3 : 2));
	}
	
	SetColor(0, 15);
}

void inVien(){
	// cai dat giao dien menu
	SetColor(0, 15);
	// cai dat box
	int x = 0;
	int y = 0;
	int w = 182;
	int h = 40;
	int t_color = 15;
	int b_color = 0;
	bool border = false;
	int border_color = 0;
	int type = 0;
	bool center = true;
	
	box(x, y, w, h, t_color, b_color, "", center, border, border_color, type);
	
	// in chieu ngang
	for(int ix = x; ix <= x + w; ix++){
		gotoXY(ix, y);
		cout << char(205);
		gotoXY(ix, y + h);
		cout << char(205);
	}
	
	// in chieu doc
	for(int iy = y; iy <= y + h; iy++){
		gotoXY(x, iy);
		cout << char(186);
		gotoXY(x + w, iy);
		cout << char(186);
	}
	
	// in cac goc
	gotoXY(x, y);
	cout << char(201); // trai tren
	gotoXY(x + w, y);
	cout << char(187); // phai tren
	
	gotoXY(x, y + h);
	cout << char(200); // trai duoi
	gotoXY(x + w, y + h);
	cout << char(188); // phai duoi
	
	// tieu de
	box(72, 0, 40, 1, 15, 0, "CHUONG TRINH QUAN LY SINH VIEN", true, false, 0, 0);
}

string convertString(int x){
	stringstream ss;
	ss<<x;
	string s;  
	ss>>s;
	return s;
}

int convertInt(string s){
	stringstream intSS;
	intSS<<s;
	int x;
	intSS>>x;
	return x;
}

void init(DSSV &ds){
	ds.count = 0;
}

void nhapInfo(sinhVien &sv, string action){
	// cai dat box
	int x = 70;
	int w = 30;
	int h = 2;
	int t_color = 15;
	int b_color = 0;
	bool border = true;
	int border_color = 15;
	int type = 0;
	bool center = false;
	
	cout << "\t\t\t\t\t\t" << "Nhap ma sinh vien: ";
	int yMa = wherey() - 1;
	box(x, yMa, w, h, t_color, b_color, "", center, border, border_color, type);
	
	cout << "\n\n\n\t\t\t\t\t\t" << "Nhap ho dem: ";
	int yHD = wherey() - 1;
	box(x, yHD, w, h, t_color, b_color, "", center, border, border_color, type);
	
	cout << "\n\n\n\t\t\t\t\t\t" << "Nhap ten: ";
	int yTen = wherey() - 1;
	box(x, yTen, w, h, t_color, b_color, "", center, border, border_color, type);
	
	cout << "\n\n\n\t\t\t\t\t\t" << "Nhap que quan: ";
	int yQQ = wherey() - 1;
	box(x, yQQ, w, h, t_color, b_color, "", center, border, border_color, type);
	
	cout << "\n\n\n\t\t\t\t\t\t" << "Nhap khoa: ";
	int yKhoa = wherey() - 1;
	box(x, yKhoa, w, h, t_color, b_color, "", center, border, border_color, type);
	
	cout << "\n\n\n\t\t\t\t\t\t" << "Nhap lop: ";
	int yLop = wherey() - 1;
	box(x, yLop, w, h, t_color, b_color, "", center, border, border_color, type);
	
	cout << "\n\n\n\t\t\t\t\t\t" << "Nhap khoa hoc: ";
	int yKH = wherey() - 1;
	box(x, yKH, w, h, t_color, b_color, "", center, border, border_color, type);
	
	ShowCur(true);
	
	if(action == "edit"){		
		string maSVTerm = convertString(sv.maSV);
		gotoXY(x + 2, yMa + 1);
		cout << maSVTerm;
		
		string hoDemTerm = sv.hoDem;
		gotoXY(x + 2, yHD + 1);
		cout << hoDemTerm;
		
		string tenTerm = sv.ten;
		gotoXY(x + 2, yTen + 1);
		cout << tenTerm;
		
		string queQuanTerm = sv.queQuan;
		gotoXY(x + 2, yQQ + 1);
		cout << queQuanTerm;
		
		string khoaTerm = sv.khoa;
		gotoXY(x + 2, yKhoa + 1);
		cout << khoaTerm;
		
		string lopTerm = sv.lop;
		gotoXY(x + 2, yLop + 1);
		cout << lopTerm;
		
		string khoaHocTerm = sv.khoaHoc;
		gotoXY(x + 2, yKH + 1);
		cout << khoaHocTerm;
		
		int inp = 2;
		for(int i = 0; i < maSVTerm.length(); i++){
			inp++;
		}
		
		while(true){
			gotoXY(x + inp, yMa + 1);
			char key_press = getch();
			int key_value = (int)key_press;
			
			if(key_value >= 48 && key_value <= 57 && inp < w - 2){
				maSVTerm += key_press;
				cout << key_press;
				inp++;
			}else if(key_value == 8 && inp > 2){
				string maSVTermOld = maSVTerm;
				maSVTerm = "";
				for(int i = 0; i < maSVTermOld.length() - 1; i++){
					maSVTerm += maSVTermOld[i];
				}
				inp--;
				gotoXY(x + inp, yMa + 1);
				cout << " ";
			}else if(key_value == 13){
				sv.maSV = convertInt(maSVTerm);
				break;
			}
		}
	
		inp = 2;
		for(int i = 0; i < hoDemTerm.length(); i++){
			inp++;
		}
		
		while(true){
			gotoXY(x + inp, yHD + 1);
			char key_press = getch();
			int key_value = (int)key_press;
			
			if(((key_value >= 48 && key_value <= 57) || (key_value >= 65 && key_value <= 90)
				|| (key_value >= 97 && key_value <= 122) || key_value == 32 || key_value == 95) && inp < w - 2){
				hoDemTerm += key_press;
				cout << key_press;
				inp++;
			}else if(key_value == 8 && inp > 2){
				string hoDemTermOld = hoDemTerm;
				hoDemTerm = "";
				for(int i = 0; i < hoDemTermOld.length() - 1; i++){
					hoDemTerm += hoDemTermOld[i];
				}
				inp--;
				gotoXY(x + inp, yHD + 1);
				cout << " ";
			}else if(key_value == 13){
				sv.hoDem = hoDemTerm;
				break;
			}
		}
		
		inp = 2;
		for(int i = 0; i < tenTerm.length(); i++){
			inp++;
		}
		
		while(true){
			gotoXY(x + inp, yTen + 1);
			char key_press = getch();
			int key_value = (int)key_press;
			
			if(((key_value >= 48 && key_value <= 57) || (key_value >= 65 && key_value <= 90)
				|| (key_value >= 97 && key_value <= 122) || key_value == 32 || key_value == 95) && inp < w - 2){
				tenTerm += key_press;
				cout << key_press;
				inp++;
			}else if(key_value == 8 && inp > 2){
				string tenTermOld = tenTerm;
				tenTerm = "";
				for(int i = 0; i < tenTermOld.length() - 1; i++){
					tenTerm += tenTermOld[i];
				}
				inp--;
				gotoXY(x + inp, yTen + 1);
				cout << " ";
			}else if(key_value == 13){
				sv.ten = tenTerm;
				break;
			}
		}
		
		inp = 2;
		for(int i = 0; i < queQuanTerm.length(); i++){
			inp++;
		}
		
		while(true){
			gotoXY(x + inp, yQQ + 1);
			char key_press = getch();
			int key_value = (int)key_press;
			
			if(((key_value >= 48 && key_value <= 57) || (key_value >= 65 && key_value <= 90)
				|| (key_value >= 97 && key_value <= 122) || key_value == 32 || key_value == 95) && inp < w - 2){
				queQuanTerm += key_press;
				cout << key_press;
				inp++;
			}else if(key_value == 8 && inp > 2){
				string queQuanTermOld = queQuanTerm;
				queQuanTerm = "";
				for(int i = 0; i < queQuanTermOld.length() - 1; i++){
					queQuanTerm += queQuanTermOld[i];
				}
				inp--;
				gotoXY(x + inp, yQQ + 1);
				cout << " ";
			}else if(key_value == 13){
				sv.queQuan = queQuanTerm;
				break;
			}
		}
		
		inp = 2;
		for(int i = 0; i < khoaTerm.length(); i++){
			inp++;
		}
		
		while(true){
			gotoXY(x + inp, yKhoa + 1);
			char key_press = getch();
			int key_value = (int)key_press;
			
			if(((key_value >= 48 && key_value <= 57) || (key_value >= 65 && key_value <= 90)
				|| (key_value >= 97 && key_value <= 122) || key_value == 32 || key_value == 95) && inp < w - 2){
				khoaTerm += key_press;
				cout << key_press;
				inp++;
			}else if(key_value == 8 && inp > 2){
				string khoaTermOld = khoaTerm;
				khoaTerm = "";
				for(int i = 0; i < khoaTermOld.length() - 1; i++){
					khoaTerm += khoaTermOld[i];
				}
				inp--;
				gotoXY(x + inp, yKhoa + 1);
				cout << " ";
			}else if(key_value == 13){
				sv.khoa = khoaTerm;
				break;
			}
		}
		
		inp = 2;
		for(int i = 0; i < lopTerm.length(); i++){
			inp++;
		}
		
		while(true){
			gotoXY(x + inp, yLop + 1);
			char key_press = getch();
			int key_value = (int)key_press;
			
			if(((key_value >= 48 && key_value <= 57) || (key_value >= 65 && key_value <= 90)
				|| (key_value >= 97 && key_value <= 122) || key_value == 32 || key_value == 95) && inp < w - 2){
				lopTerm += key_press;
				cout << key_press;
				inp++;
			}else if(key_value == 8 && inp > 2){
				string lopTermOld = lopTerm;
				lopTerm = "";
				for(int i = 0; i < lopTermOld.length() - 1; i++){
					lopTerm += lopTermOld[i];
				}
				inp--;
				gotoXY(x + inp, yLop + 1);
				cout << " ";
			}else if(key_value == 13){
				sv.lop = lopTerm;
				break;
			}
		}

		inp = 2;
		for(int i = 0; i < khoaHocTerm.length(); i++){
			inp++;
		}
		
		while(true){
			gotoXY(x + inp, yKH + 1);
			char key_press = getch();
			int key_value = (int)key_press;
			
			if(((key_value >= 48 && key_value <= 57) || (key_value >= 65 && key_value <= 90)
				|| (key_value >= 97 && key_value <= 122) || key_value == 32 || key_value == 95) && inp < w - 2){
				khoaHocTerm += key_press;
				cout << key_press;
				inp++;
			}else if(key_value == 8 && inp > 2){
				string khoaHocTermOld = khoaHocTerm;
				khoaHocTerm = "";
				for(int i = 0; i < khoaHocTermOld.length() - 1; i++){
					khoaHocTerm += khoaHocTermOld[i];
				}
				inp--;
				gotoXY(x + inp, yKH + 1);
				cout << " ";
			}else if(key_value == 13){
				sv.khoaHoc = khoaHocTerm;
				break;
			}
		}
	}else {
		gotoXY(x + 2, yMa + 1);
		cin >> sv.maSV;
		
		gotoXY(x + 2, yHD + 1);
		fflush(stdin);
		getline(cin, sv.hoDem);
		
		gotoXY(x + 2, yTen + 1);
		fflush(stdin);
		getline(cin, sv.ten);
		
		gotoXY(x + 2, yQQ + 1);
		fflush(stdin);
		getline(cin, sv.queQuan);
		
		gotoXY(x + 2, yKhoa + 1);
		fflush(stdin);
		getline(cin, sv.khoa);
		
		gotoXY(x + 2, yLop + 1);
		fflush(stdin);
		getline(cin, sv.lop);
		
		gotoXY(x + 2, yKH + 1);
		fflush(stdin);
		getline(cin, sv.khoaHoc);
	}
	ShowCur(false);
}

void inTieuDe(){
	int x = 25;
	int y = wherey();
	int h = 2;
	int t_color = 10;
	int b_color = 0;
	bool center = false;
	bool border = true;
	int border_color = 15;
	int type = 0;
	
	box(x, y, 7, h, t_color, b_color, "STT", center, border, border_color, type);
	box(x + 7, y, 15, h, t_color, b_color, "Ma sinh vien", center, border, border_color, type);
	box(x + 22, y, 25, h, t_color, b_color, "Ho dem", center, border, border_color, type);
	box(x + 47, y, 10, h, t_color, b_color, "Ten", center, border, border_color, type);
	box(x + 57, y, 20, h, t_color, b_color, "Que quan", center, border, border_color, type);
	box(x + 77, y, 30, h, t_color, b_color, "Khoa", center, border, border_color, type);
	box(x + 107, y, 10, h, t_color, b_color, "Lop", center, border, border_color, type);
	box(x + 117, y, 15, h, t_color, b_color, "Khoa hoc", center, border, border_color, type);
	
	SetColor(b_color, border_color);
	
	gotoXY(x + 7, y);
	cout << char(194);
	
	gotoXY(x + 22, y);
	cout << char(194);
	
	gotoXY(x + 47, y);
	cout << char(194);
	
	gotoXY(x + 57, y);
	cout << char(194);
	
	gotoXY(x + 77, y);
	cout << char(194);
	
	gotoXY(x + 107, y);
	cout << char(194);
	
	gotoXY(x + 117, y);
	cout << char(194);
	
	gotoXY(x, y + 2);
	SetColor(0, 15);
}

void inTieuDeToFile(ofstream &fileOut){
	fileOut << "\t"
		 << setw(15) << left << "Ma sinh vien"
		 << setw(25) << left << "Ho dem"
		 << setw(10) << left << "Ten"
		 << setw(20) << left << "Que quan"
		 << setw(30) << left << "Khoa"
		 << setw(10) << left << "Lop"
		 << setw(10) << left << "Khoa hoc" << endl;
		 
	fileOut << "\t" << setfill('-') << setw(120) << '-' << endl;
	fileOut << setfill(' ');
}

void inTieuDeIndex(){	
	int x = 72;
	int y = wherey();
	int h = 2;
	int t_color = 10;
	int b_color = 0;
	bool center = false;
	bool border = true;
	int border_color = 15;
	int type = 0;
	
	box(x, y, 7, h, t_color, b_color, "ID", center, border, border_color, type);
	box(x + 7, y, 20, h, t_color, b_color, "Ma sinh vien", center, border, border_color, type);
	box(x + 27, y, 10, h, t_color, b_color, "Lop", center, border, border_color, type);
	
	SetColor(b_color, border_color);
	
	gotoXY(x + 7, y);
	cout << char(194);
	
	gotoXY(x + 27, y);
	cout << char(194);
	
	gotoXY(x, y + 2);
	cout << char(195);
	
	gotoXY(x, y + 2);
	SetColor(0, 15);
}

void xuatInfo(sinhVien sv, int stt){
	int x = 25;
	int y = wherey();
	int h = 2;
	int t_color = 15;
	int b_color = 0;
	bool center = false;
	bool border = true;
	int border_color = 15;
	int type = 0;
	
	box(x, y, 7, h, t_color, b_color, convertString(stt), center, border, border_color, type);
	box(x + 7, y, 15, h, t_color, b_color, convertString(sv.maSV), center, border, border_color, type);
	box(x + 22, y, 25, h, t_color, b_color, sv.hoDem, center, border, border_color, type);
	box(x + 47, y, 10, h, t_color, b_color, sv.ten, center, border, border_color, type);
	box(x + 57, y, 20, h, t_color, b_color, sv.queQuan, center, border, border_color, type);
	box(x + 77, y, 30, h, t_color, b_color, sv.khoa, center, border, border_color, type);
	box(x + 107, y, 10, h, t_color, b_color, sv.lop, center, border, border_color, type);
	box(x + 117, y, 15, h, t_color, b_color, sv.khoaHoc, center, border, border_color, type);
	
	SetColor(b_color, border_color);
	
	gotoXY(x, y);
	cout << char(195);
	
	gotoXY(x + 7, y);
	cout << char(197);
	
	gotoXY(x + 22, y);
	cout << char(197);
	
	gotoXY(x + 47, y);
	cout << char(197);
	
	gotoXY(x + 57, y);
	cout << char(197);
	
	gotoXY(x + 77, y);
	cout << char(197);
	
	gotoXY(x + 107, y);
	cout << char(197);
	
	gotoXY(x + 117, y);
	cout << char(197);
	
	gotoXY(x + 132, y);
	cout << char(180);
	
	gotoXY(0, y + 2);
	SetColor(0, 15);
}

void xuatInfoToFile(ofstream &fileOut, sinhVien sv){
	fileOut << "\t"
			<< setw(15) << left << sv.maSV
			<< setw(25) << left << sv.hoDem
			<< setw(10) << left << sv.ten
			<< setw(20) << left << sv.queQuan
			<< setw(30) << left << sv.khoa
			<< setw(10) << left << sv.lop
			<< setw(10) << left << sv.khoaHoc << endl;
}

void xuat(DSSV ds){
	inTieuDe();
	if(ds.count == 0){
		int x = wherex();
		int y = wherey();
		
		box(x, y, 132, 2, 15, 0, "Khong co sinh vien nao", true, true, 15, 0);
		
		gotoXY(x, y);
		cout << char(195);
		
		gotoXY(x + 7, y);
		cout << char(193);
		
		gotoXY(x + 22, y);
		cout << char(193);
		
		gotoXY(x + 47, y);
		cout << char(193);
		
		gotoXY(x + 57, y);
		cout << char(193);
		
		gotoXY(x + 77, y);
		cout << char(193);
		
		gotoXY(x + 107, y);
		cout << char(193);
		
		gotoXY(x + 117, y);
		cout << char(193);
		
		gotoXY(x + 132, y);
		cout << char(180);
		
		gotoXY(x, y + 3);
	}else {
		for(int i = 0; i < ds.count; i++){
			xuatInfo(ds.dssv[i], i + 1);
		}
		
		int x = 25;
		int y = wherey();
		
		gotoXY(x, y);
		cout << char(192);
		
		gotoXY(x + 7, y);
		cout << char(193);
		
		gotoXY(x + 22, y);
		cout << char(193);
		
		gotoXY(x + 47, y);
		cout << char(193);
		
		gotoXY(x + 57, y);
		cout << char(193);
		
		gotoXY(x + 77, y);
		cout << char(193);
		
		gotoXY(x + 107, y);
		cout << char(193);
		
		gotoXY(x + 117, y);
		cout << char(193);
	}
}

void xuat(DSINDEX dsId){
	inTieuDeIndex();
	if(dsId.count == 0){
		int x = wherex();
		int y = wherey();
		
		box(x, y, 37, 2, 15, 0, "Khong co sinh vien nao", true, true, 15, 0);
		
		gotoXY(x, y);
		cout << char(195);
		
		gotoXY(x + 7, y);
		cout << char(193);
		
		gotoXY(x + 27, y);
		cout << char(193);
		
		gotoXY(x + 37, y);
		cout << char(180);
		
		gotoXY(x, y + 3);
	}else {
		int x = 72;
		int h = 2;
		int t_color = 15;
		int b_color = 0;
		bool center = false;
		bool border = true;
		int border_color = 15;
		int type = 0;
		
		for(int i = 0; i < dsId.count; i++){
			int y = wherey();
			box(x, y, 7, h, t_color, b_color, convertString(dsId.dsIndex[i].id), center, border, border_color, type);
			box(x + 7, y, 20, h, t_color, b_color, convertString(dsId.dsIndex[i].maSV), center, border, border_color, type);
			box(x + 27, y, 10, h, t_color, b_color, dsId.dsIndex[i].lop, center, border, border_color, type);
			
			SetColor(b_color, border_color);
			
			gotoXY(x, y);
			cout << char(195);
			
			gotoXY(x + 7, y);
			cout << char(197);
			
			gotoXY(x + 27, y);
			cout << char(197);
			
			gotoXY(x + 37, y);
			cout << char(180);
			
			gotoXY(x, y + 2);
			SetColor(0, 15);
		}
		
		int y = wherey();
		
		gotoXY(x, y);
		cout << char(192);
		
		gotoXY(x + 7, y);
		cout << char(193);
		
		gotoXY(x + 27, y);
		cout << char(193);
	}
}

void xuat(DSSV ds, DSINDEX dsId){
	inTieuDe();
	if(dsId.count == 0){
		int x = wherex();
		int y = wherey();
		
		box(x, y, 132, 2, 15, 0, "Khong co sinh vien nao", true, true, 15, 0);
		
		gotoXY(x, y);
		cout << char(195);
		
		gotoXY(x + 7, y);
		cout << char(193);
		
		gotoXY(x + 22, y);
		cout << char(193);
		
		gotoXY(x + 47, y);
		cout << char(193);
		
		gotoXY(x + 57, y);
		cout << char(193);
		
		gotoXY(x + 77, y);
		cout << char(193);
		
		gotoXY(x + 107, y);
		cout << char(193);
		
		gotoXY(x + 117, y);
		cout << char(193);
		
		gotoXY(x + 132, y);
		cout << char(180);
		
		gotoXY(x, y + 3);
	}else {
		for(int i = 0; i < dsId.count; i++){
			xuatInfo(ds.dssv[dsId.dsIndex[i].id], dsId.dsIndex[i].id + 1);
		}
		
		int x = 25;
		int y = wherey();
		
		gotoXY(x, y);
		cout << char(192);
		
		gotoXY(x + 7, y);
		cout << char(193);
		
		gotoXY(x + 22, y);
		cout << char(193);
		
		gotoXY(x + 47, y);
		cout << char(193);
		
		gotoXY(x + 57, y);
		cout << char(193);
		
		gotoXY(x + 77, y);
		cout << char(193);
		
		gotoXY(x + 107, y);
		cout << char(193);
		
		gotoXY(x + 117, y);
		cout << char(193);
	}
}

void swap(index &x, index &y){
	index temp = x;
	x = y;
	y = temp;
}

DSINDEX idTable(DSSV ds){
	DSINDEX dsId;
	dsId.count = ds.count;
	
	for(int i = 0; i < ds.count; i ++){
		dsId.dsIndex[i].maSV = ds.dssv[i].maSV;
		dsId.dsIndex[i].lop = ds.dssv[i].lop;
		dsId.dsIndex[i].id = i;
	}
	
	return dsId;
}

void bubbleSort(DSINDEX &dsId){
	for(int i = 0; i < dsId.count - 1; i++){
		for(int j = dsId.count - 1; j > i; j--){
			if(dsId.dsIndex[j].maSV < dsId.dsIndex[j-1].maSV){
				swap(dsId.dsIndex[j], dsId.dsIndex[j-1]);
			}
		}
	}
}

int binarySearch(DSINDEX dsId, int maSV){
	int l = 0;
	int r = dsId.count - 1;
	int median;
	
	while(l <= r){
		median = (l + r) / 2;
		if(dsId.dsIndex[median].maSV == maSV){
			return median;
		}
		
		if(maSV > dsId.dsIndex[median].maSV){
			l = median + 1;
		}else {
			r = median - 1;
		}
	}
	
	return -1;
}

// config
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void DisableResizeWindow()
{
    HWND hWnd = GetConsoleWindow();
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}

void SetColor(int backgound_color, int text_color)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    int color_code = backgound_color * 16 + text_color;
    SetConsoleTextAttribute(hStdout, color_code);
}

void gotoXY(int x, int y)
{
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position;
    Position.X = x;
    Position.Y = y;

	SetConsoleCursorPosition(hStdout, Position);
}

int wherex() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
}

int wherey() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
}
