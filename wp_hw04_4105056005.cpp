// wp_hw04_4105056005.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>
#include <sstream>
using namespace std;

//印出過往事件
class Cshow
{
public:
	string* showstr;
	fstream* showFile;
	ifstream* printFile;
	string* stemp;

	Cshow() {
		showstr = new string;
		stemp = new string;
		remove("ShowEvent.txt");
	}

	~Cshow() {
		delete showstr;
		delete stemp;
	}
	//儲存在ShowEvent文字檔案中
	void storeShow() {
		showFile = new fstream;
		(*showFile).open("ShowEvent.txt", ios::out | ios::app);
		if ((*showFile).is_open()) {
			(*showFile) << *showstr << endl;
			(*showFile).close();
		}
		delete showFile;
	}
	//印出ShowEvent檔案中的所有文字
	void printShow() {
		printFile = new ifstream;
		(*printFile).open("ShowEvent.txt", ifstream::in);
		if ((*printFile).is_open()) {
			while (getline(*printFile, *stemp)) {
				cout << *stemp << endl;
			}
			(*printFile).close();
		}
		delete printFile;
	}

};

//開始遊戲，決定人數、真實玩家數、玩家是否存活
class CStart
{
public:
	int* people;
	int* realPlayer;
	int* dead;
	string* strtemp;	//用來做輸入ENTER防呆
	int* i;				//迴圈變數
	int* restart;

	CStart() {
		strtemp = new string;
		dead = new int[4];
		i = new int;
		restart = new int;
		*restart = 0;

		for (*i = 0; *i < 4; (*i)++)
			*(dead + *i) = 1;
	}

	~CStart() {
		delete people;
		delete realPlayer;
		delete[] dead;
		delete strtemp;
		delete i;
		delete restart;
	}

	//印出初始規則
	void printStart() {
		cout << setw(70) << "-歡迎來到奇幻世界大富翁-" << endl << endl;
		cout << "遊戲說明:\n    此遊戲開始金額建議為1500元，每次經過起點將獲得300元。" << endl;
		cout << "    每次行動可選擇擲骰子(d)或是重新開始整個遊戲(r)。可以存檔、讀檔與刪除所有紀錄檔。" << endl;
		cout << "    踩上土地方塊時，可以決定是否購買房子，或是繼續升級房屋，一塊土地最多升到5級房子。若土地已有該玩家蓋的房子，則可收取保護費。"
			<< "站到其他玩家的土地上，則會被收取過路費。" << endl;
		cout << "    踩到機會及命運方塊，系統將自動抽牌，並執行牌上的指令。" << endl;
		cout << "    踩到抽稅方塊，玩家將減少200元。若擁有逃稅卡牌將會自動使用。" << endl;
		cout << "   存檔都在同一個檔案Save.txt，讀檔也是從Save.txt讀取，若弄亂文字的格式會造成讀檔錯誤。可選擇任何存檔紀錄。" << endl << endl;

		cout << "遊戲背景:\n    在某個奇幻世界，荒蕪的大地上充滿怪物與暴力的居民，而你是一個沒沒無聞但充滿野心的人類。\n";
		cout << "    你此生的夢想，就是在這片土地上建造自己的王國，但你並不是唯一想要統治這世界的人。\n";
		cout << "    用你手中的金幣，建造廣大的殖民地，並盡力讓你的敵人破產。最終口袋裡還有金幣的人，就是贏家...\n\n";
	}

	//輸入總共遊玩人數和真人玩家數量，有做輸入防呆
	void howManyPeople(Cshow* show) {
		(*(*show).showstr).append("遊戲開始\n");
		cout << "請輸入總共遊玩人數(2到4人): ";
		people = new int;
		do {
			if (!(cin >> *people)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "非整數輸入，請重新輸入2到4: ";
				continue;
			}
			if (*people <= 4 && *people >= 2)
				break;
			cout << "輸入範圍錯誤，請重新輸入2到4: ";
		} while (true);
		cin.ignore();

		cout << "有幾位真實玩家一同遊玩呢?請輸入數字(1代表自己玩，2代表二號玩家是真人操作，3代表二與三號玩家是真人操作，以此類推): ";
		realPlayer = new int;
		do {
			if (!(cin >> *realPlayer)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "非整數輸入，請重新輸入: ";
				continue;
			}
			if (*realPlayer <= 0 || *realPlayer > 4)
				cout << "輸入範圍錯誤，請重新輸入1到4: ";
			else if (*realPlayer > *people)
				cout << "輸入錯誤，並沒有那麼多位玩家喔~請輸入" << *people << "以下的數: ";
			else
				break;
		} while (true);
		cin.ignore();
		cout << endl;
		(*(*show).showstr).append("遊玩總人數為" + to_string(*people) + "人");
		(*(*show).showstr).append("，真實玩家數為" + to_string(*realPlayer) + "人。");
		(*show).storeShow();
		*(*show).showstr = "";
	}
};

//玩家類別，包含名字、金錢、位置、逃稅卡、玩家編號
class Cplayer
{
public:
	string* name;
	int* money;
	int* position;
	int* notax;
	int* playerNumber;
	int* buyStock;

	Cplayer() {		//初始值歸零
		money = new int;
		*money = 0;
		position = new int;
		*position = 0;
		notax = new int;
		*notax = 0;
		playerNumber = new int;
		buyStock = new int[20];

		int *x = new int;
		for (*x = 0; *x < 10; (*x)++) {
			buyStock[(*x) * 2] = 0;
			buyStock[(*x) * 2 + 1] = 0;
		}
		delete x;
	}

	~Cplayer() {
		delete name;
		delete money;
		delete position;
		delete notax;
		delete playerNumber;
		delete buyStock;
	}

	//輸入真實玩家名字
	void inputName() {
		name = new string;
		getline(cin, *name);
	}

	//輸入起始金額，只需輸入一次，所有人的金額都會相同
	void inputMoney() {
		cout << "輸入遊戲起始金額(建議1500元，不要輸入負數): ";
		do {
			if (!(cin >> *money)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "非整數輸入，請重新輸入金額: ";
				continue;
			}
			if (*money > 0)
				break;
			cout << "輸入範圍錯誤，請重新輸入金額: ";
		} while (true);
		cin.ignore();
	}

	void setMoney(Cplayer* p1) {
		*(this->money) = *(*p1).money;
	}
};

//股票類別
class Cstock {
public:
	string* company;
	int* price;
	int* oldprice;
	int* updown;
	int* i;
	char* ptemp;
	int* which;
	int* howmany;

	Cstock() {
		company = new string;
		price = new int;
		oldprice = new int;
		updown = new int;
		*updown = 0;
		i = new int;
		ptemp = new char;
		which = new int;
		howmany = new int;
	}

	~Cstock() {
		delete company;
		delete price;
		delete oldprice;
		delete updown;
		delete i;
		delete ptemp;
		delete which;
		delete howmany;
	}

