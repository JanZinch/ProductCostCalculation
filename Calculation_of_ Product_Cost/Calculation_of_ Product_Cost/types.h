#pragma once
#pragma warning (disable : 4996)   //instead #define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <windows.h>
#include <fstream>
#include <string>
#include <ctime>

#define NO_ACCESS 0
#define EMPLOYEE 1
#define ADMIN 2
#define EMPTY -1
#define UNK -2

using namespace std;


class User {

private:

	static int count;
	int code;
	char name[30];
	int password;
	int key;
	int access;
	char working_folder[100];

public:

	User();
	User(const char *, int, int, const char *);
	User(const User&);
	void Init(const char *, int, int, const char *);
	void Init(const User&);
	bool Create();
	void Print();
	void Welcome(bool);
	void SetCode(int);
	int GetCode();
	void ReduceCount();
	int GetPassword();
	string GetName();
	int GetAccess();
	void SetPassword(int);
	string GetWorkingFolder();
	void Encrypt();
	void Decrypt();
	int KeyGenerator();
};

int User::count = -1;

User::User() {

	char tmp[] = "NoData";
	char tmp_f[] = "";
	strcpy(name, tmp);
	strcpy(working_folder, tmp_f);
	access = NO_ACCESS;
	key = 0;
	password = 0;
	code = -1;

}

User::User(const char * name, int password, int access, const char * folder) {

	strcpy(this->name, name);
	strcpy(this->working_folder, folder);
	this->password = password;
	this->access = access;
	code = ++count;
	key = KeyGenerator();
}

User::User(const User &other) {

	strcpy(this->name, other.name);
	strcpy(this->working_folder, other.working_folder);
	this->password = other.password;
	this->access = other.access;
	key = other.key;
	code = ++count;
}

bool User::Create() {

	ofstream check;
	int progress = 0;

	while (progress < 4) {

		try {

			if (progress < 1) {

				if (cin.rdbuf()->in_avail() != 0) {

					cin.ignore(100, '\n');

				}

				cout << "Введите имя и фамилию нового пользователя: ";

				SetConsoleCP(1251);
				cin.getline(name, 30);
				SetConsoleCP(866);

				if (name[0] == 'u' && name[1] == 'n' && name[2] == 'd' && name[3] == 'o') {

					throw 0;
				}

				for (int i = 0; i < strlen(name); i++) {

					if ((unsigned int)name[i] < 32 || ((unsigned int)name[i] > 32 && (unsigned int)name[i] < 65) || ((unsigned int)name[i] > 122 && (unsigned int)name[i] < 129)) {

						throw "Неверное заполнение поля.";
					}
				}

				progress++;
			}

			if (progress < 2) {

				cout << "Введите пароль (шесть цифр): ";
				cin >> password;

				if (!cin) {

					throw exception("Некорректный ввод!");

				}

				if (password == 0) {

					throw 0;
				}

				if (password < 100000 || password >= 1000000) {

					throw "Пароль должен содержать 6 цифр.";

				}

				progress++;
			}

			if (progress < 3) {

				cout << "Введите путь к рабочей папке пользователя (пример: D:\\Documents\\SomeFolders\\WorkingFolder): " << endl;


				if (cin.rdbuf()->in_avail() != 0) {

					cin.ignore(100, '\n');
				}			

				SetConsoleCP(1251);
				cin.getline(working_folder, 30);
				SetConsoleCP(866);

				if (working_folder[0] == 'u' && working_folder[1] == 'n' && working_folder[2] == 'd' && working_folder[3] == 'o') {

					throw 0;
				}

				strcat(working_folder, "\\Report.txt");
				
				//strcpy(working_folder, "Report.txt"); для корневого юзера

				check.open(working_folder);
				if (!check.is_open()) {

					throw "Данная дирректория не существует или поле заполнено не в соответствии с примером. Пример дирректории: D:\\Documents\\SomeFolders\\WorkingFolder";
				}
				check.close();
				progress++;
			}

			if (progress < 4) {

				cout << "Введите уровень доступа нового пользователя (1 - сотрудник, 2 - администратор): " << endl;
				cin >> access;

				if (!cin) {

					throw exception("Некорректный ввод!");
				}

				if (access == 0) {

					throw 0;
				}

				if (access < 0 || access > 2) {

					throw "Уровень доступа должен равняться 1 или 2.";
				}

				progress++;
			}		
		}
		catch (int ex) {

			cout << "Ввод данных отменён." << endl;
			return false;
		}
		catch (const char * ex) {

			cout << ex << endl;
		}
		catch (const exception &ex) {

			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
		}
		
	}

	key = KeyGenerator();

	return true;
}

