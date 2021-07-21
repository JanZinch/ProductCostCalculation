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

//#define DEBUG // для отключения необходимости авторизации

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

				cout << "Введите наименование изделия: " << endl;
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

			cout << "Введите кол-во изделий: " << endl;
			cin >> value;

			if (!cin) {

				throw exception("Некорректный ввод!");
			}

			if (value == 0) {

				throw 0;
			}
			else if (value < 0) {

				throw "Количество не может быть отрицательным.";
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

	cout << "Ввод данных для продукта "<< object.GetName() << "." << endl;

	int progress = 0;
	do {

		try {

			if (progress < 1) {

				cout << "Введите стоимость основных фондов(средств) предприятия (BYR): ";
				cin >> value;

				if (!cin) {

					throw exception("Некорректный ввод!");
				}

				if (value < 0) {

					throw "Стоимость всегда положительна!";
				}

				if (value == 0) {

					throw 0;
				}

				progress++;
			}

			cout << "Введите срок амортизации в месяцах: ";
			cin >> term;

			if (!cin) {

				throw exception("Некорректный ввод!");
			}

			if (term < 0) {

				throw "Срок амортизации всегда положителен!";
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

			throw runtime_error("Ошибка при открытии файла с данными пользователей!");	
		}
		else {

			while (file.read((char*)&buf, sizeof(User))) {

				buf.Decrypt();
				users_list.Add(buf);
			}

			cout << "Данные пользователей загружены." << endl;
		}

		file.close();

		file.open(database_path, fstream::in | fstream::binary);

		if (!file.is_open()) {

			throw runtime_error("Ошибка при открытии файла с данными изделий!");
		}
		else {

			while (file.read((char*)&product_buffer, sizeof(Product))) {

				database.push_back(product_buffer);
			}

			cout << "Данные изделий загружены." << endl;
		}

		file.close();

		file.open(info_path, fstream::in);

		if (!file.is_open()) {

			throw runtime_error("Ошибка при открытии файла с данными затрат!");
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
			cout << "Данные затрат загружены." << endl;
		}

		file.close();	
	
	}
	catch (const runtime_error &ex) {

		cout << ex.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	catch (const exception &ex) {

		cout << "Ошибка! Файл поврежден или заполнен неверно." << endl;
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

				cout << "Введите своё имя и фамилию: " << endl;

				if (cin.rdbuf()->in_avail() != 0) {

					cin.ignore(100, '\n');
				}

				SetConsoleCP(1251);
				getline(cin, buffer_one);
				SetConsoleCP(866);

				cout << "Введите пароль: " << endl;
				cin >> password;

				if (!cin) {

					throw exception("Некорректный ввод!");
				}

				value = users_list.Check(password, buffer_one, buffer_two);

				if (value == 0) {

					throw "Неверное имя пользоателя или пароль.";
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

				cout << "1. Создать запрос\t2. Работа с БД изделий\t3. Работа с БД затрат\t4. Управление пользователями\t8. Справка\t9. Сменить пароль\t0. Выход" << endl;
			}
			else {

				cout << "1. Создать запрос\t2. Работа с БД изделий\t3. Работа с БД затрат\t8. Справка\t9. Сменить пароль\t0. Выход" << endl;
			}			

			cin >> choise;

			if (cin.rdbuf()->in_avail() > 1) {
				
				throw exception("Некорректный ввод!");				
			}

			switch (choise) {

			case '1':

				step = false;
				do {

					try {

						cout << "Формирование запроса." << endl;
						cout << "1. Найти изделие\t2. Удалить изделие\t3. Просмотр запроса\t4. Сортировка запроса\t5. Расчитать стоимость\t6. Вывести отчёт\t0. Выход" << endl;
						cin >> choise;

						if (cin.rdbuf()->in_avail() > 1) {

							throw exception("Некорректный ввод!");
						}

						switch (choise) {

						case '1':

							cout << "Введите код изделия: ";
							cin >> value;

							if (!cin) {

								throw exception("Некорректный ввод!");
							}

							if (value < 0) {

								throw "Код изделия всегда положителен!";
							}

							search = false;
							for (auto &var : database) {

								if (var.GetCode() == value) {

									product_buffer = var;
									search = true;
								}
							}

							if (!search) {

								throw "Изделие не найдено.";
							}

							request.emplace_back(product_buffer);
							request.unique();
						
							cout << "Изделие добавлено в очередь." << endl;

							break;

						case '2':
							
							if (request.empty()) {

								throw "Запрос пуст";
							}

							cout << "Введите код изделия для удаления изделия по коду или:" << endl;
							cout << "1 - удаление первого изделия, 2 - удаление последнего изделия, 3 - удаление всех изделий" << endl;
							cin >> value;

							if (!cin) {

								throw exception("Некорректный ввод!");
							}

							if (value < 0) {

								throw "Код изделия всегда положителен!";
							}

							if (value == 0) {

								throw 0;
							}
							else if (value == 1) {

								cout << "Изделие " << request.front().GetName() << " удалено из запроса." << endl;
								request.remove(request.front());
							}							
							else if (value == 2) {

								cout << "Изделие " << request.back().GetName() << " удалено из запроса." << endl;
								request.remove(request.back());

							}
							else if (value == 3) {

								request.clear();
								cout << "Все изделия удалены." << endl;
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

									throw "Изделие не найдено.";
								}

								cout << "Изделие " << product_buffer.GetName() << " удалено из запроса." << endl;
								request.remove(product_buffer);							
							}				

							break;

						case '3':

							if (request.empty()) {

								cout << "Запрос пуст." << endl;
							}

							cout << "Запрос пользователя: " << endl;

							for (auto &var : request) {

								var.Print();
							}

							break;

						case '4':

							if (request.empty()) {

								throw "Запрос пуст";
							}

							cout << "Выберите вариант сортировки: 1 - от меньшего к большему, 2 - от большего к меньшему." << endl;
							cin >> value;

							if (!cin) {

								throw exception("Некорректный ввод!");
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

								throw "Некорректный ввод!";
							}
							break;

						case '5':

							if (request.empty()) {

								throw "Запрос пуст";
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
									cout << "Данные изделия " << var_i.GetName() << " не найдены." << endl;
								}
															
							}																											

							cout << "Расчёты произведены." << endl;
							break;

						case '6':

							if (request.empty()) {

								throw "Запрос пуст";
							}

							search = false;
							cout << setfill('-');
							cout << "\n" << setw(63) << "Отчёт о рассчитанной стоимости товаров" << setw(28) << "-\n" << endl;

							for (auto &var : request) {

								var.Print();

								if (var.GetCost() == emp || var.GetCost() == unk) {

									search = true;
								}
							}

							if (search) {

								cout << "Внимание! Не для всех изделий произведены рассчёты." << endl;
							}

							cout << endl;

							cout << setw(91) << "-" << "\n" << endl;
							cout << "Желаете ли вывести отчёт в файл? Нажмите 'Y' для подтверждения или 'N' для отказа." << endl;

							SetConsoleCP(1251);
							c = _getch();
							SetConsoleCP(866);

							if (c == 'Y' || c == 'Н' || c == 'y' || c == 'н') {

								file.open(current.GetWorkingFolder(), fstream::out | fstream::app);

								if (!file.is_open()) {

									throw "Ошибка при открытии файла!";
								}
								
								file << setfill('-');
								file << "\n" << setw(63) << "Отчёт о рассчитанной стоимости товаров" << setw(28) << "-\n" << endl;

								for (auto &var : request) {

									file << var << endl;									
								}

								if (search) {
								
									file << "Внимание! Не для всех изделий произведены рассчёты." << endl << endl;
								}

								file << setw(91) << "-" << "\n" << endl;					
								file.close();
								cout << "Отчёт выведен." << endl;
							}

							break;

						case '0':

							step = true;
							break;
						}
					}
					catch (int ex) {

						cout << "Ввод данных отменён." << endl;
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

						cout << "Управление базой изделий" << endl;
						cout << "1. Добавить изделие\t2. Удалить изделие\t3. Просмотреть список\t4. Сортировка по коду\t0. Выход" << endl;

						cin >> choise;

						if (cin.rdbuf()->in_avail() > 1) {

							throw exception("Некорректный ввод!");
						}

						switch (choise) {

						case '1':							

							file.open(database_path, fstream::out | fstream::app | fstream::binary);

							if (!file.is_open()) {

								throw "Ошибка при открытии файла!";
							}
							
							CreateProduct(value, buffer_one);
							product_buffer.Init(buffer_one.c_str(), value);
							database.push_back(product_buffer);
							file.write((char*)&product_buffer, sizeof(Product));	

							cout << "Изделие добавлено." << endl;

							file.close();

							break;

						case '2':

							cout << "Введите код удаляемого изделия: ";
							cin >> value;

							if (!cin) {

								throw exception("Некорректный ввод!");
							}

							search = false;
							for (auto &var : database) {

								if (var.GetCode() == value) {

									product_buffer = var;
									search = true;
								}
							}

							if (!search) {

								throw "Изделие не найдено.";
							}
														
							file.open(database_path, fstream::out | fstream::binary);

							if (!file.is_open()) {

								throw "Ошибка при открытии файла!";
							}
							
							database.remove(product_buffer);
								
							for (auto &var : database) {

								file.write((char*)&var, sizeof(Product));
							}
			
							cout << "Изделие удалено." << endl;
							file.close();																												
														
							break;

						case '3':

							cout << "Список изделий: " << endl;

							for (auto &var : database) {

								var.Print();
							}
							break;

						case '4':

							cout << "Выберите вариант сортировки: 1 - от меньшего к большему, 2 - от большего к меньшему." << endl;

							cin >> value;

							if (!cin) {

								throw exception("Некорректный ввод!");
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

								throw "Некорректный ввод!";
							}

							file.open(database_path, fstream::out | fstream::binary);

							if (!file.is_open()) {

								throw "Ошибка при открытии файла!";
							}

							for (auto &var : database) {

								file.write((char*)&var, sizeof(Product));
							}

							file.close();
						
							cout << "Сортировка произведена." << endl;
							break;

						case '0':

							step = true;
							break;

						default:

							throw "Некорректный ввод!";
						}

					}
					catch (int ex) {

						cout << "Ввод данных отменён." << endl;					
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

						cout << "Управление списком затрат" << endl;
						cout << "1. Добавить запись\t2. Обновить список\t3. Очистить список\t0. Выход" << endl;

						cin >> choise;

						if (cin.rdbuf()->in_avail() > 1) {

							throw exception("Некорректный ввод!");
						}

						switch (choise) {

						case '1':

							file.open(info_path, fstream::out | fstream::app);

							if (!file.is_open()) {

								throw "Ошибка при открытии файла!";
							}

							progress = 0;

							do {

								try {

									for (i = progress; i < size; i++) {

										if (i == 0) {

											cout << "Введите код изделия: " << endl;
										}
										else if (i == 1) {

											cout << "Введите стоимость сырья: " << endl;
										}
										else if (i == 2) {

											cout << "Введите стоимость возвратных отходов: " << endl;
										}
										else if (i == 3) {

											cout << "Введите стоимость зарплат сотрудников: " << endl;
										}
										else if (i == 4) {

											cout << "Введите стоимость премий сотрудников: " << endl;
										}
										else if (i == 5) {

											cout << "Введите стоимость рекламирования: " << endl;
										}
										else if (i == 6) {

											cout << "Введите значение фидбека: " << endl;
										}
										else if (i == 7) {

											cout << "Введите стоимость топлива: " << endl;
										}
										else if (i == 8) {

											cout << "Введите стоимость погрузки/разгрузки: " << endl;
										}
										else if (i == 9) {

											cout << "Введите стоимость аренды/лизинга транспорта: " << endl;
										}
										else if (i == 10) {

											cout << "Введите стоимость ремонтных работ: " << endl;
										}
										else if (i == 11) {

											cout << "Введите стоимость зарплат администраторов: " << endl;
										}
										else {

											cout << "Введите стоимость непроизводственных покупок: " << endl;
										}

										cin >> data[i];

										if (!cin) {

											throw exception("Некорректный ввод!");
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

							cout << "Запись добавлена. Для вступления изменений в силу необходимо обновить список затрат." << endl;

							file.close();

							break;

						case '2':

							try {

								expenses.clear();

								file.open(info_path, fstream::in);

								if (!file.is_open()) {

									throw "Ошибка при открытии файла с данными затрат!";
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
									cout << "Список затрат обновлён." << endl;
								}

								file.close();

							}
							catch (const char * ex) {

								cout << ex << endl;
							}
							catch (const exception &ex) {

								cout << "Ошибка! Файл поврежден или заполнен неверно." << endl;

							}
							catch (...) {

								throw;
							}
							
							break;

						case '3':

							try {

								cout << "Все существующие в списке данные о затратах будут утеряны. Вы действительно собираетесь удалить их?" << endl;
								cout << "Нажмите 'Y' для подтверждения или 'N' для отмены." << endl;

								SetConsoleCP(1251);
								c = _getch();
								SetConsoleCP(866);

								if (c != 'Y' && c != 'Н' && c != 'y' && c != 'н') {

									throw "Удаление отменено.";
								}							

								file.open(info_path, fstream::out);

								if (!file.is_open()) {

									throw "Ошибка при открытии файла с данными затрат!";
								}

								file.close();

								cout << "Список затрат очищен." << endl;

							}
							catch (const char * ex) {

								cout << ex << endl;
							}

							break;

						case '0':

							step = true;
							break;

						default:

							throw "Некорректный ввод!";
						}

					}
					catch (int ex) {

						cout << "Ввод данных отменён." << endl;
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

					throw "Некорректный ввод!";
				}

				step = false;			
				do {

					try {

						cout << "Управление пользователями" << endl;
						cout << "1. Добавить запись\t2. Удалить запись\t3. Просмотр записей\t0. Выход " << endl;
						cin >> choise;

						if (cin.rdbuf()->in_avail() > 1) {

							throw exception("Некорректный ввод!");
						}

						switch (choise) {

						case '1':							

							file.open(users_path, fstream::out | fstream::app | fstream::binary);

							if (!file.is_open()) {

								throw "Ошибка при открытии файла!";
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
							cout << "Запись добавлена." << endl;
							file.close();
							break;

						case '2':
							
							cout << "Введите код удаляемого пользователя: " << endl;
							cin >> value;

							if (!cin) {

								throw exception("Некорректный ввод!");
							}

							if (value < 0) {

								throw "Код неверен!";
							}

							if (value == 0) {

								throw  "Данный пользователь не может быть удалён.";
							}

							users_list.Delete(value);
							users_list.Write(users_path);
							cout << "Внимание! Коды пользователей изменены." << endl;
							break;

						case '3':

							cout << "Список пользователей: " << endl;
							users_list.Print();
							break;

						case '0':

							step = true;
							break;

						default:

							throw "Некорректный ввод!";					
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
				
					cout << "Введите новый пароль: " << endl;
					cin >> value;

					if (!cin) {

						throw exception("Некорректный ввод!");
					}
					if (value == 0) {

						throw 0;
					}					
					if (value < 100000 || value > 1000000) {

						throw exception("Пароль должен содержать 6 цифр.");
					}

					password = value;

					cout << "Повторите ввод проля: " << endl;
					cin >> value;

					if (!cin) {

						throw exception("Некорректный ввод!");
					}
					if (value == 0) {

						throw 0;
					}
					if (password != value) {

						throw "Введённые пароли не совпадают.";
					}

					users_list.SetNewPassword(current.GetName(), current.GetPassword(), password);
					users_list.Write(users_path);			
					step = true;

				} while (!step);

				break;

			case '0':

				cout << "Нажмите 'Y' для подтверждения или 'N' для отмены." << endl;

				SetConsoleCP(1251);
				c = _getch();
				SetConsoleCP(866);  

				if (c == 'Y' || c == 'Н' || c == 'y' || c == 'н') {

					Product::MemoryClean();
					return 0;
				}
				else {

					break;
				}

			default:

				throw "Некорректный ввод!";
			}

		}
		catch (int ex) {

			cout << "Ввод данных отменён." << endl;
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

			cout << "Что-то пошло не так..." << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
	}	
	
}