	void buysale(Cplayer* player, Cstock* stock, Cshow* show) {
		(*(*show).showstr).append("查看股票，");
		for (*i = 0; *i < 10; (*i)++) {
			if ((*player).buyStock[(*i) * 2] != 0) {
				cout << "  你擁有股票 ";
				cout << (*i)+1 << "." << *(*(stock + *i)).company << " " << (*player).buyStock[(*i) * 2]
					<< "股 購買共花費" << (*player).buyStock[(*i) * 2 + 1] << "元 ";
				cout << "賣掉可以得到" << (*(*(stock + *i)).price) * ((*player).buyStock[(*i) * 2]) << "元" << endl;
			}
		}
		do {
			cout << "\n輸入n即可跳出股票畫面，輸入b購買股票，輸入s販賣股票:";
			cin.getline(ptemp, 20);
			if (*ptemp == 'b' && *(ptemp + 1) == '\0') {		//購買股票
				(*(*show).showstr).append("選擇購買股票，");
				cout << "請問你要購買哪一個股票?(輸入數字編號):";
				do {
					cin.getline(ptemp, 20);
					*which = atoi(ptemp);
					if (*which <= 0 || *which > 10)
						cout << "輸入範圍錯誤，請重新輸入:";
					else
						break;
				} while (true);
				*which -= 1;
				cout << "請問要購買幾股呢?(輸入數字):";
				cin.getline(ptemp, 20);
				*howmany = atoi(ptemp);
				(*player).buyStock[(*which)*2] += *howmany;
				(*player).buyStock[(*which) * 2 + 1] += (*(stock[*which]).price) * (*howmany);
				
				//防止破產機制
				if ((*howmany) * (*(stock[*which]).price) > *(*player).money) {
					(*(*show).showstr).append("餘額不足，無法購買。\n");
					cout << "餘額不足，無法購買股票" << endl;
					continue;
				}
				*(*player).money -= (*howmany) * (*(stock[*which]).price);
				cout << "購買[" << *(stock[*which]).company << "]，花費" << (*howmany) * (*(stock[*which]).price) << "元，玩家擁有" << *(*player).money << "元。" << endl;
				(*(*show).showstr).append("購買" + *(stock[*which]).company + "的" + to_string((*player).buyStock[(*which) * 2]) + "張股票。\n");
			}
			else if (*ptemp == 's' && *(ptemp + 1) == '\0') {		//販賣股票
				(*(*show).showstr).append("選擇購買股票，");
				*howmany = 0;
				for (*i = 0; *i < 10; (*i)++) {
					if ((*player).buyStock[(*i) * 2] == 0)
						*howmany += 1;
				}
				if (*howmany == 10) {
					(*(*show).showstr).append("沒有可以買賣的股票。\n");
					cout << "沒有可以買賣的股票。" << endl;
					continue;
				}
				for (*i = 0; *i < 10; (*i)++) {
					if ((*player).buyStock[(*i) * 2] != 0) {
						cout << "  你擁有股票 ";
						cout << (*i) + 1 << "." << *(*(stock + *i)).company << " " << (*player).buyStock[(*i) * 2]
							<< "股 購買共花費" << (*player).buyStock[(*i) * 2 + 1] << "元 ";
						cout << "賣掉可以得到" << (*(*(stock + *i)).price) * ((*player).buyStock[(*i) * 2]) << "元" << endl;
					}
				}
				cout << "請問你要賣哪一家股票呢?(輸入股票公司號碼):";
				do {
					cin.getline(ptemp, 20);
					*which = atoi(ptemp);
					if (*which <= 0 || *which > 10)
						cout << "輸入範圍錯誤，請重新輸入:";
					else if ((*player).buyStock[((*which) - 1) * 2] == 0)
						cout << "並沒有此間公司的股票，請重新輸入:";
					else
						break;
				} while (true);
				*which -= 1;
				*howmany = (*player).buyStock[(*which) * 2];
				(*player).buyStock[(*which) * 2] = 0;
				(*player).buyStock[(*which) * 2 + 1] = 0;
				*(*player).money += (*(stock[*which]).price) * (*howmany);
				cout << "獲得" << (*(stock[*which]).price) * (*howmany) << "元，玩家擁有" << *(*player).money << "元。" << endl;
				(*(*show).showstr).append("賣掉" + *(stock[*which]).company + "的股票，賺到" + to_string((*(stock[*which]).price) * (*howmany)) + "元。\n");
			}
			else if (*ptemp == 'n' && *(ptemp + 1) == '\0')
				break;
			else
				cout << "輸入錯誤。";
		} while (true);
		cout << endl;
		(*show).storeShow();
		*(*show).showstr = "";
	}

	void printStock(Cstock* stock) {
		cout << "\n        -股票波動表-(單位:元)" << endl;
		cout << "   股票名稱 \t\t股價 \t\t升降 \t\t漲跌" << endl;
		for (*i = 0; *i < 10; (*i)++) {
			cout << *i + 1 << ". " << *(*(stock + *i)).company << "\t\t" << *(*(stock + *i)).price << "\t\t";
			if (*(*(stock + *i)).updown == 0)	//無升降
				cout << " - ";
			else if (*(*(stock + *i)).updown == -1)	//下跌
				cout << "  v";
			else if (*(*(stock + *i)).updown == 1)	//上漲
				cout << "^  ";
			cout << "\t\t" << *(*(stock + *i)).price - *(*(stock + *i)).oldprice << endl;
		}
		cout << endl;
	}

	//隨機波動
	void priceChange(Cstock* stock) {
		for (*i = 0; *i < 10; (*i)++)		//紀錄舊價格
			*(*(stock + *i)).oldprice = *(*(stock + *i)).price;
		srand(time(NULL));
		for (*i = 0; *i < 10; (*i)++) {
			*(*(stock + *i)).price += (rand() % 200) - 100;
			if (*(*(stock + *i)).price < 0)		//股票價值不低於0元
				*(*(stock + *i)).price = 0;
			if (*(*(stock + *i)).oldprice > *(*(stock + *i)).price)
				*(*(stock + *i)).updown = -1;
			else if(*(*(stock + *i)).oldprice < *(*(stock + *i)).price)
				*(*(stock + *i)).updown = 1;
			else
				*(*(stock + *i)).updown = 0;
		}
	}
};


//土地類別，包含名稱、價格、擁有者、房子數量
class CLands
{
private:
	int* pi;
	int* pj;
	int* pfront;
	int* pback;

public:
	char** landName;
	int* cost;
	int* owner;
	int* houses;

	CLands(){
		landName = new char*[30];
		cost = new int[30];
		owner = new int[30];
		houses = new int[30];

		pi = new int;
		for (*pi = 0; *pi < 30; (*pi)++) {
			*(houses + (*pi)) = 0;
			*(owner + (*pi)) = 0;
		}
	}

	~CLands(){
		delete[] landName;
		delete[] cost;
		delete[] owner;
		delete[] houses;
	}

	//設定地圖地名與價格
	void setmap() {
		*landName = "Start  ->";				*(landName + 1) = "Hobbiton";		*(landName + 2) = "EchoIsles";
		*(landName + 3) = "Citadel";			*(landName + 4) = "Midgar";			*(landName + 5) = "Gotham";
		*(landName + 6) = "CHANCE";				*(landName + 7) = "Hideout";		*(landName + 8) = "SouthPark";
		*(landName + 9) = "Waterdeep";			*(landName + 10) = "   -TAX-   ";	*(landName + 11) = "Silent Hill";
		*(landName + 12) = "Racoon City";		*(landName + 13) = "Los Santos";	*(landName + 14) = "District X";
		*(landName + 15) = "Snowdin";			*(landName + 16) = "Grillby's";		*(landName + 17) = "COMMUNITY";
		*(landName + 18) = "Skyrim";			*(landName + 19) = "Helheim";		*(landName + 20) = "Outland";
		*(landName + 21) = "Ravenholm";			*(landName + 22) = "Asgard";		*(landName + 23) = "Detroit";
		*(landName + 24) = "Alfheim";			*(landName + 25) = "   -TAX-   ";	*(landName + 26) = "Highway 26";
		*(landName + 27) = "Gravity Falls";		*(landName + 28) = "Black Beach";	*(landName + 29) = "Sea of light";

		*(cost) = 0;			*(cost + 1) = 90;		*(cost + 2) = 110;
		*(cost + 3) = 120;		*(cost + 4) = 80;		*(cost + 5) = 100;
		*(cost + 6) = 0;		*(cost + 7) = 100;		*(cost + 8) = 150;
		*(cost + 9) = 160;		*(cost + 10) = 200;		*(cost + 11) = 190;
		*(cost + 12) = 150;		*(cost + 13) = 220;		*(cost + 14) = 170;
		*(cost + 15) = 400;		*(cost + 16) = 170;		*(cost + 17) = 0;
		*(cost + 18) = 220;		*(cost + 19) = 250;		*(cost + 20) = 160;
		*(cost + 21) = 200;		*(cost + 22) = 180;		*(cost + 23) = 230;
		*(cost + 24) = 250;		*(cost + 25) = 200;		*(cost + 26) = 300;
		*(cost + 27) = 320;		*(cost + 28) = 160;		*(cost + 29) = 320;
	}

