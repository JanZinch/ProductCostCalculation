#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <list>
#include <deque>
#include <vector>
#include <memory>
#include "types.h"

//#define DEBUG // ��� ���������� ������������� �����������

using namespace std;


struct ExpensesData {

private:

	int code;

public:	

	TransportExpenses trans_exp;
	SalaryExpenses sal_exp;
	CommercialExpenses com_exp;
	AdministrativeExpenses adm_exp;
	MaterialExpenses mat_exp;

	ExpensesData() : code(-1), trans_exp(), sal_exp(), com_exp(), adm_exp(), mat_exp() {};

	ExpensesData(int code_, TransportExpenses trans, SalaryExpenses sal, CommercialExpenses com, AdministrativeExpenses adm, MaterialExpenses mat) {

		code = code_;
		trans_exp = trans;
		sal_exp = sal;
		com_exp = com;
		adm_exp = adm;
		mat_exp = mat;
	}

	ExpensesData(const ExpensesData &other) {

		code = other.code;
		trans_exp = other.trans_exp;
		sal_exp = other.sal_exp;
		com_exp = other.com_exp;
		adm_exp = other.adm_exp;
		mat_exp = other.mat_exp;
	}

	void Copy(const ExpensesData &other) {

		code = other.code;
		trans_exp = other.trans_exp;
		sal_exp = other.sal_exp;
		com_exp = other.com_exp;
		adm_exp = other.adm_exp;
		mat_exp = other.mat_exp;
	}

	int GetCode() {

		return code;
	}

};

void CreateProduct(int &value, string &buffer) {

	int progress = 0;
	do {

		try {

			if (progress < 1) {

				cout << "������� ������������ �������: " << endl;
				if (cin.rdbuf()->in_avail() != 0) {

					cin.ignore(100, '\n');
				}

				SetConsoleCP(1251);
				getline(cin, buffer);
				SetConsoleCP(866);

				if (buffer == "undo") {

					throw 0;
				}
				progress++;
			}

			cout << "������� ���-�� �������: " << endl;
			cin >> value;

			if (!cin) {

				throw exception("������������ ����!");
			}

			if (value == 0) {

				throw 0;
			}
			else if (value < 0) {

				throw "���������� �� ����� ���� �������������.";
			}
			progress++;
		}
		catch (int ex) {

			throw;
		}
		catch (const char * ex) {

			cout << ex << endl;
		}
		catch (const exception &ex) {

			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
		}

	} while (progress < 2);
}

Money CalcExpens(Expenses*);

void CalculateCost(Product &object, ExpensesData &data) {

	Money result;

	float value;
	float term;

	cout << "���� ������ ��� �������� "<< object.GetName() << "." << endl;

	int progress = 0;
	do {

		try {

			if (progress < 1) {

				cout << "������� ��������� �������� ������(�������) ����������� (BYR): ";
				cin >> value;

				if (!cin) {

					throw exception("������������ ����!");
				}

				if (value < 0) {

					throw "��������� ������ ������������!";
				}

				if (value == 0) {

					throw 0;
				}

				progress++;
			}

			cout << "������� ���� ����������� � �������: ";
			cin >> term;

			if (!cin) {

				throw exception("������������ ����!");
			}

			if (term < 0) {

				throw "���� ����������� ������ �����������!";
			}

			if (term == 0) {

				throw 0;
			}
			
			progress++;
		}
		catch (int ex) {

			throw;
		}
		catch (const char * ex) {

			cout << ex << endl;
		}
		catch (const exception &ex) {

			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
		}

	} while (progress < 2);

	
	Money amortization((value/term)/(float)object.GetCount(), "BYR");
	result = CalcExpens(&data.trans_exp) + CalcExpens(&data.sal_exp) + CalcExpens(&data.com_exp) + CalcExpens(&data.adm_exp) + CalcExpens(&data.mat_exp) + amortization;
	object.SetCost(result);

}

Money CalcExpens(Expenses *pointer) {

	return pointer->Calculate();

}



