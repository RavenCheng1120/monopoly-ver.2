/*
4105056005	�G�a�x	�ĥ|���@�~	11/21
4105056005	Yun-Ting Cheng The Fourth Homework 11/21
*/

// wp_hw04_4105056005.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>
#include <sstream>
using namespace std;

//�}�l�C���A�M�w�H�ơB�u�ꪱ�a��
class CStart
{
public:
	int* people;
	int* realPlayer;
	string* strtemp;	//�ΨӰ���JENTER���b

	CStart() {
		strtemp = new string;
	}

	~CStart() {
		delete people;
		delete realPlayer;
		delete strtemp;
	}

	//�L�X��l�W�h
	void printStart() {
		cout << setw(70) << "-�w��Ө�_�ۥ@�ɤj�I��-" << endl << endl;
		cout << "�C������:\n    ���C���}�l���B��ĳ��1500���A�C���g�L�_�I�N��o300���C" << endl;
		cout << "    �C����ʥi����Y��l(d)�άO���s�}�l��ӹC��(r)�C�i�H�s�ɡBŪ�ɻP�R���Ҧ������ɡC" << endl;
		cout << "    ��W�g�a����ɡA�i�H�M�w�O�_�ʶR�Фl�A�άO�~��ɯũЫΡA�@���g�a�̦h�ɨ�5�ũФl�C�Y�g�a�w���Ӫ��a�\���Фl�A�h�i�����O�@�O�C"
			<< "�����L���a���g�a�W�A�h�|�Q�����L���O�C" << endl;
		cout << "    �����|�ΩR�B����A�t�αN�۰ʩ�P�A�ð���P�W�����O�C" << endl;
		cout << "    ����|����A���a�N���200���C�Y�֦��k�|�d�P�N�|�۰ʨϥΡC" << endl;
		cout << "   �s�ɳ��b�P�@���ɮ�Save.txt�AŪ�ɤ]�O�qSave.txtŪ���A�Y�˶ä�r���榡�|�y��Ū�ɿ��~�C�i��ܥ���s�ɬ����C" << endl << endl;

		cout << "�C���I��:\n    �b�Y�ө_�ۥ@�ɡA����j�a�W�R���Ǫ��P�ɤO���~���A�ӧA�O�@�ӨS�S�L�D���R�����ߪ��H���C\n";
		cout << "    �A���ͪ��ڷQ�A�N�O�b�o���g�a�W�سy�ۤv������A���A�ä��O�ߤ@�Q�n�Ϊv�o�@�ɪ��H�C\n";
		cout << "    �ΧA�⤤�������A�سy�s�j���ޥ��a�A�úɤO���A���ĤH�}���C�̲פf�U���٦��������H�A�N�OĹ�a...\n\n";
	}

	//��J�`�@�C���H�ƩM�u�H���a�ƶq�A������J���b
	void howManyPeople() {
		cout << "�п�J�`�@�C���H��(2��4�H): ";
		people = new int;
		do {
			if (!(cin >> *people)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�D��ƿ�J�A�Э��s��J2��4: ";
				continue;
			}
			if (*people <= 4 && *people >= 2)
				break;
			cout << "��J�d����~�A�Э��s��J2��4: ";
		} while (true);
		cin.ignore();

		cout << "���X��u�ꪱ�a�@�P�C���O?�п�J�Ʀr(1�N��ۤv���A2�N��G�����a�O�u�H�ާ@�A3�N��G�P�T�����a�O�u�H�ާ@�A�H������): ";
		realPlayer = new int;
		do {
			if (!(cin >> *realPlayer)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�D��ƿ�J�A�Э��s��J: ";
				continue;
			}
			if (*realPlayer <= 0 || *realPlayer > 4)
				cout << "��J�d����~�A�Э��s��J1��4: ";
			else if (*realPlayer > *people)
				cout << "��J���~�A�èS������h�쪱�a��~�п�J" << *people << "�H�U����: ";
			else
				break;
		} while (true);
		cin.ignore();
		cout << endl;
	}
};