	void printmap(Cplayer* p1, Cplayer* p2, Cplayer* p3, Cplayer* p4, CStart* start) {
		pi = new int;
		pj = new int;
		pfront = new int;
		pback = new int;
		*pfront = 0;
		*pback = 29;
		//***頂端橫線***
		for (*pi = 0; *pi < 120; (*pi)++)
			cout << "-";

		//***上方格子***
		for (*pj = 0; *pj < 4; (*pj)++) {
			//印出房子
			if (*pj == 0) {
				if (*(houses + (*pfront)) > 0)
					cout << "|    " << *(owner + (*pfront)) << "'s house";
				else
					cout << "|             ";
				(*pfront)++;
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*(houses + (*pfront)) > 0)
						cout << "|" << *(owner + (*pfront)) << "'s house";
					else
						cout << "|         ";
					(*pfront)++;
				}
				if (*(houses + (*pfront)) > 0)
					cout << "|    " << *(owner+(*pfront)) << "'s house| ";
				else
					cout << "|             | ";
				(*pfront)++;
			}
			//印出地名
			else if (*pj == 1) {
				*pfront = 0;
				cout << "|" << setw(13) << *(landName + (*pfront));
				(*pfront)++;
				for (*pi = 0; *pi < 9; (*pi)++) {
					cout << "|" << setw(9) << *(landName + (*pfront));
					(*pfront)++;
				}
				cout << "|" << setw(13) << *(landName + (*pfront)) << "| ";
				(*pfront)++;
			}
			//印出房價
			else if (*pj == 2) {
				*pfront = 1;
				cout << "|     起點    ";
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*pfront == 6) {
						cout << "|   機會  ";
						(*pfront)++;
						continue;
					}
					cout << "|" << setw(7) << *(cost + (*pfront)) << "元";
					(*pfront)++;
				}
				cout << "|   $$$$$$$   | ";
				(*pfront)++;
			}
			//印出玩家位置
			else if (*pj == 3) {
				*pfront = 0;
				if (*(*p1).position == (*pfront) && *(*start).dead == 0)
					cout << "|□";
				else
					cout << "|  ";
				if (*(*p2).position == (*pfront) && *((*start).dead + 1) == 0)
					cout << "■";
				else
					cout << "  ";
				if (*(*p3).position == (*pfront) && *(*start).people > 2 && *((*start).dead + 2) == 0)
					cout << "△";
				else
					cout << "  ";
				if (*(*p4).position == (*pfront) && *(*start).people > 3 && *((*start).dead + 3) == 0)
					cout << "▲     ";
				else
					cout << "       ";
				(*pfront)++;
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*(*p1).position == (*pfront) && *(*start).dead == 0)
						cout << "|□";
					else
						cout << "|  ";
					if (*(*p2).position == (*pfront) && *((*start).dead + 1) == 0)
						cout << "■";
					else
						cout << "  ";
					if (*(*p3).position == (*pfront) && *(*start).people > 2 && *((*start).dead + 2) == 0)
						cout << "△";
					else
						cout << "  ";
					if (*(*p4).position == (*pfront) && *(*start).people > 3 && *((*start).dead + 3) == 0)
						cout << "▲ ";
					else
						cout << "   ";
					(*pfront)++;
				}
				if (*(*p1).position == (*pfront) && *(*start).dead == 0)
					cout << "|□";
				else
					cout << "|  ";
				if (*(*p2).position == (*pfront) && *((*start).dead + 1) == 0)
					cout << "■";
				else
					cout << "  ";
				if (*(*p3).position == (*pfront) && *(*start).people > 2 && *((*start).dead + 2) == 0)
					cout << "△";
				else
					cout << "  ";
				if (*(*p4).position == (*pfront) && *(*start).people > 3 && *((*start).dead + 3) == 0)
					cout << "▲     | ";
				else
					cout << "       | ";
				(*pfront)++;
			}
		}

		//***橫線***
		for (*pi = 0; *pi < 120; (*pi)++)
			cout << '-';

		//***中間格子***
		for (*pi = 0; *pi < 4; (*pi)++) {
			for (*pj = 0; *pj < 4; (*pj)++) {
				//印出房屋
				if (*pj == 0) {
					if (*(houses + (*pback)) > 0 && *pi == 0)
						cout << "|    " <<  *(owner+(*pback)) << "'s house|       □:1th player        ■:2th player    ";
					else if (*pi == 0)
						cout << "|             |       □:1th player        ■:2th player    ";
					else if (*(houses + (*pback)) > 0)
						cout << "|    " << *(owner+(*pback)) << "'s house|                                             ";
					else
						cout << "|             |                                             ";
					if (*(houses+(*pfront)) > 0 && *pi == 0)
						cout << "   △:3th player       ▲:4th player        |    " << *(owner+(*pfront)) << "'s house| ";
					else if (*pi == 0)
						cout << "   △:3th player       ▲:4th player        |             | ";
					else if (*(houses + (*pfront)) > 0)
						cout << "                                            |    " << *(owner+(*pfront)) << "'s house| ";
					else
						cout << "                                            |             | ";
				}
				//印出地名
				else if (*pj == 1) {
					cout << "|" << setw(13) << *(landName + (*pback)) << "|                                             ";
					cout << "                                            |" << setw(13) << *(landName + (*pfront)) << "| ";
				}
				//印出價格
				else if (*pj == 2) {
					cout << "|" << setw(11) << *(cost + (*pback)) << "元|                                             ";
					cout << "                                            |" << setw(11) << *(cost + (*pfront)) << "元| ";
				}
				//印出玩家位置
				else if (*pj == 3) {
					//左邊格子
					if (*(*p1).position == *pback && *(*start).dead == 0)
						cout << "|□";
					else
						cout << "|  ";
					if (*(*p2).position == *pback && *((*start).dead + 1) == 0)
						cout << "■";
					else
						cout << "  ";
					if (*(*p3).position == *pback && *(*start).people > 2 && *((*start).dead + 2) == 0)
						cout << "△";
					else
						cout << "  ";
					if (*(*p4).position == *pback && *(*start).people > 3 && *((*start).dead + 3) == 0)
						cout << "▲     |                                             ";
					else
						cout << "       |                                             ";
					//右邊格子
					if (*(*p1).position == *pfront && *(*start).dead == 0)
						cout << "                                            |□";
					else
						cout << "                                            |  ";
					if (*(*p2).position == *pfront && *((*start).dead + 1) == 0)
						cout << "■";
					else
						cout << "  ";
					if (*(*p3).position == *pfront && *(*start).people > 2 && *((*start).dead + 2) == 0)
						cout << "△";
					else
						cout << "  ";
					if (*(*p4).position == *pfront && *(*start).people > 3 && *((*start).dead + 3) == 0)
						cout << "▲     | ";
					else
						cout << "       | ";
					(*pfront)++;
					(*pback)--;
				}
				else {
					cout << "|             |                                             ";
					cout << "                                            |             | ";
				}
			}
			if (*pi != 3) {
				cout << "--------------                                                  ";
				cout << "                                        --------------- ";
			}
		}

		//***橫線***
		for (*pi = 0; *pi < 120; (*pi)++)
			cout << '-';

		//***下方格子***
		for (*pj = 0; *pj < 4; (*pj)++) {
			//印出房子
			if (*pj == 0) {
				if (*(houses + (*pback)) > 0)
					cout << "|    " << *(owner + (*pback)) << "'s house";
				else
					cout << "|             ";
				(*pback)--;
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*(houses + (*pback)) > 0)
						cout << "|" << *(owner + (*pback)) << "'s house";
					else
						cout << "|         ";
					(*pback)--;
				}
				if (*(houses + (*pback)) > 0)
					cout << "|    " << *(owner + (*pback)) << "'s house| ";
				else
					cout << "|             | ";
				(*pback)--;
			}
			//印出地名
			else if (*pj == 1) {
				*pback = 25;
				cout << "|" << setw(13) << *(landName + (*pback));
				(*pback)--;
				for (*pi = 0; *pi < 9; (*pi)++) {
					cout << "|" << setw(9) << *(landName + (*pback));
					(*pback)--;
				}
				cout << "|" << setw(13) << *(landName + (*pback)) << "| ";
				(*pback)--;
			}
			//印出房價
			else if (*pj == 2) {
				*pback = 24;
				cout << "|   $$$$$$$   ";
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*pi == 7) {
						cout << "|   命運  ";
						(*pback)--;
						continue;
					}
					cout << "|" << setw(7) << *(cost + (*pback)) << "元";
					(*pback)--;
				}
				cout << "|" << setw(11) << *(cost + (*pback)) << "元| ";
				(*pback)--;
			}
			//印出玩家位置
			else if (*pj == 3) {
				*pback = 25;
				if (*(*p1).position == (*pback) && *(*start).dead == 0)
					cout << "|□";
				else
					cout << "|  ";
				if (*(*p2).position == (*pback) && *((*start).dead + 1) == 0)
					cout << "■";
				else
					cout << "  ";
				if (*(*p3).position == (*pback) && *(*start).people > 2 && *((*start).dead + 2) == 0)
					cout << "△";
				else
					cout << "  ";
				if (*(*p4).position == (*pback) && *(*start).people > 3 && *((*start).dead + 3) == 0)
					cout << "▲     ";
				else
					cout << "       ";
				(*pback)--;
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*(*p1).position == (*pback) && *(*start).dead == 0)
						cout << "|□";
					else
						cout << "|  ";
					if (*(*p2).position == (*pback) && *((*start).dead + 1) == 0)
						cout << "■";
					else
						cout << "  ";
					if (*(*p3).position == (*pback) && *(*start).people > 2 && *((*start).dead + 2) == 0)
						cout << "△";
					else
						cout << "  ";
					if (*(*p4).position == (*pback) && *(*start).people > 3 && *((*start).dead + 3) == 0)
						cout << "▲ ";
					else
						cout << "   ";
					(*pback)--;
				}
				if (*(*p1).position == (*pback) && *(*start).dead == 0)
					cout << "|□";
				else
					cout << "|  ";
				if (*(*p2).position == (*pback) && *((*start).dead + 1) == 0)
					cout << "■";
				else
					cout << "  ";
				if (*(*p3).position == (*pback) && *(*start).people > 2 && *((*start).dead + 2) == 0)
					cout << "△";
				else
					cout << "  ";
				if (*(*p4).position == (*pback) && *(*start).people > 3 && *((*start).dead + 3) == 0)
					cout << "▲     | ";
				else
					cout << "       | ";
				(*pback)--;
			}
		}

		//***橫線***
		for (*pi = 0; *pi < 120; (*pi)++) 
			cout << '-';

		delete pi;
		delete pj;
		delete pfront;
		delete pback;
	}
};