void User::Init(const char * name, int password, int access, const char * folder) {

	strcpy(this->name, name);
	strcpy(this->working_folder, folder);
	this->password = password;
	this->access = access;
	
}

void User::Init(const User &other) {

	strcpy(this->name, other.name);
	strcpy(this->working_folder, other.working_folder);
	this->password = other.password;
	this->access = other.access;
}

void User::Print() {

	cout << "Код: " << code << "  Имя: " << name << "  Права доступа: " << access << endl;

}

void User::SetCode(int new_code) {

	code = new_code;
}

int User::GetCode() {

	return code;
}

void User::ReduceCount() {

	count--;

}

int User::GetPassword() {

	return password;
}

string User::GetName() {

	return (string)name;

}

int User::GetAccess() {

	return access;
}

void User::Welcome(bool mode) {

	cout << "Доброго времени суток, " << name << "." << endl;
	cout << "Ваш уровень дступа: ";
	if (access == ADMIN) {

		cout << "администратор." << endl;
	}
	else {

		cout << "сотрудник." << endl;
	}

	if (mode) {

		cout << "Ваша рабочая папка для хранения отчётов: " << working_folder << endl;
		cout << "Ваш текущий пароль: "<< password << endl;
		cout << "Используйте специальные команды для отмены ввода данных: undo - отменить ввод строки, 0 - отменить числовой ввод." << endl;
	}

}

void User::SetPassword(int new_pw) {

	password = new_pw;
}

string User::GetWorkingFolder()
{
	return (string)working_folder;
}

int User::KeyGenerator() {

	srand(time(NULL));
	return rand() % 250 + 150;

}

void User::Encrypt() {
	
	if (key <= 200) {

		password = password >> 7;
	}
	else if (key > 200 && key <= 250) {

		password = password << 4;
	}
	else if (key > 250 && key <= 300) {

		password = password >> 9;
	}
	else if (key > 300) {

		password = password << 5;
	}

}

void User::Decrypt() {

	if (key <= 200) {

		password = password << 7;
	}
	else if (key > 200 && key <= 250) {

		password = password >> 4;
	}
	else if (key > 250 && key <= 300) {

		password = password << 9;
	}
	else if (key > 300) {

		password = password >> 5;
	}
}



class Money {

private:

	float value;
	char currency[4];

public:

	Money();
	Money(float, const char*);
	Money(const Money&);
	friend ostream& operator<<(ostream&, const Money&);
	friend istream& operator>>(istream&, Money&);
	Money& operator=(const Money&);
	Money operator+(const Money&);
	Money operator-(const Money&);
	friend bool operator==(const Money&, const Money&);
	template<typename T>
	Money operator*(T&);
	float Get();

};

Money::Money() {

	value = EMPTY;
	char tmp[] = "N/D";
	strcpy(currency, tmp);

}

Money::Money(float value, const char *currency) {

	this->value = (float)value;
	strcpy(this->currency, currency);

}

Money::Money(const Money &object) {

	value = object.value;
	strcpy(currency, object.currency);

}

ostream& operator<<(ostream& out, const Money &object) {

	out << setiosflags(ios::fixed) << setprecision(2) << object.value << " " << object.currency;
	return out;
}

istream& operator>>(istream& in, Money &object) {

	in >> object.value >> object.currency;
	return in;
}

Money& Money::operator=(const Money &object)
{

	value = object.value;
	strcpy(currency, object.currency);

	return *this;
}

Money Money::operator+(const Money &object) {

	if (strcmp(this->currency, object.currency) == 0) {

		float sum = this->value + object.value;
		Money Result(sum, currency);
		return Result;
	}
	else {

		Money Err(0, "Err");
		return Err;
	}
}

Money Money::operator-(const Money &object) {

	if (strcmp(this->currency, object.currency) == 0) {

		float sum = this->value - object.value;
		Money Result(sum, currency);
		return Result;

	}
	else {

		Money Err(0, "Err");
		return Err;
	}

}

bool operator==(const Money &object_1, const Money &object_2) {

	if (strcmp(object_1.currency, object_2.currency) == 0) {

		if (object_1.value == object_2.value) {

			return true;
		}
		else {

			return false;
		}
	}
	else {

		return false;
	}

}

template<typename T>
Money Money::operator*(T &mult){

	Money result(value * (float)mult, this->currency);
	return result;
}

float Money::Get() {

	return value;
}


class Product {

private:

	int code;
	char name[30];
	Money cost;
	int count;
	static int cls_c;
	static int *codes;
	static int size;

public:

	Product();
	Product(const char *, Money, int);
	Product(const Product&);
	void SetCost(Money);
	void Init(const char*, int);
	void Print();
	friend ostream& operator<<(ostream&, Product&);
	int GenerateCode();
	Product& operator=(const Product&);
	bool operator==(const Product&);
	friend bool operator==(const Product&, const Product&);
	bool operator<(const Product&);
	bool operator>(const Product&);
	static int GetProdCount();
	static bool Add(int);
	static void MemoryClean();
	int GetCode();
	string GetName();
	int GetCount();
	Money GetCost();

};

int Product::cls_c = 0;
int::Product::size = 0;
int*::Product::codes = new int[size];

Product::Product() : cost(), count(0), code(EMPTY) {

	char tmp[] = "NoData";
	strcpy(name, tmp);

}

Product::Product(const char * name, Money cost, int count)
{
	strcpy(this->name, name);
	this->cost = cost;
	this->count = count;

}

Product::Product(const Product &other)
{
	strcpy(name, other.name);
	cost = other.cost;
	count = other.count;
	code = other.code;
}

void Product::SetCost(Money cost)
{
	this->cost = cost;
}

void Product::Init(const char * name, int count)
{
	strcpy(this->name, name);	
	this->count = count;

	int code_;	
	while (true) {

		code_ = GenerateCode();
	
		if (Product::Add(code_) == true) {

			code = code_;
			break;
		}
	}
	
}

void Product::Print() {

	if (cost.Get() == EMPTY) {

		cout << "Код: " << code << "  Наименование: " << name << "  Кол-во: " << count << endl;
	}
	else if(cost.Get() == UNK) {
	
		cout << "Код: " << code << "  Наименование: " << name << "  Кол-во: " << count << "  Текущая стоимость: нет данных." << endl;
	}
	else {

		cout << "Код: " << code << "  Наименование: " << name << "  Кол-во: " << count << "  Текущая стоимость. " << " Единичная: " << cost << "  Общая: " << cost * count << endl;
	}

}

int Product::GenerateCode() {

	srand(time(NULL));
	return rand() % 900 + 100;
}

Product& Product::operator=(const Product &other) {

	strcpy(name, other.name);
	cost = other.cost;
	count = other.count;
	code = other.code;
	return *this;
}

bool Product::operator==(const Product &other) {
	
	return this->code == other.code;
}

bool operator==(const Product& our, const Product& other) {

	return our.code == other.code;
}

bool Product::operator<(const Product &other) {

	return this->code < other.code;
}

bool Product::operator>(const Product &other) {

	return this->code > other.code;
}

int Product::GetProdCount()
{
	return cls_c;
}

bool Product::Add(int code) {

	int i;

	for (i = 0; i < size; i++) {
		
		if (codes[i] == code) {

			return false;
		}
	}

	int *tmp = new int[size + 1];

	for (i = 0; i < size; i++) {
		tmp[i] = codes[i];
	}

	tmp[size] = code;

	size++;
	delete[] codes;
	codes = tmp;

	return true;
}

void Product::MemoryClean() {

	size = 0;
	delete[] codes;
}

int Product::GetCode() {

	return code;
}

string Product::GetName()
{
	return (string)name;
}

int Product::GetCount() {

	return count;
}

Money Product::GetCost()
{
	return cost;
}

ostream & operator<<(ostream &out, Product &object)
{
	if (object.cost.Get() == EMPTY) {

		out << "Код: " << object.code << "  Наименование: " << object.name << "  Кол-во: " << object.count << endl;
	}
	else if (object.cost.Get() == UNK) {

		out << "Код: " << object.code << "  Наименование: " << object.name << "  Кол-во: " << object.count << "  Текущая стоимость: нет данных." << endl;
	}
	else {

		out << "Код: " << object.code << "  Наименование: " << object.name << "  Кол-во: " << object.count << "  Текущая стоимость. " << " Единичная: " << object.cost << "  Общая: " << object.cost * object.count << endl;
	}

	return out;
}


namespace own {

	template <typename T>
	class SimpleArray {

	private:

		int size;
		T *arr;

	public:

		SimpleArray();
		void Add(T);
		void Delete(int);
		void Print();
		void Write(string);
		int Check(int, string&, string&);
		void SetNewPassword(string, int, int);
		~SimpleArray();
	};

	template<typename T>
	SimpleArray<T>::SimpleArray() {

		size = 0;
		arr = new T[size];

	}

	template<typename T>
	void SimpleArray<T>::Add(T object)
	{
		T *tmp = new T[size + 1];

		for (int i = 0; i < size; i++) {
			tmp[i] = arr[i];
		}

		tmp[size] = object;

		size++;
		delete[] arr;
		arr = tmp;
	}