int main() {

	setlocale(LC_ALL, "ru");
	SetConsoleCP(866);

	string users_path = "Users.txt";
	string database_path = "DataBase.txt";
	string info_path = "Info.txt";
	fstream file;

	User buf;
	User current;
	Product product_buffer;
	ExpensesData exp_data_buffer;
	own::SimpleArray<User> users_list;
	list<Product> database;	
	list<Product> request;
	vector<ExpensesData> expenses;

	bool autorization = false;
	char choise, c;
	bool step;
	int password;
	int progress;
	int value;
	string buffer_one;
	string buffer_two;
	bool search;

	int i = 0, size = 13;
	shared_ptr<float[]> data(new float[size]);
	Money unk(UNK, "N/D");
	Money emp;	
	
	try {
		
		file.open(users_path, fstream::in | fstream::binary);

		if (!file.is_open()) {

			throw runtime_error("������ ��� �������� ����� � ������� �������������!");	
		}
		else {

			while (file.read((char*)&buf, sizeof(User))) {

				buf.Decrypt();
				users_list.Add(buf);
			}

			cout << "������ ������������� ���������." << endl;
		}

		file.close();

		file.open(database_path, fstream::in | fstream::binary);

		if (!file.is_open()) {

			throw runtime_error("������ ��� �������� ����� � ������� �������!");
		}
		else {

			while (file.read((char*)&product_buffer, sizeof(Product))) {

				database.push_back(product_buffer);
			}

			cout << "������ ������� ���������." << endl;
		}

		file.close();

		file.open(info_path, fstream::in);

		if (!file.is_open()) {

			throw runtime_error("������ ��� �������� ����� � ������� ������!");
		}
		else {

			getline(file, buffer_one);

			while (!file.eof()) {

				for (i = 0; i < size; i++) {

					file >> data[i];
					if (i == 0 || i == 3 || i == 5 || i == 7 || i == 10 || i == 12) {

						file >> buffer_one;
					}
				}

				search = false;
				for (auto &var : expenses) {

					if (var.GetCode() == data[0]) {

						TransportExpenses T(data[1], data[2], data[3]);
						SalaryExpenses S(data[4], data[5]);
						CommercialExpenses C(data[6], data[7]);
						AdministrativeExpenses A(data[8], data[9], data[10]);
						MaterialExpenses M(data[11], data[12]);
						ExpensesData Object((int)data[0], T, S, C, A, M);
						var.Copy(Object);
						search = true;
						break;
					}
				}

				if (!search) {

					TransportExpenses T(data[1], data[2], data[3]);
					SalaryExpenses S(data[4], data[5]);
					CommercialExpenses C(data[6], data[7]);
					AdministrativeExpenses A(data[8], data[9], data[10]);
					MaterialExpenses M(data[11], data[12]);
					ExpensesData Object((int)data[0], T, S, C, A, M);
					expenses.push_back(Object);

				}
			}
			cout << "������ ������ ���������." << endl;
		}

		file.close();	
	
	}
	catch (const runtime_error &ex) {

		cout << ex.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	catch (const exception &ex) {

		cout << "������! ���� ��������� ��� �������� �������." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	catch (...) {

		exit(EXIT_FAILURE);
	}


	while (true) {

		try {

#ifndef DEBUG

			if (!autorization) {

				cout << "������� ��� ��� � �������: " << endl;

				if (cin.rdbuf()->in_avail() != 0) {

					cin.ignore(100, '\n');
				}

				SetConsoleCP(1251);
				getline(cin, buffer_one);
				SetConsoleCP(866);

				cout << "������� ������: " << endl;
				cin >> password;

				if (!cin) {

					throw exception("������������ ����!");
				}

				value = users_list.Check(password, buffer_one, buffer_two);

				if (value == 0) {

					throw "�������� ��� ����������� ��� ������.";
				}
				
				current.Init(buffer_one.c_str(), password, value, buffer_two.c_str());
				current.Welcome(false);
				autorization = true;												
			}

#endif
	
#ifdef DEBUG
	
			current.Init("Test", 111111, 2, "Report.txt");  

#endif 
			

			if (current.GetAccess() == 2) {

				cout << "1. ������� ������\t2. ������ � �� �������\t3. ������ � �� ������\t4. ���������� ��������������\t8. �������\t9. ������� ������\t0. �����" << endl;
			}
			else {

				cout << "1. ������� ������\t2. ������ � �� �������\t3. ������ � �� ������\t8. �������\t9. ������� ������\t0. �����" << endl;
			}			

			cin >> choise;

			if (cin.rdbuf()->in_avail() > 1) {
				
				throw exception("������������ ����!");				
			}

			switch (choise) {

			case '1':

				step = false;
				do {

					try {

						cout << "������������ �������." << endl;
						cout << "1. ����� �������\t2. ������� �������\t3. �������� �������\t4. ���������� �������\t5. ��������� ���������\t6. ������� �����\t0. �����" << endl;
						cin >> choise;

						if (cin.rdbuf()->in_avail() > 1) {

							throw exception("������������ ����!");
						}

						switch (choise) {

						case '1':

							cout << "������� ��� �������: ";
							cin >> value;

							if (!cin) {

								throw exception("������������ ����!");
							}

							if (value < 0) {

								throw "��� ������� ������ �����������!";
							}

							search = false;
							for (auto &var : database) {

								if (var.GetCode() == value) {

									product_buffer = var;
									search = true;
								}
							}

							if (!search) {

								throw "������� �� �������.";
							}

							request.emplace_back(product_buffer);
							request.unique();
						
							cout << "������� ��������� � �������." << endl;

							break;

						case '2':
							
							if (request.empty()) {

								throw "������ ����";
							}

							cout << "������� ��� ������� ��� �������� ������� �� ���� ���:" << endl;
							cout << "1 - �������� ������� �������, 2 - �������� ���������� �������, 3 - �������� ���� �������" << endl;
							cin >> value;

							if (!cin) {

								throw exception("������������ ����!");
							}

							if (value < 0) {

								throw "��� ������� ������ �����������!";
							}

							if (value == 0) {

								throw 0;
							}
							else if (value == 1) {

								cout << "������� " << request.front().GetName() << " ������� �� �������." << endl;
								request.remove(request.front());
							}							
							else if (value == 2) {

								cout << "������� " << request.back().GetName() << " ������� �� �������." << endl;
								request.remove(request.back());

							}
							else if (value == 3) {

								request.clear();
								cout << "��� ������� �������." << endl;
							}
							else {

								search = false;
								for (auto &var : request) {

									if (var.GetCode() == value) {

										product_buffer = var;
										search = true;
									}
								}

								if (!search) {

									throw "������� �� �������.";
								}

								cout << "������� " << product_buffer.GetName() << " ������� �� �������." << endl;
								request.remove(product_buffer);							
							}				

							break;

						case '3':

							if (request.empty()) {

								cout << "������ ����." << endl;
							}

							cout << "������ ������������: " << endl;

							for (auto &var : request) {

								var.Print();
							}

							break;

						case '4':

							if (request.empty()) {

								throw "������ ����";
							}

							cout << "�������� ������� ����������: 1 - �� �������� � ��������, 2 - �� �������� � ��������." << endl;
							cin >> value;

							if (!cin) {

								throw exception("������������ ����!");
							}

							if (value == 0) {

								throw 0;
							}
							else if (value == 1) {

								request.sort();
							}
							else if (value == 2) {

								request.sort();
								request.reverse();
							}
							else {

								throw "������������ ����!";
							}
							break;

						case '5':

							if (request.empty()) {

								throw "������ ����";
							}

							for (auto &var_i : request) {

								value = var_i.GetCode();

								search = false;
								for (auto &var_j : expenses) {

									if (var_j.GetCode() == value) {

										if (var_i.GetCost() == unk || var_i.GetCost() == emp) {

											CalculateCost(var_i, var_j);
										}										
										search = true;
									}
								}

								if (!search) {

									var_i.SetCost(unk);
									cout << "������ ������� " << var_i.GetName() << " �� �������." << endl;
								}
															
							}																											

							cout << "������� �����������." << endl;
							break;

						case '6':

							if (request.empty()) {

								throw "������ ����";
							}

							search = false;
							cout << setfill('-');
							cout << "\n" << setw(63) << "����� � ������������ ��������� �������" << setw(28) << "-\n" << endl;

							for (auto &var : request) {

								var.Print();

								if (var.GetCost() == emp || var.GetCost() == unk) {

									search = true;
								}
							}

							if (search) {

								cout << "��������! �� ��� ���� ������� ����������� ��������." << endl;
							}

							cout << endl;

							cout << setw(91) << "-" << "\n" << endl;
							cout << "������� �� ������� ����� � ����? ������� 'Y' ��� ������������� ��� 'N' ��� ������." << endl;

							SetConsoleCP(1251);
							c = _getch();
							SetConsoleCP(866);

							if (c == 'Y' || c == '�' || c == 'y' || c == '�') {

								file.open(current.GetWorkingFolder(), fstream::out | fstream::app);

								if (!file.is_open()) {

									throw "������ ��� �������� �����!";
								}
								
								file << setfill('-');
								file << "\n" << setw(63) << "����� � ������������ ��������� �������" << setw(28) << "-\n" << endl;

								for (auto &var : request) {

									file << var << endl;									
								}

								if (search) {
								
									file << "��������! �� ��� ���� ������� ����������� ��������." << endl << endl;
								}

								file << setw(91) << "-" << "\n" << endl;					
								file.close();
								cout << "����� �������." << endl;
							}

							break;

						case '0':

							step = true;
							break;
						}
					}
					catch (int ex) {

						cout << "���� ������ ������." << endl;
					}
					catch (const char * ex) {

						cout << ex << endl;
					}
					catch (const exception &ex) {

						cout << ex.what() << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}


				} while (!step);

				break;

			case '2':

				step = false;
				do {

					try {

						cout << "���������� ����� �������" << endl;
						cout << "1. �������� �������\t2. ������� �������\t3. ����������� ������\t4. ���������� �� ����\t0. �����" << endl;

						cin >> choise;

						if (cin.rdbuf()->in_avail() > 1) {

							throw exception("������������ ����!");
						}

						switch (choise) {

						case '1':							

							file.open(database_path, fstream::out | fstream::app | fstream::binary);

							if (!file.is_open()) {

								throw "������ ��� �������� �����!";
							}
							
							CreateProduct(value, buffer_one);
							product_buffer.Init(buffer_one.c_str(), value);
							database.push_back(product_buffer);
							file.write((char*)&product_buffer, sizeof(Product));	

							cout << "������� ���������." << endl;

							file.close();

							break;

						case '2':

							cout << "������� ��� ���������� �������: ";
							cin >> value;

							if (!cin) {

								throw exception("������������ ����!");
							}

							search = false;
							for (auto &var : database) {

								if (var.GetCode() == value) {

									product_buffer = var;
									search = true;
								}
							}

							if (!search) {

								throw "������� �� �������.";
							}
														
							file.open(database_path, fstream::out | fstream::binary);

							if (!file.is_open()) {

								throw "������ ��� �������� �����!";
							}
							
							database.remove(product_buffer);
								
							for (auto &var : database) {

								file.write((char*)&var, sizeof(Product));
							}
			
							cout << "������� �������." << endl;
							file.close();																												
														
							break;

						case '3':

							cout << "������ �������: " << endl;

							for (auto &var : database) {

								var.Print();
							}
							break;

						case '4':

							cout << "�������� ������� ����������: 1 - �� �������� � ��������, 2 - �� �������� � ��������." << endl;

							cin >> value;

							if (!cin) {

								throw exception("������������ ����!");
							}

							if (value == 0) {

								throw 0;
							}
							else if (value == 1) {

								database.sort();

							}
							else if (value == 2) {

								database.sort();
								database.reverse();
							}
							else {

								throw "������������ ����!";
							}

							file.open(database_path, fstream::out | fstream::binary);

							if (!file.is_open()) {

								throw "������ ��� �������� �����!";
							}

							for (auto &var : database) {

								file.write((char*)&var, sizeof(Product));
							}

							file.close();
						
							cout << "���������� �����������." << endl;
							break;

						case '0':

							step = true;
							break;

						default:

							throw "������������ ����!";
						}

					}
					catch (int ex) {

						cout << "���� ������ ������." << endl;					
					}
					catch (const char * ex) {

						cout << ex << endl;
					}
					catch (const exception &ex) {

						cout << ex.what() << endl;
						cin.clear();
						cin.ignore(100, '\n');						
					}

				} while (!step);

				break;		

			case '3':

				step = false;
				do {

					try {

						cout << "���������� ������� ������" << endl;
						cout << "1. �������� ������\t2. �������� ������\t3. �������� ������\t0. �����" << endl;

						cin >> choise;

						if (cin.rdbuf()->in_avail() > 1) {

							throw exception("������������ ����!");
						}

						switch (choise) {

						case '1':

							file.open(info_path, fstream::out | fstream::app);

							if (!file.is_open()) {

								throw "������ ��� �������� �����!";
							}

							progress = 0;

							do {

								try {

									for (i = progress; i < size; i++) {

										if (i == 0) {

											cout << "������� ��� �������: " << endl;
										}
										else if (i == 1) {

											cout << "������� ��������� �����: " << endl;
										}
										else if (i == 2) {

											cout << "������� ��������� ���������� �������: " << endl;
										}
										else if (i == 3) {

											cout << "������� ��������� ������� �����������: " << endl;
										}
										else if (i == 4) {

											cout << "������� ��������� ������ �����������: " << endl;
										}
										else if (i == 5) {

											cout << "������� ��������� ��������������: " << endl;
										}
										else if (i == 6) {

											cout << "������� �������� �������: " << endl;
										}
										else if (i == 7) {

											cout << "������� ��������� �������: " << endl;
										}
										else if (i == 8) {

											cout << "������� ��������� ��������/���������: " << endl;
										}
										else if (i == 9) {

											cout << "������� ��������� ������/������� ����������: " << endl;
										}
										else if (i == 10) {

											cout << "������� ��������� ��������� �����: " << endl;
										}
										else if (i == 11) {

											cout << "������� ��������� ������� ���������������: " << endl;
										}
										else {

											cout << "������� ��������� ������������������ �������: " << endl;
										}

										cin >> data[i];

										if (!cin) {

											throw exception("������������ ����!");
										}

										progress++;

									}							
								}										
								catch (const exception &ex) {

									cout << ex.what() << endl;
									cin.clear();
									cin.ignore(100, '\n');
								}

							} while (progress != 13);					
							
							buffer_one = "---------";							

							file << endl;

							for (i = 0; i < size; i++) {
							
								file << data[i] << endl;
								if (i == 0 || i == 3 || i == 5 || i == 7 || i == 10) {

									file << buffer_one << endl;
								}
								if (i == 12) {

									file << "//----//----//";

								}
							}							

							cout << "������ ���������. ��� ���������� ��������� � ���� ���������� �������� ������ ������." << endl;

							file.close();

							break;

						case '2':

							try {

								expenses.clear();

								file.open(info_path, fstream::in);

								if (!file.is_open()) {

									throw "������ ��� �������� ����� � ������� ������!";
								}
								else {

									getline(file, buffer_one);

									while (!file.eof()) {

										for (i = 0; i < size; i++) {

											file >> data[i];
											if (i == 0 || i == 3 || i == 5 || i == 7 || i == 10 || i == 12) {

												file >> buffer_one;
											}
										}

										search = false;
										for (auto &var : expenses) {

											if (var.GetCode() == data[0]) {

												TransportExpenses T(data[1], data[2], data[3]);
												SalaryExpenses S(data[4], data[5]);
												CommercialExpenses C(data[6], data[7]);
												AdministrativeExpenses A(data[8], data[9], data[10]);
												MaterialExpenses M(data[11], data[12]);
												ExpensesData Object((int)data[0], T, S, C, A, M);
												var.Copy(Object);
												search = true;
												break;
											}
										}

										if (!search) {

											TransportExpenses T(data[1], data[2], data[3]);
											SalaryExpenses S(data[4], data[5]);
											CommercialExpenses C(data[6], data[7]);
											AdministrativeExpenses A(data[8], data[9], data[10]);
											MaterialExpenses M(data[11], data[12]);
											ExpensesData Object((int)data[0], T, S, C, A, M);
											expenses.push_back(Object);

										}
									}
									cout << "������ ������ �������." << endl;
								}

								file.close();

							}
							catch (const char * ex) {

								cout << ex << endl;
							}
							catch (const exception &ex) {

								cout << "������! ���� ��������� ��� �������� �������." << endl;

							}
							catch (...) {

								throw;
							}
							
							break;

						case '3':

							try {

								cout << "��� ������������ � ������ ������ � �������� ����� �������. �� ������������� ����������� ������� ��?" << endl;
								cout << "������� 'Y' ��� ������������� ��� 'N' ��� ������." << endl;

								SetConsoleCP(1251);
								c = _getch();
								SetConsoleCP(866);

								if (c != 'Y' && c != '�' && c != 'y' && c != '�') {

									throw "�������� ��������.";
								}							

								file.open(info_path, fstream::out);

								if (!file.is_open()) {

									throw "������ ��� �������� ����� � ������� ������!";
								}

								file.close();

								cout << "������ ������ ������." << endl;

							}
							catch (const char * ex) {

								cout << ex << endl;
							}

							break;

						case '0':

							step = true;
							break;

						default:

							throw "������������ ����!";
						}

					}
					catch (int ex) {

						cout << "���� ������ ������." << endl;
					}
					catch (const char * ex) {

						cout << ex << endl;
					}
					catch (const exception &ex) {

						cout << ex.what() << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}

				} while (!step);

				break;

			case '4':

				if (current.GetAccess() != 2) {

					throw "������������ ����!";
				}

				step = false;			
				do {

					try {

						cout << "���������� ��������������" << endl;
						cout << "1. �������� ������\t2. ������� ������\t3. �������� �������\t0. ����� " << endl;
						cin >> choise;

						if (cin.rdbuf()->in_avail() > 1) {

							throw exception("������������ ����!");
						}

						switch (choise) {

						case '1':							

							file.open(users_path, fstream::out | fstream::app | fstream::binary);

							if (!file.is_open()) {

								throw "������ ��� �������� �����!";
							}

							if (buf.Create() == true) {

								users_list.Add(buf);
							}
							else {

								throw 0;
							}

							buf.Encrypt();
							file.write((char*)& buf, sizeof(User));
							buf.Decrypt();
							cout << "������ ���������." << endl;
							file.close();
							break;

						case '2':
							
							cout << "������� ��� ���������� ������������: " << endl;
							cin >> value;

							if (!cin) {

								throw exception("������������ ����!");
							}

							if (value < 0) {

								throw "��� �������!";
							}

							if (value == 0) {

								throw  "������ ������������ �� ����� ���� �����.";
							}

							users_list.Delete(value);
							users_list.Write(users_path);
							cout << "��������! ���� ������������� ��������." << endl;
							break;

						case '3':

							cout << "������ �������������: " << endl;
							users_list.Print();
							break;

						case '0':

							step = true;
							break;

						default:

							throw "������������ ����!";					
						}
					}
					catch (int ex) {

						break;
					}
					catch (const char * ex) {

						cout << ex << endl;
					}
					catch (const exception &ex) {

						cout << ex.what() << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}

				} while (!step);

				break;


			case '8':

				current.Welcome(true);
				break;

			case '9':
			
				step = false;
				do {
				
					cout << "������� ����� ������: " << endl;
					cin >> value;

					if (!cin) {

						throw exception("������������ ����!");
					}
					if (value == 0) {

						throw 0;
					}					
					if (value < 100000 || value > 1000000) {

						throw exception("������ ������ ��������� 6 ����.");
					}

					password = value;

					cout << "��������� ���� �����: " << endl;
					cin >> value;

					if (!cin) {

						throw exception("������������ ����!");
					}
					if (value == 0) {

						throw 0;
					}
					if (password != value) {

						throw "�������� ������ �� ���������.";
					}

					users_list.SetNewPassword(current.GetName(), current.GetPassword(), password);
					users_list.Write(users_path);			
					step = true;

				} while (!step);

				break;

			case '0':

				cout << "������� 'Y' ��� ������������� ��� 'N' ��� ������." << endl;

				SetConsoleCP(1251);
				c = _getch();
				SetConsoleCP(866);  

				if (c == 'Y' || c == '�' || c == 'y' || c == '�') {

					Product::MemoryClean();
					return 0;
				}
				else {

					break;
				}

			default:

				throw "������������ ����!";
			}

		}
		catch (int ex) {

			cout << "���� ������ ������." << endl;
		}
		catch (const char * ex) {

			cout << ex << endl;
		}
		catch (const exception &ex) {

			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
		}
		catch (...) {

			cout << "���-�� ����� �� ���..." << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
	}	
	
}