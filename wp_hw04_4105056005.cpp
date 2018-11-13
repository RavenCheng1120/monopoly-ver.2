/*
4105056005	鄭筠庭	第四次作業	11/21
4105056005	Yun-Ting Cheng The Fourth Homework 11/21
*/

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

//開始遊戲，決定人數、真實玩家數
class CStart
{
public:
	int* people;
	int* realPlayer;
	string* strtemp;	//用來做輸入ENTER防呆

	CStart() {
		strtemp = new string;
	}

	~CStart() {
		delete people;
		delete realPlayer;
		delete strtemp;
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
	void howManyPeople() {
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
	}
};

//玩家類別，包含名字、金錢、位置、逃稅卡、是否死亡
class Cplayer
{
public:
	string* name;
	int* money;
	int* position;
	int* notax;
	int* dead;

	Cplayer() {		//初始值歸零
		money = new int;
		*money = 0;
		position = new int;
		*position = 0;
		notax = new int;
		*notax = 0;
		dead = new int;
		*dead = 0;
	}

	~Cplayer() {
		delete name;
		delete money;
		delete position;
		delete notax;
		delete dead;
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
		*(landName + 9) = "Waterdeep";			*(landName + 10) = "-TAX-    ";		*(landName + 11) = "Silent Hill";
		*(landName + 12) = "Racoon City";		*(landName + 13) = "Los Santos";	*(landName + 14) = "District X";
		*(landName + 15) = "Snowdin";			*(landName + 16) = "Grillby's";		*(landName + 17) = "COMMUNITY";
		*(landName + 18) = "Skyrim";			*(landName + 19) = "Helheim";		*(landName + 20) = "Outland";
		*(landName + 21) = "Ravenholm";			*(landName + 22) = "Asgard";		*(landName + 23) = "Detroit";
		*(landName + 24) = "Alfheim";			*(landName + 25) = "-TAX-    ";		*(landName + 26) = "Highway 26";
		*(landName + 27) = "Gravity Falls";		*(landName + 28) = "Black Beach";	*(landName + 29) = "Sea of light";

		*(cost) = 0;			*(cost + 1) = 90;		*(cost + 2) = 60;
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
				if (*(*p1).position == (*pfront))
					cout << "|□";
				else
					cout << "|  ";
				if (*(*p2).position == (*pfront) && *(*p2).dead == 0)
					cout << "■";
				else
					cout << "  ";
				if (*(*p3).position == (*pfront) && *(*start).people > 2 && *(*p3).dead == 0)
					cout << "△";
				else
					cout << "  ";
				if (*(*p4).position == (*pfront) && *(*start).people > 3 && *(*p4).dead == 0)
					cout << "▲     ";
				else
					cout << "       ";
				(*pfront)++;
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*(*p1).position == (*pfront))
						cout << "|□";
					else
						cout << "|  ";
					if (*(*p2).position == (*pfront) && *(*p2).dead == 0)
						cout << "■";
					else
						cout << "  ";
					if (*(*p3).position == (*pfront) && *(*start).people > 2 && *(*p3).dead == 0)
						cout << "△";
					else
						cout << "  ";
					if (*(*p4).position == (*pfront) && *(*start).people > 3 && *(*p4).dead == 0)
						cout << "▲ ";
					else
						cout << "   ";
					(*pfront)++;
				}
				if (*(*p1).position == (*pfront))
					cout << "|□";
				else
					cout << "|  ";
				if (*(*p2).position == (*pfront) && *(*p2).dead == 0)
					cout << "■";
				else
					cout << "  ";
				if (*(*p3).position == (*pfront) && *(*start).people > 2 && *(*p3).dead == 0)
					cout << "△";
				else
					cout << "  ";
				if (*(*p4).position == (*pfront) && *(*start).people > 3 && *(*p4).dead == 0)
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
					if (*(*p1).position == *pback)
						cout << "|□";
					else
						cout << "|  ";
					if (*(*p2).position == *pback && *(*p2).dead == 0)
						cout << "■";
					else
						cout << "  ";
					if (*(*p3).position == *pback && *(*start).people > 2 && *(*p3).dead == 0)
						cout << "△";
					else
						cout << "  ";
					if (*(*p4).position == *pback && *(*start).people > 3 && *(*p4).dead == 0)
						cout << "▲     |                                             ";
					else
						cout << "       |                                             ";
					//右邊格子
					if (*(*p1).position == *pfront)
						cout << "                                            |□";
					else
						cout << "                                            |  ";
					if (*(*p2).position == *pfront && *(*p2).dead == 0)
						cout << "■";
					else
						cout << "  ";
					if (*(*p3).position == *pfront && *(*start).people > 2 && *(*p3).dead == 0)
						cout << "△";
					else
						cout << "  ";
					if (*(*p4).position == *pfront && *(*start).people > 3 && *(*p4).dead == 0)
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
				if (*(*p1).position == (*pback))
					cout << "|□";
				else
					cout << "|  ";
				if (*(*p2).position == (*pback) && *(*p2).dead == 0)
					cout << "■";
				else
					cout << "  ";
				if (*(*p3).position == (*pback) && *(*start).people > 2 && *(*p3).dead == 0)
					cout << "△";
				else
					cout << "  ";
				if (*(*p4).position == (*pback) && *(*start).people > 3 && *(*p4).dead == 0)
					cout << "▲     ";
				else
					cout << "       ";
				(*pback)--;
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*(*p1).position == (*pback))
						cout << "|□";
					else
						cout << "|  ";
					if (*(*p2).position == (*pback) && *(*p2).dead == 0)
						cout << "■";
					else
						cout << "  ";
					if (*(*p3).position == (*pback) && *(*start).people > 2 && *(*p3).dead == 0)
						cout << "△";
					else
						cout << "  ";
					if (*(*p4).position == (*pback) && *(*start).people > 3 && *(*p4).dead == 0)
						cout << "▲ ";
					else
						cout << "   ";
					(*pback)--;
				}
				if (*(*p1).position == (*pback))
					cout << "|□";
				else
					cout << "|  ";
				if (*(*p2).position == (*pback) && *(*p2).dead == 0)
					cout << "■";
				else
					cout << "  ";
				if (*(*p3).position == (*pback) && *(*start).people > 2 && *(*p3).dead == 0)
					cout << "△";
				else
					cout << "  ";
				if (*(*p4).position == (*pback) && *(*start).people > 3 && *(*p4).dead == 0)
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
	char* ptemp;
	
	CGameloop() {
		dice = new int;
		ptemp = new char[30];
	}

	~CGameloop(){
		delete dice;
		delete[] ptemp;
	}

	void realGamerLoop(Cplayer* player) {
		do {
			cout << "按d擲骰子，按r重新開始，show印出所有過往事件，load載入舊遊戲檔，save存檔，delete刪除所有紀錄檔: ";
			cin.getline(ptemp, 30);
			if (*ptemp == 'd' && *(ptemp + 1) == '\0') {
				srand(time(NULL));
				dice = new int;
				*dice = (rand() % 11) + 2;
				cout << "登登愣~你共擲出了 " << *dice << " 點。 " << endl;
				break;
			}
			else {
				cout << "輸入錯誤。" << endl;
			}
		} while (true);
	}

};

