#include <iostream>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <iterator>
#include <list>
#include <array>
#include <memory>
#include <stdexcept>
#include <mmsystem.h>
#include <conio.h>
#pragma comment(lib, Winmm.lib)
#pragma comment (lib,"Winmm.lib")
// ce hoces kompajlat ta program v linker dodaj sledece: -std=c++11 -lWinmm
using namespace std;

class UI{
private:
    short int _x = 0;
    short int _y = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //string _seznam[1024];
    list <string> _seznam;
    int sc = 0;
    int _index = 0;
    string mapa = "test";
    typedef void (UI::*funkcija_void_int)(int);
    typedef int (UI::*funkcija_int)();
	void dodaj_izhod(){
		_seznam.push_back ((string)"izhod");
	}
	int dir_a = 36;

public:
    string seznam_get(int x_pos){
        std::list<std::string>::iterator it = _seznam.begin();
        std::advance(it, x_pos);
        return *it;
    }

    void set_cp(){
        COORD pos = {_x, _y};
        SetConsoleCursorPosition(hConsole, pos);
        return;
    }
    void out(string out_p = "", int barva = 7){
        SetConsoleTextAttribute(hConsole, barva);
        WriteConsole(hConsole, out_p.c_str(), out_p.length(), NULL, NULL);
        return;
    }
    void x(int x_input){
        _x = x_input;
    }
    void y(int y_input){
        _y = y_input;
    }
    void xy(int x_input, int y_input){
        _x = x_input;
        _y = y_input;
    }
    void seznam_dodaj(string niz){
        _seznam.push_back (niz);
    }
    void izpisi_seznam(){
        out("Mitja Klajnsek\n\n");
        int barva = 10;
        for(int i = 0; i < _seznam.size(); i++){
            if(_index==i){
                out(seznam_get(i) + '\n', barva+13);
            }
            else{
                out(seznam_get(i) + '\n', barva);
            }
            barva++;
            if(barva>=15){
                barva=10;
            }
        }
        out();
        return;
    }
    void izberi(int smer = 0){
        system("cls");
        _index += smer;
        if(_index==-1){
            _index=0;
        }
        else if(_index >= _seznam.size()){
            _index-=smer;
        }
        izpisi_seznam();
        x(seznam_get(_index).size());
        y(_index+2);
        set_cp();
        Sleep(200);
        return;
    }
    int seznam(){
        bool dela = true;
        while(dela == true){
            if(GetAsyncKeyState(VK_UP)!=0){
                izberi(-1);
            }
            if(GetAsyncKeyState(VK_DOWN)!=0){
                izberi(1);
            }
            if(GetAsyncKeyState(VK_ESCAPE)!=0){
            	exit(0);
			}
			if(GetAsyncKeyState(VK_RETURN)!=0){
				if(_index==0){
					exit(0);
				}
				return _index;
			}
        }
        return -1;
    }
    void INIT(char *flags = "Win11"){
    	dodaj_izhod();
    	if(flags=="Win10"){
    		dir_a = 38;
		}
		return;
	}
	std::string cmd_komanda(const char* cmd) {
   		std::array<char, 128> buffer;
   		std::string result;
  		std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
 		if (!pipe) {
   		    throw std::runtime_error("popen() failed!");
  		}
  		while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
 		    result += buffer.data();
 		}
 		return result;
	}
	void izpisi_datoteke_v_seznam() {
		string ukaz = "dir " + mapa;
		string dir = cmd_komanda(ukaz.c_str());
		string datoteke[1024];
		string _datoteke[1024];
		int start = 0;
		int sc = 0;
		int element = 0;
		for(int i = 0; i < dir.length(); i++){
			switch(dir.at(i)){
				case '\n':{
					element++;
					break;
				}
				default:{
					_datoteke[element]+=dir.at(i);
					break;
				}
			}
		}
		for(int i = 7; i < element-2; i++){
			datoteke[i]=_datoteke[i].substr(dir_a, _datoteke[i].length()-1);
		}
		for(int i = 7; i < element-2; i++){
			_seznam.push_back (datoteke[i]);
		}
		return;
	}
	void spilaj(int _f){
		string pot = mapa+"\\"+seznam_get(_f);
		PlaySound(TEXT(pot.c_str()), NULL, SND_SYNC);
		return;
	}
	void loop_f_seznam(funkcija_void_int vd){
		UI UI;
		while(1){
			(UI.*vd)(seznam());
		}
		return;
	}
	void loop_f(funkcija_void_int vd, funkcija_int in_t){
		UI UI;
		while(1){
			int stev = (UI.*in_t)();
			(UI.*vd)(stev);
		}
		return;
	}
	void loop(){
		while(1){
 			spilaj(seznam());
 			if(GetAsyncKeyState(VK_SPACE)!=0){
 				PlaySound(0, 0, 0);
			 }
			 Sleep(500);
		}
	}
};
UI UI;

int main(int argc, char **argv)
{
	UI.INIT(argv[1]);
    UI.izpisi_datoteke_v_seznam();
    UI.izpisi_seznam();
    UI.loop();
    return 0;
}