class CGameloop
{
public:
	int* dice;
	int* itemp;
	char* ptemp;
	string* stemp;
	fstream *saveFile;
	ifstream *loadFile;
	int* count;	//用來數檔案數量
	int* lines;	//紀錄讀檔行數
	int* findWord; //紀錄檔尋找分割點
	int* cutWord;	//紀錄分割點
	string* words;
	int* i;	//for迴圈用
	
	CGameloop() {
		dice = new int;
		itemp = new int;
		ptemp = new char[30];
		stemp = new string;
		saveFile = new fstream;
		loadFile = new ifstream;
		words = new string;
		i = new int;
	}

	~CGameloop(){
		delete dice;
		delete itemp;
		delete[] ptemp;
		delete stemp;
		delete saveFile;
		delete loadFile;
		delete count;
		delete lines;
		delete i;
	}

	//存檔
	void saveText(Cplayer* p1, Cplayer* p2, Cplayer* p3, Cplayer* p4, CLands* map, CStart* start) {
		(*saveFile).open("Save.txt", ios::out | ios::app);
		if ((*saveFile).is_open()) {
			(*saveFile) << "\nSave file:" << endl;
			(*saveFile) << "Player one's name:" << *(*p1).name << endl;
			(*saveFile) << "Player two's name:" << *(*p2).name << endl;
			(*saveFile) << "Player three's name:" << *(*p3).name << endl;
			(*saveFile) << "Player four's name:" << *(*p4).name << endl;

			(*saveFile) << "Player one's money:" << *(*p1).money << endl;
			(*saveFile) << "Player two's money:" << *(*p2).money << endl;
			(*saveFile) << "Player three's money:" << *(*p3).money << endl;
			(*saveFile) << "Player four's money:" << *(*p4).money << endl;

			(*saveFile) << "Number of players:" << *(*start).people << endl;
			(*saveFile) << "Number of real players:" << *(*start).realPlayer << endl;

			(*saveFile) << "Players' position:" << *(*p1).position << "/" << *(*p2).position << "/" <<
				*(*p3).position << "/" << *(*p4).position << "/" << endl;

			(*saveFile) << "Players' notax card:" << *(*p1).notax << "/" << *(*p2).notax << "/" << 
				*(*p3).notax << "/" << *(*p4).notax << "/" << endl;

			(*saveFile) << "Players are alive or not:" << *(*start).dead << "/" << *((*start).dead + 1) << "/" <<
				*((*start).dead + 2) << "/" << *((*start).dead + 3) << "/" << endl;

			(*saveFile) << "Cost of each place from 00 to 29:";
			for (*itemp = 0; *itemp < 30; (*itemp)++)
				(*saveFile) << *((*map).cost + (*itemp)) << "/";
			(*saveFile) << endl;

			(*saveFile) << "Number of houses on each place from 00 to 29:";
			for (*itemp = 0; *itemp < 30; (*itemp)++)
				(*saveFile) << *((*map).houses + (*itemp)) << "/";
			(*saveFile) << endl;

			(*saveFile) << "owner of each place from 00 to 29:";
			for (*itemp = 0; *itemp < 30; (*itemp)++)
				(*saveFile) << *((*map).owner + (*itemp)) << "/";
			(*saveFile) << endl;

			
			(*saveFile).close();
			cout << "儲存成功。" << endl;
		}
		return;
	}
	