int main()
{
	//遊戲開始，印出規則和輸入人數
	CStart *start = new CStart();
	(*start).printStart();
	(*start).howManyPeople();
	Cplayer *playerOne = new Cplayer();
	Cplayer *playerTwo = new Cplayer();
	Cplayer *playerThree = new Cplayer();
	Cplayer *playerFour = new Cplayer();

	//輸入金錢，設定所有玩家的金錢數
	(*playerOne).inputMoney();
	cout << endl;

	//輸入玩家名字與設定金額
	cout << "輸入一號玩家英文名字: ";
	(*playerOne).inputName();
	if (*(*start).people >= 2) {
		(*playerTwo).setMoney(playerOne);		//設定金錢
		if (*(*start).realPlayer >= 2) {		//如果有第二位真實玩家
			cout << "輸入二號玩家英文名字: ";
			(*playerTwo).inputName();
		}
		else {									//第二位玩家為電腦玩家
			(*playerTwo).name = new string;
			*(*playerTwo).name = "ComputerPlayerTwo";
		}
	}
	if (*(*start).people >= 3) {
		(*playerThree).setMoney(playerOne);		//設定金錢
		if (*(*start).realPlayer >= 3) {		//如果有第三位真實玩家
			cout << "輸入三號玩家英文名字: ";
			(*playerThree).inputName();
		}
		else {									//第三位玩家為電腦玩家
			(*playerThree).name = new string;
			*(*playerThree).name = "ComputerPlayerThree";
		}
	}
	if (*(*start).people == 4) {
		(*playerFour).setMoney(playerOne);		//設定金錢
		if (*(*start).realPlayer == 4) {		//如果有第四位真實玩家
			cout << "輸入四號玩家英文名字: ";
			(*playerFour).inputName();
		}
		else {									//第四位玩家為電腦玩家
			(*playerFour).name = new string;
			*(*playerFour).name = "ComputerPlayerFour";
		}
	}
	cout << "\n大功告成，您設定完初始資料了! 所有人的開場金額為" << *(*playerOne).money << "元。";
	cout << "總共玩家數為" << *(*start).people << "人，選擇" << *(*start).realPlayer << "個真實玩家。(按下enter繼續)";
	getline(cin, *(*start).strtemp);

	CLands *map = new CLands;
	(*map).setmap();			//設定地圖地名與價格
	(*map).printmap(playerOne, playerTwo, playerThree, playerFour, start);		//印出地圖

	CGameloop *game = new CGameloop;
	(*game).realGamerLoop(playerOne);

	delete start;
	delete playerOne;
	delete playerTwo;
	delete playerThree;
	delete playerFour;
	delete map;
	delete game;
    return 0;
}

