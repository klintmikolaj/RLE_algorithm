#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <windows.h>

using namespace std;

string RLE_compression_double_letter(const string &);
string RLE_compression_special_mark(const string &);
string input_file(const string &);
string type_of_encoding(const string &, const string &);
double compression_ratio(const string &, const string &);
void conditions(const string &, const string &);
void output_file(const string &, const double &);
void clear_file();
void menu();
void select_type();


//Uchwyt pod zmiane koloru tekstu
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
int white = 15;
int red = 12;
int green = 10;
int pink = 13;

int main()
{
    SetConsoleTitleA("RLE CONVERTER");
    SetConsoleTextAttribute(h, white);
    string choice_file = "";
    string choice_type = "";
    cout << "WELCOME TO RLE CONVERTER"<< endl << endl;
    while (true)
    {
        menu();
        cout << "=> ";
        getline(cin, choice_file);
        if (choice_file == "Q") break;
        if(choice_file != "clear_file")
        {
            select_type();
            cout << "=> ";
            getline(cin, choice_type);
            conditions(choice_file, choice_type);
        }
        else
        {
            SetConsoleTextAttribute(h, green);
            cout << "THE FILE HAS BEEN CLEARED" << endl;
            clear_file();
        }
    }
    return 0;
}

//Wyswietlanie tekstu w konsoli
void select_type()
{
    cout << "TYPE "; SetConsoleTextAttribute(h, red); cout << "1 "; SetConsoleTextAttribute(h, white);
    cout <<"TO ENCODE WITH A "; SetConsoleTextAttribute(h, red); cout << "DOUBLE SYMBOL "; SetConsoleTextAttribute(h, white);
    cout << "OR TYPE "; SetConsoleTextAttribute(h, red); cout << "2 "; SetConsoleTextAttribute(h, white); cout << "TO ENCODE BY A ";
    SetConsoleTextAttribute(h, red); cout << "SPECIAL MARK"; SetConsoleTextAttribute(h, white); cout << ":" << endl;
}

//Wczytywanie danych do pliku i zapisywanie ich w zmiennej typu string
string input_file(const string & choice)
{
    ifstream file;
    file.open (choice);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    file.close();
    return file_contents;
}

//Algorytm RLE z uzyciem podwojnego symbolu do oznaczenia sekwencji powtarzajacych sie znaków
string RLE_compression_double_letter(const string & message)
{
    int message_size = message.size();
    string letters;

    for (int i = 0; i < message_size; ++i)
    {
        int count = 1;
        while (message[i] == message[i+1])
        {
            count++;
            i++;
        }
        switch (count)
        {
            case 1:
                letters.push_back(message[i]);
                break;
            case 2:
                letters.push_back(message[i]); letters.push_back(message[i]); letters.push_back('0');
                break;
            default:
                letters.push_back(message[i]); letters.push_back(message[i]);
                letters += (to_string(count - 2));
        }
    }
    return letters;
}

//Algorytm RLE z wykorzystaniem specjalnego znaku do oznaczenia ciagu
string RLE_compression_special_mark(const string & message)
{
    int message_size = message.size();
    string letters;

    for (int i = 0; i < message_size; ++i)
    {
        int count = 1;
        while (message[i] == message[i+1])
        {
            count++;
            i++;
        }
        switch (count)
        {
            case 1:
                letters.push_back(message[i]);
                break;
            default:
                letters.push_back(message[i]); letters += ";"; letters += to_string(count); letters += ";";
        }
    }
    return letters;
}

//Zapisywanie danych do pliku
void output_file(const string & output, const double & comp_ratio)
{
    fstream file("output.txt", ios::in | ios::out | ios::app);
    if (file.is_open())
    {
        file << "[ENCODED MESSAGE:] " << output << endl;
        file << "[COMPRESSION RATIO:] " << comp_ratio << endl << endl;
    }
    // M.J.: Warto by poinformować użytkownika, gdy nie uda się otworzyć pliku.
}