	//讀檔
	void loadtext(Cplayer* p1, Cplayer* p2, Cplayer* p3, Cplayer* p4, CLands* map, CStart* start) {
		(*loadFile).open("Save.txt", ifstream::in);
		if ((*loadFile).is_open()) {
			count = new int;
			*count = 0;
			//數出有幾個檔案
			while (getline(*loadFile, *stemp)) {
				if ((*stemp).compare("Save file:") == 0)
					*count += 1;
			}
			(*loadFile).close();
			if (*count == 0) {
				cout << "無任何存檔紀錄" << endl;
				return;
			}
			cout << "現在有" << *count << "個記錄檔，請輸入數字選擇要第幾個紀錄檔，數字越大紀錄越新: ";
			do {
				if (!(cin >> *itemp)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "非整數輸入，請重新輸入: ";
					continue;
				}
				if (*itemp > 0 && *itemp <= *count)
					break;
				cout << "輸入錯誤，請輸入範圍內的數字: ";
			} while (true);
			cin.ignore();
		}

		(*loadFile).open("Save.txt", ifstream::in);
		*count = 0;
		lines = new int;
		*lines = 1;
		findWord = new int;
		cutWord = new int;
		while (getline(*loadFile, *stemp)) {
			if ((*stemp).compare("Save file:") == 0)
				(*count)++;
			if (*count == *itemp) {
				switch (*lines)
				{
				case 2:		//第二行是玩家一號的名字
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord)+1);
					*(*p1).name = *words;
					break;
				case 3:		//第三行是玩家二號的名字
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*(*p2).name = *words;
					break;
				case 4:		//第四行是玩家三號的名字
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*(*p3).name = *words;
					break;
				case 5:		//第五行是玩家四號的名字
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*(*p4).name = *words;
					break;
				case 6:		//第六行是玩家一號的金錢數量
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*findWord = stoi(*words);
					*(*p1).money = *findWord;
					break;
				case 7:		//第七行是玩家二號的金錢數量
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*findWord = stoi(*words);
					*(*p2).money = *findWord;
					break;
				case 8:		//第八行是玩家三號的金錢數量
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*findWord = stoi(*words);
					*(*p3).money = *findWord;
					break;
				case 9:		//第九行是玩家四號的金錢數量
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*findWord = stoi(*words);
					*(*p4).money = *findWord;
					break;
				case 10:	//第十行是玩家人數
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*findWord = stoi(*words);
					*(*start).people = *findWord;
					break;
				case 11:	//第十一行是真實玩家人數
					*findWord = (*stemp).find(":");
					*words = (*stemp).substr((*findWord) + 1);
					*findWord = stoi(*words);
					*(*start).realPlayer = *findWord;
					break;
				case 12:	//第十二行是玩家位置
					*findWord = (*stemp).find(":");
					*cutWord = (*stemp).find("/");
					*words = (*stemp).substr((*findWord) + 1, (*cutWord - *findWord - 1));	//取出:和/之間的數
					*findWord = stoi(*words);
					*(*p1).position = *findWord;	//存入一號玩家位置
					
					*stemp = (*stemp).substr((*cutWord) + 1);
					*cutWord = (*stemp).find("/");	//取出第二個斜線間的數
					*words = (*stemp).substr(0, *cutWord);
					*findWord = stoi(*words);
					*(*p2).position = *findWord;

					*stemp = (*stemp).substr((*cutWord) + 1);
					*cutWord = (*stemp).find("/");	//取出第三個斜線間的數
					*words = (*stemp).substr(0, *cutWord);
					*findWord = stoi(*words);
					*(*p3).position = *findWord;

					*stemp = (*stemp).substr((*cutWord) + 1);
					*cutWord = (*stemp).find("/");	//取出第四個斜線間的數
					*words = (*stemp).substr(0, *cutWord);
					*findWord = stoi(*words);
					*(*p4).position = *findWord;
					break;
				case 13:	//第十三行是玩家逃稅卡
					*findWord = (*stemp).find(":");
					*cutWord = (*stemp).find("/");
					*words = (*stemp).substr((*findWord) + 1, (*cutWord - *findWord - 1));	//取出:和/之間的數
					*findWord = stoi(*words);
					*(*p1).notax = *findWord;	//存入一號玩家位置

					*stemp = (*stemp).substr((*cutWord) + 1);
					*cutWord = (*stemp).find("/");	//取出第二個斜線間的數
					*words = (*stemp).substr(0, *cutWord);
					*findWord = stoi(*words);
					*(*p2).notax = *findWord;

					*stemp = (*stemp).substr((*cutWord) + 1);
					*cutWord = (*stemp).find("/");	//取出第三個斜線間的數
					*words = (*stemp).substr(0, *cutWord);
					*findWord = stoi(*words);
					*(*p3).notax = *findWord;

					*stemp = (*stemp).substr((*cutWord) + 1);
					*cutWord = (*stemp).find("/");	//取出第四個斜線間的數
					*words = (*stemp).substr(0, *cutWord);
					*findWord = stoi(*words);
					*(*p4).notax = *findWord;
					break;
				case 14:	//第十四行是玩家存活或死亡
					*findWord = (*stemp).find(":");
					*cutWord = (*stemp).find("/");
					*words = (*stemp).substr((*findWord) + 1, (*cutWord - *findWord - 1));	//取出:和/之間的數
					*findWord = stoi(*words);
					*(*start).dead = *findWord;	

					for (*i = 1; *i < 4; (*i)++) {
						*stemp = (*stemp).substr((*cutWord) + 1);
						*cutWord = (*stemp).find("/");	//取出斜線間的數
						*words = (*stemp).substr(0, *cutWord);
						*findWord = stoi(*words);
						*((*start).dead + *i) = *findWord;
					}
					break;
				case 15:	//第十五行是各地房價
					*findWord = (*stemp).find(":");
					*cutWord = (*stemp).find("/");
					*words = (*stemp).substr((*findWord) + 1, (*cutWord - *findWord - 1));	//取出:和/之間的數
					*findWord = stoi(*words);
					*(*map).cost = *findWord;

					for (*i = 1; *i < 30; (*i)++) {
						*stemp = (*stemp).substr((*cutWord) + 1);
						*cutWord = (*stemp).find("/");	//取出斜線間的數
						*words = (*stemp).substr(0, *cutWord);
						*findWord = stoi(*words);
						*((*map).cost + *i) = *findWord;
					}
					break;
				case 16:	//第十六行是各地房屋數量
					*findWord = (*stemp).find(":");
					*cutWord = (*stemp).find("/");
					*words = (*stemp).substr((*findWord) + 1, (*cutWord - *findWord - 1));	//取出:和/之間的數
					*findWord = stoi(*words);
					*(*map).houses = *findWord;

					for (*i = 1; *i < 30; (*i)++) {
						*stemp = (*stemp).substr((*cutWord) + 1);
						*cutWord = (*stemp).find("/");	//取出斜線間的數
						*words = (*stemp).substr(0, *cutWord);
						*findWord = stoi(*words);
						*((*map).houses + *i) = *findWord;
					}
					break;
				case 17:	//第十七行是各地房屋擁有者
					*findWord = (*stemp).find(":");
					*cutWord = (*stemp).find("/");
					*words = (*stemp).substr((*findWord) + 1, (*cutWord - *findWord - 1));	//取出:和/之間的數
					*findWord = stoi(*words);
					*(*map).owner = *findWord;

					for (*i = 1; *i < 30; (*i)++) {
						*stemp = (*stemp).substr((*cutWord) + 1);
						*cutWord = (*stemp).find("/");	//取出斜線間的數
						*words = (*stemp).substr(0, *cutWord);
						*findWord = stoi(*words);
						*((*map).owner + *i) = *findWord;
					}
					break;
				default:
					break;
				}
				(*lines)++;
			}
		}
		delete count;
		delete findWord;
		delete cutWord;
		(*loadFile).close();
	}

	//機會
	void chance(Cplayer* player, Cstock* stock) {
		srand(time(NULL));
		*itemp = rand() % 5;
		switch (*itemp) {
		case 0:
			cout << "一名受傷又迷路的精靈向你求助，你給了他250元旅費。(按下ENTER)";
			*(*player).money -= 250;
			getline(cin, *stemp);
			break;
		case 1:
			cout << "被捲進半獸人戰爭，重傷住院，花700元治療費。(按下ENTER)";
			*(*player).money -= 700;
			getline(cin, *stemp);
			break;
		case 2:
			cout << "得到逃稅大師的親自指點，獲得一本逃稅密籍。(可以少繳一次稅)(按下ENTER)";
			(*(*player).notax)++;
			getline(cin, *stemp);
			break;
		case 3:
			cout << "矮人領導者送上500元，已表示友好，看來你交到了新朋友呢。" << endl;
			cout << "兩個種族相處友好，影響觀光類別的股票，[國殯飯店]與[雄濕旅遊]的股票大漲。(按下ENTER)";
			*(*player).money += 500;
			*stock[8].price += 500;
			*stock[9].price += 600;
			getline(cin, *stemp);
			break;
		case 4:
			cout << "到黑市變賣你偷來的精靈寶劍，賺了400元，良心好像有點不安...(按下ENTER)";
			*(*player).money += 400;
			getline(cin, *stemp);
			break;
		default:
			cout << "Error." << endl;
		}
		return;
	}

	//命運
	void community(Cplayer* player) {
		srand(time(NULL));
		*itemp = rand() % 5;
		switch (*itemp) {
		case 0:
			cout << "你進到一個神祕的洞穴探險，試圖找到寶藏。(按下ENTER)";
			getline(cin, *stemp);
			cout << "突然冒出了吸血鬼女王，她大聲譴責你無理的闖入行為，並露出獠牙，你嚇得雙腿發軟。(按下ENTER)";
			getline(cin, *stemp);
			cout << "你轉身逃跑，慌亂中口袋的金幣掉了出來，損失100元。(按下ENTER)";
			*(*player).money -= 100;
			getline(cin, *stemp);
			break;
		case 1:
			cout << "你養的龍掙脫鎖鏈，大肆破壞你的殖民地，損失慘重，花費1000元重建。(按下ENTER)";
			*(*player).money -= 1000;
			getline(cin, *stemp);
			break;
		case 2:
			cout << "我一見你骨骼精奇，就是練武的奇才，這本逃稅密籍拿去吧。(可以少繳兩次稅)(按下ENTER)";
			(*(*player).notax) += 2;
			getline(cin, *stemp);
			break;
		case 3:
			cout << "你在廢棄的礦坑裡找到一箱金幣，發財啦發財啦!!喔，等等，裡面只有50元...(按下ENTER)";
			*(*player).money += 50;
			getline(cin, *stemp);
			break;
		case 4:
			cout << "一隻玻璃獸躲在你的靴子裡，穿鞋時差點把他踩扁了，你抖了抖靴子，玻璃獸連同他身上的450元一起掉下來。(按下ENTER)";
			*(*player).money += 450;
			getline(cin, *stemp);
			break;
		default:
			cout << "Error." << endl;
		}
		return;
	}

	//***真實玩家***
	void realGamerLoop(Cplayer* player, CLands* map, CStart* start, Cplayer* p2, Cplayer* p3, Cplayer* p4, Cshow* show, Cstock* stock) {
		(*(*show).showstr).append("玩家" + *(*player).name + "的回合，");
		//擲骰子
		do {
			cout << "按d擲骰子，按r重新開始，show印出過往事件，load載入舊檔，save存檔，stock進入股票畫面: ";
			cin.getline(ptemp, 30);
			if (*ptemp == 'd' && *(ptemp + 1) == '\0') {
				srand(time(NULL));
				dice = new int;
				*dice = (rand() % 11) + 2;
				cout << "登登愣~你共擲出了 " << *dice << " 點。 " << endl;
				(*(*show).showstr).append("骰出了" + to_string(*dice) + "點。\n");
				break;
			}
			else if (*ptemp == 'r' && *(ptemp + 1) == '\0') {
				*(*start).restart = 1;
				return;
			}
			else if (*ptemp == 's' && *(ptemp + 1) == 'a' && *(ptemp + 2) == 'v' && *(ptemp + 3) == 'e' && *(ptemp + 4) == '\0') {
				(*(*show).showstr).append("選擇存檔，");
				if (*(*player).playerNumber == 1)
					saveText(player, p2, p3, p4, map, start);
				else if (*(*player).playerNumber == 2)
					saveText(p2, player, p3, p4, map, start);
				else if (*(*player).playerNumber == 3)
					saveText(p3, p2, player, p4, map, start);
				else if (*(*player).playerNumber == 4)
					saveText(p4, p2, p3, player, map, start);
			}
			else if (*ptemp == 'l' && *(ptemp + 1) == 'o' && *(ptemp + 2) == 'a' && *(ptemp + 3) == 'd' && *(ptemp + 4) == '\0') {
				(*(*show).showstr).append("選擇讀檔...\n");
				if (*(*player).playerNumber == 1)
					loadtext(player, p2, p3, p4, map, start);
				else if (*(*player).playerNumber == 2)
					loadtext(p2, player, p3, p4, map, start);
				else if (*(*player).playerNumber == 3)
					loadtext(p3, p2, player, p4, map, start);
				else if (*(*player).playerNumber == 4)
					loadtext(p4, p2, p3, player, map, start);

				//防止讀檔時讀取到破產真實玩家
				if (*(*player).money <= 0) {
					cout << "此玩家已破產，換下一個玩家。" << endl;
					return;
				}
			}
			else if (*ptemp == 's' && *(ptemp + 1) == 'h' && *(ptemp + 2) == 'o' && *(ptemp + 3) == 'w' && *(ptemp + 4) == '\0') {
				(*show).printShow();
			}
			else if (*ptemp == 's' && *(ptemp + 1) == 't' && *(ptemp + 2) == 'o' && *(ptemp + 3) == 'c' && *(ptemp + 4) == 'k' && *(ptemp + 5) == '\0') {
				(*stock).printStock(stock);
				(*stock).buysale(player, stock, show);
			}
			else {
				cout << "輸入錯誤。" << endl;
			}
		} while (true);

		*(*player).position += *dice;		//移動
		if (*(*player).position >= 30) {	//如果經過原點，獲得300元
			*(*player).position -= 30;
			cout << "經過起點，很高興你能活著回來，送你300元獎勵。" << endl;
			(*(*show).showstr).append("經過起點，獲得300元獎勵。");
			*(*player).money += 300;
		}
		cout << "前往[ " << *((*map).landName+ *(*player).position) << " ]方塊。(按下ENTER)";
		(*(*show).showstr).append("前往方塊:");
		(*(*show).showstr).append(*((*map).landName + *(*player).position));
		(*(*show).showstr).append(" ");

		getline(cin, *stemp);
		//到達機會方塊
		if (*(*player).position == 6) {
			cout << "危機就是轉機! 親愛的玩家，抽取一張機會牌...(按下ENTER)";
			getline(cin, *stemp);
			(*(*show).showstr).append("抽取機會牌。\n");
			chance(player, stock);
		}
		//到達命運方塊
		else if (*(*player).position == 17) {
			cout << "你到達了命運女神的殿堂，她將賜予你一張命運牌...(按下ENTER)";
			getline(cin, *stemp);
			(*(*show).showstr).append("抽取命運牌。\n");
			community(player);
		}
		//到達抽稅方塊
		else if (*(*player).position == 10 || *(*player).position == 25) {
			cout << "真倒楣，是收稅時間!!! 請上繳200元給系統。(按下ENTER)";
			getline(cin, *stemp);
			if (*(*player).notax > 0) {
				cout << "使用逃稅密籍!!!我就是死不繳稅三十六式!(按下ENTER)" << endl;
				(*(*show).showstr).append("使用逃稅密籍。\n");
				(*(*player).notax)--;
				getline(cin, *stemp);
			}
			else {
				*(*player).money -= 200;
				(*(*show).showstr).append("繳稅200元。\n");
			}
		}
		//到達起點
		else if (*(*player).position == 0) {
			//不做任何事
		}
		//房屋方塊
		else {
			//到達可購買的方塊
			if (*((*map).owner + *(*player).position) == *(*player).playerNumber || *((*map).owner + *(*player).position) == 0) {
				//土地到達五級房子
				if (*((*map).houses + *(*player).position) >= 5) {
					cout << "此地點已是五級房子，到達土地上限，無法再蓋房子。收取租金，獲得" << (int)*((*map).cost + *(*player).position) * 1.5 << "元。(按下ENTER)";
					*(*player).money += (int)*((*map).cost + *(*player).position) * 1.5;
					getline(cin, *stemp);
					(*(*show).showstr).append("收取租金，獲得" + to_string((int)*((*map).cost + *(*player).position) * 1.5) + "元。\n");
				}
				//買房子
				else {
					if (*((*map).houses + *(*player).position) == 0) {
						if (*(*player).money <= *((*map).cost + *(*player).position))
							cout << "****警告!有破產危機***" << endl;
						cout << "玩家" << *(*player).name << "有現金" << *(*player).money << "元，請問要購買房子嗎? 此地房子要" << *((*map).cost + *(*player).position) << "元。(y/n)";
					}
					else {
						cout << "你在此地已經有" << *((*map).houses + *(*player).position) << "級房子。收取租金，獲得" << (int)*((*map).cost + *(*player).position) / 2 * 1.5 << "元。\n";
						*(*player).money += (int)*((*map).cost + *(*player).position) / 2 * 1.5;
						(*(*show).showstr).append("收取租金，獲得" + to_string((int)*((*map).cost + *(*player).position) / 2 * 1.5) + "元。");
						if (*(*player).money <= *((*map).cost + *(*player).position))
							cout << "****警告!有破產危機***" << endl;
						cout << "玩家" << *(*player).name << "有現金" << *(*player).money << "元，請問要升級房子嗎?需要花費" << *((*map).cost + *(*player).position) << "元。(y/n): ";
					}
					do {
						cin.getline(ptemp, 25);
						if (*ptemp == 'y' && *(ptemp + 1) == '\0') {			//接受買房子
							*(*player).money -= *((*map).cost + *(*player).position);
							*((*map).houses + *(*player).position) += 1;
							if (*((*map).houses + *(*player).position) < 5)
								*((*map).cost + *(*player).position) *= 2;
							cout << "購買了[" << *((*map).landName + *(*player).position) << "]的房子。 此地點是" << *((*map).houses + *(*player).position) << "級房屋。(按下ENTER)";
							*((*map).owner + *(*player).position) = *(*player).playerNumber;
							(*(*show).showstr).append("購買房子，此地點為" + to_string(*((*map).houses + *(*player).position)) + "級房屋。\n");
							getline(cin, *stemp);
							break;
						}
						else if (*ptemp == 'n' && *(ptemp + 1) == '\0') {			//拒絕買房
							cout << "拒絕購買房子，遊戲繼續。(按下ENTER)";
							(*(*show).showstr).append("拒絕購買房子。\n");
							getline(cin, *stemp);
							break;
						}
						else
							cout << "輸入錯誤，請重新輸入(y/n):";
					} while (true);
				}
			}

			//踩上別人家的土地
			else if (*((*map).owner + *(*player).position) != *(*player).playerNumber && *((*map).owner + *(*player).position) > 0) {
				if (*((*start).dead + *((*map).owner + *(*player).position)-1) == 0) {		//此地擁有者還活著
					cout << "你站上了" << *((*map).owner + *(*player).position) << "號玩家的土地，需要付他" << *((*map).cost + *(*player).position) / 2 << "元過路費。(按下ENTER)";
					(*(*show).showstr).append("站上" + to_string(*((*map).owner + *(*player).position)) + "號玩家的土地，需要付他" + to_string(*((*map).cost + *(*player).position) / 2) + "元。\n");
					*(*player).money -= (int)*((*map).cost + *(*player).position) * 0.5;
					if( *(*p2).playerNumber == *((*map).owner + *(*player).position))		//誰的土地，誰就賺錢
						*(*p2).money += (int)*((*map).cost + *(*player).position) * 0.5;
					else if(*(*p3).playerNumber == *((*map).owner + *(*player).position))
						*(*p3).money += (int)*((*map).cost + *(*player).position) * 0.5;
					else if (*(*p4).playerNumber == *((*map).owner + *(*player).position))
						*(*p4).money += (int)*((*map).cost + *(*player).position) * 0.5;
					getline(cin, *stemp);
				}
				else {
					if (*(*player).money <= *((*map).cost + *(*player).position))
						cout << "****警告!有破產危機***" << endl;
					cout << "擁有這塊地的玩家已經死亡了，這邊是一片充滿怪物的荒地，" << (int)*((*map).cost + *(*player).position) / 2 << "元可以接管他的土地(y/n): ";
					do {
						cin.getline(ptemp, 25);
						if (*ptemp == 'y' && *(ptemp + 1) == '\0') {
							*(*player).money -= (int)*((*map).cost + *(*player).position) / 2;
							cout << "接管了[ " << *((*map).landName + *(*player).position) << " ]的房子。 此地點是" << *((*map).houses + *(*player).position) << "級房屋。(按下ENTER)";
							(*(*show).showstr).append("接管" + to_string(*((*map).owner + *(*player).position)) + "號玩家的房子。\n");
							*((*map).owner + *(*player).position) = *(*player).playerNumber;
							getline(cin, *stemp);
							break;
						}
						else if (*ptemp == 'n' && *(ptemp + 1) == '\0') {			//拒絕買房
							cout << "拒絕接管房子，遊戲繼續。(按下ENTER)";
							getline(cin, *stemp);
							break;
						}
						else
							cout << "輸入錯誤，請重新輸入(y/n):";
					} while (true);
				}
			}
		}
		//破產
		if (*(*player).money <= 0) {
			cout << "\n你破產了!!!!!!!!!(按下ENTER)" << endl;
			*((*start).dead + *(*player).playerNumber - 1) = 1;			//設定此真實玩家死亡
			(*(*show).showstr).append("此玩家破產!!!!\n");
			getline(cin, *stemp);
		}
		(*show).storeShow();
		*(*show).showstr = "";
	}

	//***虛擬電腦玩家***
	void computerGamerLoop(Cplayer* player, CLands* map, CStart* start, Cplayer* p2, Cplayer* p3, Cplayer* p4, Cshow* show, Cstock* stock) {
		(*(*show).showstr).append("玩家" + *(*player).name + "的回合，");
		//擲骰子
		srand(time(NULL));
		dice = new int;
		*dice = (rand() % 11) + 2;
		cout << "電腦玩家擲出了 " << *dice << " 點。 ";
		(*(*show).showstr).append("骰出了" + to_string(*dice) + "點。\n");

		*(*player).position += *dice;		//移動
		if (*(*player).position >= 30) {	//如果經過原點，獲得300元
			*(*player).position -= 30;
			cout << "經過起點，獲得300元獎勵。";
			(*(*show).showstr).append("經過起點，獲得300元獎勵。");
			*(*player).money += 300;
		}
		cout << "前往[ " << *((*map).landName + *(*player).position) << " ]方塊。\n";
		(*(*show).showstr).append("前往方塊:");
		(*(*show).showstr).append(*((*map).landName + *(*player).position));
		(*(*show).showstr).append(" ");
		//到達機會方塊
		if (*(*player).position == 6) {
			cout << "危機就是轉機! 抽取一張機會牌...\n";
			(*(*show).showstr).append("抽取機會牌。\n");
			chance(player, stock);
		}
		//到達命運方塊
		else if (*(*player).position == 17) {
			cout << "到達了命運女神的殿堂，她賜予一張命運牌...\n";
			(*(*show).showstr).append("抽取命運牌。\n");
			community(player);
		}
		//到達抽稅方塊
		else if (*(*player).position == 10 || *(*player).position == 25) {
			cout << "是收稅時間! 上繳200塊給系統。(按下ENTER)";
			getline(cin, *stemp);
			if (*(*player).notax > 0) {
				cout << "使用了逃稅密籍。";
				(*(*player).notax)--;
				(*(*show).showstr).append("使用逃稅密籍。\n");
				cout << "   (按下ENTER)";
				getline(cin, *stemp);
			}
			else {
				*(*player).money -= 200;
				(*(*show).showstr).append("繳稅200元。\n");
			}
		}
		//到達起點
		else if (*(*player).position == 0) {
			cout << "   (按下ENTER)";
			getline(cin, *stemp);
		}
		//房屋方塊
		else {
			//到達可購買的方塊
			if (*((*map).owner + *(*player).position) == *(*player).playerNumber || *((*map).owner + *(*player).position) == 0) {
				//土地到達五級房子
				if (*((*map).houses + *(*player).position) >= 5) {
					cout << "此地點已是五級房子，到達土地上限，無法再蓋房子。收取租金，獲得" << (int)*((*map).cost + *(*player).position) * 1.5 << "元。(按下ENTER)";
					*(*player).money += (int)*((*map).cost + *(*player).position) * 1.5;
					(*(*show).showstr).append("收取租金，獲得" + to_string((int)*((*map).cost + *(*player).position) * 1.5) + "元。\n");
					getline(cin, *stemp);
				}
				//買房子
				else {
					if (*((*map).houses + *(*player).position) == 0)
						cout << "玩家" << *(*player).name << "有現金" << *(*player).money << "元，此地房子要" << *((*map).cost + *(*player).position) << "元。";
					else {
						cout << "在此地已經有" << *((*map).houses + *(*player).position) << "級房子。收取租金，獲得" << (int)*((*map).cost + *(*player).position) / 2 * 1.5 << "元。\n";
						*(*player).money += (int)*((*map).cost + *(*player).position) / 2 * 1.5;
						(*(*show).showstr).append("收取租金，獲得" + to_string((int)*((*map).cost + *(*player).position) / 2 * 1.5) + "元。");
						cout << "玩家" << *(*player).name << "有現金" << *(*player).money << "元，升級房子需要花費" << *((*map).cost + *(*player).position) << "元。\n ";
					}
					if (*(*player).money > *((*map).cost + *(*player).position) + 150) {			//接受買房子
						*(*player).money -= *((*map).cost + *(*player).position);
						*((*map).houses + *(*player).position) += 1;
						if (*((*map).houses + *(*player).position) < 5)
							*((*map).cost + *(*player).position) *= 2;
						cout << "他購買了[" << *((*map).landName + *(*player).position) << "]的房子。 此地點是" << *((*map).houses + *(*player).position) << "級房屋。(按下ENTER)";
						*((*map).owner + *(*player).position) = *(*player).playerNumber;
						(*(*show).showstr).append("購買房子，此地點為" + to_string(*((*map).houses + *(*player).position)) + "級房屋。\n");
						getline(cin, *stemp);
					}
					else{			//拒絕買房
						cout << "他拒絕購買房子，遊戲繼續。(按下ENTER)";
						(*(*show).showstr).append("拒絕購買房子。\n");
						getline(cin, *stemp);
					}
				}
			}

			//踩上別人家的土地
			else if (*((*map).owner + *(*player).position) != *(*player).playerNumber && *((*map).owner + *(*player).position) > 0) {
				if (*((*start).dead + *((*map).owner + *(*player).position) - 1) == 0) {		//此地擁有者還活著
					cout << "站上了" << *((*map).owner + *(*player).position) << "號玩家的土地，付他" << (int)*((*map).cost + *(*player).position) * 0.5 << "元過路費。(按下ENTER)";
					(*(*show).showstr).append("站上" + to_string(*((*map).owner + *(*player).position)) + "號玩家的土地，需要付他" + to_string(*((*map).cost + *(*player).position) / 2) + "元。\n");
					*(*player).money -= (int)*((*map).cost + *(*player).position) * 0.5;
					if (*(*p2).playerNumber == *((*map).owner + *(*player).position))		//誰的土地，誰就賺錢
						*(*p2).money += (int)*((*map).cost + *(*player).position) * 0.5;
					else if (*(*p3).playerNumber == *((*map).owner + *(*player).position))
						*(*p3).money += (int)*((*map).cost + *(*player).position) * 0.5;
					else if (*(*p4).playerNumber == *((*map).owner + *(*player).position))
						*(*p4).money += (int)*((*map).cost + *(*player).position) * 0.5;
					getline(cin, *stemp);
				}
				else {
					cout << "擁有這塊地的玩家已經死亡了，" << (int)*((*map).cost + *(*player).position) / 2 << "元可以接管土地。\n";
					if (*(*player).money > (int)*((*map).cost + *(*player).position) / 2 + 130) {
						*(*player).money -= (int)*((*map).cost + *(*player).position) / 2;
						cout << "接管了[ " << *((*map).landName + *(*player).position) << " ]的房子。 此地點是" << *((*map).houses + *(*player).position) << "級房屋。(按下ENTER)";
						*((*map).owner + *(*player).position) = *(*player).playerNumber;
						(*(*show).showstr).append("接管" + to_string(*((*map).owner + *(*player).position)) + "號玩家的房子。\n");
						getline(cin, *stemp);
					}
					else {			//拒絕買房
						cout << "他拒絕接管房子，遊戲繼續。(按下ENTER)";
						getline(cin, *stemp);
					}
				}
			}
		}
		//破產
		if (*(*player).money <= 0) {
			cout << "\n此玩家破產了!!!" << endl;
			*((*start).dead + *(*player).playerNumber - 1) = 1;			//設定此玩家死亡
			(*(*show).showstr).append("此玩家破產!!!!\n");
		}
		(*show).storeShow();
		*(*show).showstr = "";
	}
};