//���a���O�A�]�t�W�r�B�����B��m�B�k�|�d�B�O�_���`
class Cplayer
{
public:
	string* name;
	int* money;
	int* position;
	int* notax;
	int* dead;

	Cplayer() {		//��l���k�s
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

	//��J�u�ꪱ�a�W�r
	void inputName() {
		name = new string;
		getline(cin, *name);
	}

	//��J�_�l���B�A�u�ݿ�J�@���A�Ҧ��H�����B���|�ۦP
	void inputMoney() {
		cout << "��J�C���_�l���B(��ĳ1500���A���n��J�t��): ";
		do {
			if (!(cin >> *money)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "�D��ƿ�J�A�Э��s��J���B: ";
				continue;
			}
			if (*money > 0)
				break;
			cout << "��J�d����~�A�Э��s��J���B: ";
		} while (true);
		cin.ignore();
	}

	void setMoney(Cplayer* p1) {
		*(this->money) = *(*p1).money;
	}
};

//�g�a���O�A�]�t�W�١B����B�֦��̡B�Фl�ƶq
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

	//�]�w�a�Ϧa�W�P����
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
		//***���ݾ�u***
		for (*pi = 0; *pi < 120; (*pi)++)
			cout << "-";

		//***�W���l***
		for (*pj = 0; *pj < 4; (*pj)++) {
			//�L�X�Фl
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
			//�L�X�a�W
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
			//�L�X�л�
			else if (*pj == 2) {
				*pfront = 1;
				cout << "|     �_�I    ";
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*pfront == 6) {
						cout << "|   ���|  ";
						(*pfront)++;
						continue;
					}
					cout << "|" << setw(7) << *(cost + (*pfront)) << "��";
					(*pfront)++;
				}
				cout << "|   $$$$$$$   | ";
				(*pfront)++;
			}
			//�L�X���a��m
			else if (*pj == 3) {
				*pfront = 0;
				if (*(*p1).position == (*pfront))
					cout << "|��";
				else
					cout << "|  ";
				if (*(*p2).position == (*pfront) && *(*p2).dead == 0)
					cout << "��";
				else
					cout << "  ";
				if (*(*p3).position == (*pfront) && *(*start).people > 2 && *(*p3).dead == 0)
					cout << "��";
				else
					cout << "  ";
				if (*(*p4).position == (*pfront) && *(*start).people > 3 && *(*p4).dead == 0)
					cout << "��     ";
				else
					cout << "       ";
				(*pfront)++;
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*(*p1).position == (*pfront))
						cout << "|��";
					else
						cout << "|  ";
					if (*(*p2).position == (*pfront) && *(*p2).dead == 0)
						cout << "��";
					else
						cout << "  ";
					if (*(*p3).position == (*pfront) && *(*start).people > 2 && *(*p3).dead == 0)
						cout << "��";
					else
						cout << "  ";
					if (*(*p4).position == (*pfront) && *(*start).people > 3 && *(*p4).dead == 0)
						cout << "�� ";
					else
						cout << "   ";
					(*pfront)++;
				}
				if (*(*p1).position == (*pfront))
					cout << "|��";
				else
					cout << "|  ";
				if (*(*p2).position == (*pfront) && *(*p2).dead == 0)
					cout << "��";
				else
					cout << "  ";
				if (*(*p3).position == (*pfront) && *(*start).people > 2 && *(*p3).dead == 0)
					cout << "��";
				else
					cout << "  ";
				if (*(*p4).position == (*pfront) && *(*start).people > 3 && *(*p4).dead == 0)
					cout << "��     | ";
				else
					cout << "       | ";
				(*pfront)++;
			}
		}

		//***��u***
		for (*pi = 0; *pi < 120; (*pi)++)
			cout << '-';

		//***������l***
		for (*pi = 0; *pi < 4; (*pi)++) {
			for (*pj = 0; *pj < 4; (*pj)++) {
				//�L�X�Ы�
				if (*pj == 0) {
					if (*(houses + (*pback)) > 0 && *pi == 0)
						cout << "|    " <<  *(owner+(*pback)) << "'s house|       ��:1th player        ��:2th player    ";
					else if (*pi == 0)
						cout << "|             |       ��:1th player        ��:2th player    ";
					else if (*(houses + (*pback)) > 0)
						cout << "|    " << *(owner+(*pback)) << "'s house|                                             ";
					else
						cout << "|             |                                             ";
					if (*(houses+(*pfront)) > 0 && *pi == 0)
						cout << "   ��:3th player       ��:4th player        |    " << *(owner+(*pfront)) << "'s house| ";
					else if (*pi == 0)
						cout << "   ��:3th player       ��:4th player        |             | ";
					else if (*(houses + (*pfront)) > 0)
						cout << "                                            |    " << *(owner+(*pfront)) << "'s house| ";
					else
						cout << "                                            |             | ";
				}
				//�L�X�a�W
				else if (*pj == 1) {
					cout << "|" << setw(13) << *(landName + (*pback)) << "|                                             ";
					cout << "                                            |" << setw(13) << *(landName + (*pfront)) << "| ";
				}
				//�L�X����
				else if (*pj == 2) {
					cout << "|" << setw(11) << *(cost + (*pback)) << "��|                                             ";
					cout << "                                            |" << setw(11) << *(cost + (*pfront)) << "��| ";
				}
				//�L�X���a��m
				else if (*pj == 3) {
					//�����l
					if (*(*p1).position == *pback)
						cout << "|��";
					else
						cout << "|  ";
					if (*(*p2).position == *pback && *(*p2).dead == 0)
						cout << "��";
					else
						cout << "  ";
					if (*(*p3).position == *pback && *(*start).people > 2 && *(*p3).dead == 0)
						cout << "��";
					else
						cout << "  ";
					if (*(*p4).position == *pback && *(*start).people > 3 && *(*p4).dead == 0)
						cout << "��     |                                             ";
					else
						cout << "       |                                             ";
					//�k���l
					if (*(*p1).position == *pfront)
						cout << "                                            |��";
					else
						cout << "                                            |  ";
					if (*(*p2).position == *pfront && *(*p2).dead == 0)
						cout << "��";
					else
						cout << "  ";
					if (*(*p3).position == *pfront && *(*start).people > 2 && *(*p3).dead == 0)
						cout << "��";
					else
						cout << "  ";
					if (*(*p4).position == *pfront && *(*start).people > 3 && *(*p4).dead == 0)
						cout << "��     | ";
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

		//***��u***
		for (*pi = 0; *pi < 120; (*pi)++)
			cout << '-';

		//***�U���l***
		for (*pj = 0; *pj < 4; (*pj)++) {
			//�L�X�Фl
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
			//�L�X�a�W
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
			//�L�X�л�
			else if (*pj == 2) {
				*pback = 24;
				cout << "|   $$$$$$$   ";
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*pi == 7) {
						cout << "|   �R�B  ";
						(*pback)--;
						continue;
					}
					cout << "|" << setw(7) << *(cost + (*pback)) << "��";
					(*pback)--;
				}
				cout << "|" << setw(11) << *(cost + (*pback)) << "��| ";
				(*pback)--;
			}
			//�L�X���a��m
			else if (*pj == 3) {
				*pback = 25;
				if (*(*p1).position == (*pback))
					cout << "|��";
				else
					cout << "|  ";
				if (*(*p2).position == (*pback) && *(*p2).dead == 0)
					cout << "��";
				else
					cout << "  ";
				if (*(*p3).position == (*pback) && *(*start).people > 2 && *(*p3).dead == 0)
					cout << "��";
				else
					cout << "  ";
				if (*(*p4).position == (*pback) && *(*start).people > 3 && *(*p4).dead == 0)
					cout << "��     ";
				else
					cout << "       ";
				(*pback)--;
				for (*pi = 0; *pi < 9; (*pi)++) {
					if (*(*p1).position == (*pback))
						cout << "|��";
					else
						cout << "|  ";
					if (*(*p2).position == (*pback) && *(*p2).dead == 0)
						cout << "��";
					else
						cout << "  ";
					if (*(*p3).position == (*pback) && *(*start).people > 2 && *(*p3).dead == 0)
						cout << "��";
					else
						cout << "  ";
					if (*(*p4).position == (*pback) && *(*start).people > 3 && *(*p4).dead == 0)
						cout << "�� ";
					else
						cout << "   ";
					(*pback)--;
				}
				if (*(*p1).position == (*pback))
					cout << "|��";
				else
					cout << "|  ";
				if (*(*p2).position == (*pback) && *(*p2).dead == 0)
					cout << "��";
				else
					cout << "  ";
				if (*(*p3).position == (*pback) && *(*start).people > 2 && *(*p3).dead == 0)
					cout << "��";
				else
					cout << "  ";
				if (*(*p4).position == (*pback) && *(*start).people > 3 && *(*p4).dead == 0)
					cout << "��     | ";
				else
					cout << "       | ";
				(*pback)--;
			}
		}

		//***��u***
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
			cout << "��d�Y��l�A��r���s�}�l�Ashow�L�X�Ҧ��L���ƥ�Aload���J�¹C���ɡAsave�s�ɡAdelete�R���Ҧ�������: ";
			cin.getline(ptemp, 30);
			if (*ptemp == 'd' && *(ptemp + 1) == '\0') {
				srand(time(NULL));
				dice = new int;
				*dice = (rand() % 11) + 2;
				cout << "�n�n�h~�A�@�Y�X�F " << *dice << " �I�C " << endl;
				break;
			}
			else {
				cout << "��J���~�C" << endl;
			}
		} while (true);
	}

};