	template<typename T>
	void SimpleArray<T>::Delete(int index)
	{
		if (size == 0) {

			cout << "Список пуст." << endl;
		}
		else {

			if (index < 0 || index >= size) {

				cout << "Запись не найдена." << endl;

			}
			else {

				int i;

				for (i = index; i < size; i++) {
					arr[i] = arr[i + 1];
				}

				size--;

				T *tmp = new T[size];
				for (i = 0; i < size; i++) {
					tmp[i] = arr[i];
				}

				delete[] arr;
				arr = tmp;

				for (i = 1; i < size; i++) {

					arr[i].SetCode(i);
				}

				arr[i].ReduceCount();
				cout << "Удаление завершено." << endl;
			}
		}
	}

	template<typename T>
	void SimpleArray<T>::Print()
	{
		if (size == 0) {

			cout << "Список пуст." << endl;
		}
		else {

			for (int i = 0; i < size; i++) {

				arr[i].Print();
			}
		}
	}

	template<typename T>
	void SimpleArray<T>::Write(string path) {

		if (size == 0) {

			cout << "Список пуст." << endl;
			return;
		}

		fstream file;

		file.open(path, fstream::out | fstream::binary);

		if (!file.is_open()) {

			cout << "Ошибка при открытии файла!" << endl;
		}
		else {

			for (int i = 0; i < size; i++) {

				arr[i].Encrypt();
				file.write((char*)& arr[i], sizeof(T));
				arr[i].Decrypt();
			}
		}

		file.close();

		return;
	}

	template<typename T>
	int SimpleArray<T>::Check(int password, string &name, string &path)
	{
		if (size == 0) {

			cout << "Список пуст." << endl;
		}
		else {

			for (int i = 0; i < size; i++) {

				if (arr[i].GetName() == name) {

					if (arr[i].GetPassword() == password) {

						path = arr[i].GetWorkingFolder();
						return arr[i].GetAccess();

					}
				}
			}

			return 0;
		}
	}

	template<typename T>
	SimpleArray<T>::~SimpleArray() {

		delete[] arr;

	}

	template<typename T>
	void SimpleArray<T>::SetNewPassword(string name, int old_pw, int new_pw)
	{
		if (size == 0) {

			cout << "Список пуст." << endl;
		}
		else {

			for (int i = 0; i < size; i++) {

				if (arr[i].GetName() == name) {

					if (arr[i].GetPassword() == old_pw) {

						arr[i].SetPassword(new_pw);
						return;

					}
				}
			}

		}
	}

}




class Expenses {

public:

	virtual Money Calculate() = 0;	
};


class TransportExpenses : public Expenses {

private:

	Money fuel;
	Money loading;
	Money leasing;

public:

	TransportExpenses() : fuel(), loading(), leasing() {};

	TransportExpenses(float f, float ld, float ls) {

		fuel = Money(f, "BYR");
		loading = Money(ld, "BYR");
		leasing = Money(ls, "BYR");
	}

	Money Calculate() override {

		return fuel + loading + leasing;
	}

};

class SalaryExpenses : public Expenses {

private:

	Money salary;
	Money prizes;

public:

	SalaryExpenses() : salary(), prizes() {};

	SalaryExpenses(float sl, float pr) {
	
		salary = Money(sl, "BYR");
		prizes = Money(pr, "BYR");
	}

	Money Calculate() override {

		return salary + prizes;
	}

};

class CommercialExpenses : public Expenses {

private:

	Money advertising;
	Money feedback;

public:

	CommercialExpenses() : advertising(), feedback() {};

	CommercialExpenses(float adv, float fdb) {
		
		advertising = Money(adv, "BYR");
		feedback = Money(fdb, "BYR");
	}

	Money Calculate() override {

		return advertising - feedback;
	}

};

class AdministrativeExpenses : public Expenses {

private:

	Money repair;
	Money furniture;
	Money admin_salary;

public:

	AdministrativeExpenses() : repair(), furniture(), admin_salary() {};

	AdministrativeExpenses(float rp, float fr, float adm) {

		repair = Money(rp, "BYR");;
		furniture = Money(fr, "BYR");
		admin_salary = Money(adm, "BYR");
	}

	Money Calculate() override {

		return repair + furniture + admin_salary;
	}

};


class MaterialExpenses : public Expenses {

private:

	Money raw;
	Money residues;

public:

	MaterialExpenses() : raw(), residues() {};

	MaterialExpenses(float rw, float res) {
		
		raw = Money(rw, "BYR");
		residues = Money(res, "BYR");
	}

	Money Calculate() override {

		return raw - residues;
	}

};