int main()
{
	while (1) {
		//遊戲開始，印出規則和輸入人數
		CStart* start = new CStart();
		Cshow* show = new Cshow();
		Cstock* stock = new Cstock[10];	//10家股票陣列
		(*start).printStart();
		(*start).howManyPeople(show);
		Cplayer *playerOne = new Cplayer();
		Cplayer *playerTwo = new Cplayer();
		Cplayer *playerThree = new Cplayer();
		Cplayer *playerFour = new Cplayer();

		//設定股票。屬性:三家食品、兩家保險、三家建設、兩家觀光。
		*(*stock).company = "統二食品";
		*(*(stock + 1)).company = "胃全食品";
		*(*(stock + 2)).company = "白松食品";
		*(*(stock + 3)).company = "仁壽保險";
		*(*(stock + 4)).company = "太山保險";
		*(*(stock + 5)).company = "台尼水泥";
		*(*(stock + 6)).company = "富幫不動產";
		*(*(stock + 7)).company = "裕風建造";
		*(*(stock + 8)).company = "國殯飯店";
		*(*(stock + 9)).company = "雄濕旅遊";
		srand(time(NULL));
		*(*stock).price = (rand() % 250) + 100;
		*(*(stock + 1)).price = (rand() % 250) + 100;
		*(*(stock + 2)).price = (rand() % 250) + 100;
		*(*(stock + 3)).price = (rand() % 250) + 100;
		*(*(stock + 4)).price = (rand() % 250) + 100;
		*(*(stock + 5)).price = (rand() % 250) + 100;
		*(*(stock + 6)).price = (rand() % 250) + 100;
		*(*(stock + 7)).price = (rand() % 250) + 100;
		*(*(stock + 8)).price = (rand() % 250) + 100;
		*(*(stock + 9)).price = (rand() % 250) + 100;
		for (*(*stock).i = 0; *(*stock).i < 10; (*(*stock).i)++)
			*(*(stock + *(*stock).i)).oldprice = *(*(stock + *(*stock).i)).price;

		//輸入金錢，設定所有玩家的金錢數
		(*playerOne).inputMoney();
		cout << endl;

		//輸入玩家名字與設定金額
		cout << "輸入一號玩家英文名字: ";
		(*playerOne).inputName();
		*(*playerOne).playerNumber = 1;
		*(*start).dead = 0;
		(*(*show).showstr).append("一號玩家名字為" + *(*playerOne).name);

		(*playerTwo).setMoney(playerOne);		//設定金錢
		*(*playerTwo).playerNumber = 2;			//設定玩家號碼
		*((*start).dead + 1) = 0;
		if (*(*start).realPlayer >= 2) {		//如果有第二位真實玩家
			cout << "輸入二號玩家英文名字: ";
			(*playerTwo).inputName();
			(*(*show).showstr).append("\t二號玩家名字為" + *(*playerTwo).name);
		}
		else {									//第二位玩家為電腦玩家
			(*playerTwo).name = new string;
			*(*playerTwo).name = "ComputerTwo";
			(*(*show).showstr).append("\t二號玩家名字為ComputerTwo");
		}

		(*playerThree).setMoney(playerOne);		//設定金錢
		*(*playerThree).playerNumber = 3;			//設定玩家號碼
		if (*(*start).realPlayer >= 3) {		//如果有第三位真實玩家
			cout << "輸入三號玩家英文名字: ";
			(*playerThree).inputName();
			*((*start).dead + 2) = 0;
			(*(*show).showstr).append("\n三號玩家名字為" + *(*playerThree).name);
		}
		else {									//第三位玩家為電腦玩家
			(*playerThree).name = new string;
			*(*playerThree).name = "ComputerThree";
			if (*(*start).people >= 3) {
				*((*start).dead + 2) = 0;
				(*(*show).showstr).append("\n三號玩家名字為ComputerThree");
			}
		}

		(*playerFour).setMoney(playerOne);		//設定金錢
		*(*playerFour).playerNumber = 4;			//設定玩家號碼
		if (*(*start).realPlayer == 4) {		//如果有第四位真實玩家
			cout << "輸入四號玩家英文名字: ";
			(*playerFour).inputName();
			*((*start).dead + 3) = 0;
			(*(*show).showstr).append("\t四號玩家名字為" + *(*playerFour).name);
		}
		else {									//第四位玩家為電腦玩家
			(*playerFour).name = new string;
			*(*playerFour).name = "ComputerFour";
			if (*(*start).people == 4) {
				*((*start).dead + 3) = 0;
				(*(*show).showstr).append("\t四號玩家名字為ComputerFour");
			}
		}
		(*(*show).showstr).append("\n所有人的財產為");
		(*(*show).showstr).append(to_string(*(*playerOne).money) + "元。\n");
		(*show).storeShow();
		*(*show).showstr = "";

		cout << "\n大功告成，您設定完初始資料了! 所有人的開場金額為" << *(*playerOne).money << "元。";
		cout << "總共玩家數為" << *(*start).people << "人，選擇" << *(*start).realPlayer << "個真實玩家。(按下enter開始)";
		getline(cin, *(*start).strtemp);

		CLands *map = new CLands;
		(*map).setmap();			//設定地圖地名與價格

		CGameloop *game = new CGameloop;
		do {
			if (*(*start).dead == 0) {
				(*map).printmap(playerOne, playerTwo, playerThree, playerFour, start);					//印出地圖
				cout << "玩家一號 " << *(*playerOne).name << "擁有" << *(*playerOne).money << "元。\n";
				(*game).realGamerLoop(playerOne, map, start, playerTwo, playerThree, playerFour, show, stock);		//一號玩家
			}
			if (*(*start).restart == 1)		//重新開始遊戲
				break;

			if (*(*start).people >= 2 && *((*start).dead + 1) == 0) {
				(*map).printmap(playerOne, playerTwo, playerThree, playerFour, start);					//印出地圖
				cout << "玩家二號 " << *(*playerTwo).name << "擁有" << *(*playerTwo).money << "元。\n";
				if (*(*start).realPlayer >= 2)
					(*game).realGamerLoop(playerTwo, map, start, playerOne, playerThree, playerFour, show, stock);	//二號真實玩家
				else
					(*game).computerGamerLoop(playerTwo, map, start, playerOne, playerThree, playerFour, show, stock);//二號電腦玩家
			}
			if (*(*start).restart == 1)		//重新開始遊戲
				break;

			if (*(*start).people >= 3 && *((*start).dead + 2) == 0) {
				(*map).printmap(playerOne, playerTwo, playerThree, playerFour, start);					//印出地圖
				cout << "玩家三號 " << *(*playerThree).name << "擁有" << *(*playerThree).money << "元。\n";
				if (*(*start).realPlayer >= 3)
					(*game).realGamerLoop(playerThree, map, start, playerTwo, playerOne, playerFour, show, stock);	//三號真實玩家
				else
					(*game).computerGamerLoop(playerThree, map, start, playerTwo, playerOne, playerFour, show, stock);//三號電腦玩家
			}
			if (*(*start).restart == 1)		//重新開始遊戲
				break;

			if (*(*start).people == 4 && *((*start).dead + 3) == 0) {
				(*map).printmap(playerOne, playerTwo, playerThree, playerFour, start);					//印出地圖
				cout << "玩家四號 " << *(*playerFour).name << "擁有" << *(*playerFour).money << "元。\n";
				if (*(*start).realPlayer == 4)
					(*game).realGamerLoop(playerFour, map, start, playerTwo, playerThree, playerOne, show, stock);	//四號真實玩家
				else
					(*game).computerGamerLoop(playerFour, map, start, playerTwo, playerThree, playerOne, show, stock);//四號電腦玩家
			}
			if (*(*start).restart == 1)		//重新開始遊戲
				break;

			if (*(*start).dead == 1 && *(*start).realPlayer == 1 || *(*start).dead == 1 && *((*start).dead + 1) == 1 && *(*start).realPlayer == 2 ||
				*(*start).dead == 1 && *((*start).dead + 1) == 1 && *((*start).dead + 2) == 1 && *(*start).realPlayer == 3)
			{
				cout << "\n全部真人玩家都破產了!!!!!!!送你一把人生重來槍，要重新開始遊戲嗎?(按r重新開始)";
				do {
					cin.getline((*game).ptemp, 25);
					if (*(*game).ptemp == 'r' && *((*game).ptemp + 1) == '\0')
						break;
					else
						cout << "輸入錯誤，重新輸入: ";
				} while (true);
				break;
			}
			else if (*(*start).dead == 0 && *((*start).dead + 1) == 1 && *((*start).dead + 2) == 1 && *((*start).dead + 3) == 1){
				cout << "恭喜一號玩家" << *(*playerOne).name << "勝利!!!(按下ENTER)" << endl;
				getline(cin, *(*start).strtemp);
				break;
			}
			else if (*(*start).dead == 1 && *((*start).dead + 1) == 0 && *((*start).dead + 2) == 1 && *((*start).dead + 3) == 1) {
				cout << "恭喜二號玩家" << *(*playerTwo).name << "勝利!!!(按下ENTER)" << endl;
				getline(cin, *(*start).strtemp);
				break;
			}
			else if (*(*start).dead == 1 && *((*start).dead + 1) == 1 && *((*start).dead + 2) == 0 && *((*start).dead + 3) == 1) {
				cout << "恭喜三號玩家" << *(*playerThree).name << "勝利!!!(按下ENTER)" << endl;
				getline(cin, *(*start).strtemp);
				break;
			}
			else if (*(*start).dead == 1 && *((*start).dead + 1) == 1 && *((*start).dead + 2) == 1 && *((*start).dead + 3) == 0) {
				cout << "恭喜四號玩家" << *(*playerFour).name << "勝利!!!(按下ENTER)" << endl;
				getline(cin, *(*start).strtemp);
				break;
			}

			//股票價格變動
			(*stock).priceChange(stock);
			*stock[8].price += 500;
			*stock[9].price += 600;
		} while (true);

		cout << endl << endl;
		cout << "---------------------------------------------------------------------------------------------------------------------" << endl << endl;

		delete start;
		delete playerOne;
		delete playerTwo;
		delete playerThree;
		delete playerFour;
		delete map;
		delete game;
		delete[] stock;
	}

    return 0;
}