int main()
{
	//�C���}�l�A�L�X�W�h�M��J�H��
	CStart *start = new CStart();
	(*start).printStart();
	(*start).howManyPeople();
	Cplayer *playerOne = new Cplayer();
	Cplayer *playerTwo = new Cplayer();
	Cplayer *playerThree = new Cplayer();
	Cplayer *playerFour = new Cplayer();

	//��J�����A�]�w�Ҧ����a��������
	(*playerOne).inputMoney();
	cout << endl;

	//��J���a�W�r�P�]�w���B
	cout << "��J�@�����a�^��W�r: ";
	(*playerOne).inputName();
	if (*(*start).people >= 2) {
		(*playerTwo).setMoney(playerOne);		//�]�w����
		if (*(*start).realPlayer >= 2) {		//�p�G���ĤG��u�ꪱ�a
			cout << "��J�G�����a�^��W�r: ";
			(*playerTwo).inputName();
		}
		else {									//�ĤG�쪱�a���q�����a
			(*playerTwo).name = new string;
			*(*playerTwo).name = "ComputerPlayerTwo";
		}
	}
	if (*(*start).people >= 3) {
		(*playerThree).setMoney(playerOne);		//�]�w����
		if (*(*start).realPlayer >= 3) {		//�p�G���ĤT��u�ꪱ�a
			cout << "��J�T�����a�^��W�r: ";
			(*playerThree).inputName();
		}
		else {									//�ĤT�쪱�a���q�����a
			(*playerThree).name = new string;
			*(*playerThree).name = "ComputerPlayerThree";
		}
	}
	if (*(*start).people == 4) {
		(*playerFour).setMoney(playerOne);		//�]�w����
		if (*(*start).realPlayer == 4) {		//�p�G���ĥ|��u�ꪱ�a
			cout << "��J�|�����a�^��W�r: ";
			(*playerFour).inputName();
		}
		else {									//�ĥ|�쪱�a���q�����a
			(*playerFour).name = new string;
			*(*playerFour).name = "ComputerPlayerFour";
		}
	}
	cout << "\n�j�\�i���A�z�]�w����l��ƤF! �Ҧ��H���}�����B��" << *(*playerOne).money << "���C";
	cout << "�`�@���a�Ƭ�" << *(*start).people << "�H�A���" << *(*start).realPlayer << "�ӯu�ꪱ�a�C(���Uenter�~��)";
	getline(cin, *(*start).strtemp);

	CLands *map = new CLands;
	(*map).setmap();			//�]�w�a�Ϧa�W�P����
	(*map).printmap(playerOne, playerTwo, playerThree, playerFour, start);		//�L�X�a��

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