//Obliczanie stopnia kompresji danych i zapisanie go w zmiennej typu double
double compression_ratio(const string & input, const string & output)
{
    string before = input;
    string after = output;
    int before_len =  int(before.length());
    int after_len = int(after.length());
    //Stopien kompresji liczony ze wzoru: dlugosc po skompresowaniu / dlugosc przed skompresowaniem
    //Zatem im mniejsza wartosc stopnia kompresji, tym wiecej miejsca zaoszczedziliœmy
    double comp_ratio = double(after_len) / double(before_len);
    return comp_ratio;
}

//Usuwanie wszystkich danych z pliku
void clear_file()
{
    ofstream file("output.txt");
    file << "";
    file.close();
}

//Wybieranie rodzaju kompresji danych
string type_of_encoding(const string & input, const string & type)
{
    if (type == "1")
    {
        return RLE_compression_double_letter(input);
    }
    else if (type == "2")
    {
        return RLE_compression_special_mark(input);
    }
    else
    {
        return "INCORRECT OPTION, TYPE 1 OR 2!";
    }
}

//Wyswietlanie wyników kompresji w konsoli
void conditions(const string & choice, const string & type)
{
    //Opcja wyswietlania, jezeli wpisany plik jest poprawny
    if (input_file(choice) != "")
    {
        cout << "\nYOUR FILE CONTENTS ARE:" << endl;
        SetConsoleTextAttribute( h, green); cout << input_file(choice) << endl;
        SetConsoleTextAttribute(h, white);
        cout << "\nYOUR MESSAGE AFTER ENCODING:" << endl;
        SetConsoleTextAttribute( h, green); cout << type_of_encoding(input_file(choice), type) << endl;
        SetConsoleTextAttribute(h, white);
        //Opcja wyswietlania, jezeli wpisana zostanie wartosc 1 (kodowanie podwojnym symbolem) lub 2 (kodowanie specjalnym znakiem)
        if (type_of_encoding(input_file(choice), type) != "INCORRECT OPTION, TYPE 1 OR 2!")
        {
            cout << "\nYOUR COMPRESSION RATIO:" << endl;
            SetConsoleTextAttribute( h, green); cout << compression_ratio(input_file(choice), type_of_encoding(input_file(choice), type)) << endl;
            SetConsoleTextAttribute(h, white);
            output_file(type_of_encoding(input_file(choice), type), compression_ratio(input_file(choice), type_of_encoding(input_file(choice), type)));
            cout << "\nYOUR ENCODED MESSAGE WAS SAVED IN: "; SetConsoleTextAttribute( h, green); cout<< "output.txt" << endl;
            SetConsoleTextAttribute(h, white);
        }
    }
        //Opcja wyswietlania, jezeli wpisany plik jest niepoprawny
    else
    {
        cout << "\nYOUR FILE CONTENTS ARE:" << endl;
        SetConsoleTextAttribute( h, red ); cout << "none" << endl;
        SetConsoleTextAttribute(h, white);
        cout << "\nYOUR MESSAGE AFTER ENCODING:" << endl;
        SetConsoleTextAttribute(h, white);
        SetConsoleTextAttribute( h, red);cout << "none" << endl;
        SetConsoleTextAttribute(h, white);
        cout << "\nYOUR COMPRESSION RATIO:" << endl; SetConsoleTextAttribute(h, white);
        SetConsoleTextAttribute( h, red);cout << "none" << endl;
        SetConsoleTextAttribute(h, white);
        cout << "\nYOUR ENCODED MESSAGE WASN'T SAVED" << endl;
    }
}

//Wyswietlanie menu w konsoli
void menu()
{
    SetConsoleTextAttribute(h, white);
    cout << "################################################################################################" << endl;
    SetConsoleTextAttribute( h, red);
    cout << "IF YOU WANT TO CLEAR THE FILE IN WHICH YOUR MESSAGE WILL BE SAVED TYPE COMMAND: clear_file"<< endl;
    cout << "IF YOU WANT TO QUIT THE PROGRAM PRESS: Q" << endl;
    SetConsoleTextAttribute(h, white);
    cout << "\nTYPE THE FILE NAME WHICH YOU WANT TO ENCODE (e.g.: input.txt):"<< endl;
}
