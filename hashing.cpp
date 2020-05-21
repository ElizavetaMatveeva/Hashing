/*  Реализация открытого и закрытого хеширования */
#include <iostream>
#include <fstream>
#include <string.h>
#define FILE "dictionary.txt"
using namespace std;

namespace open_table
{
	struct node // Узел
	{
		char name[10];
		node* next;
		node () { name[0]='\0'; next=0; }
		node (char* n, node* nxt): next(nxt) { strcpy(name, n); }
	};
	
	class dictionary // Словарь
	{
		public:
			dictionary() {} // Конструктор
			~dictionary() { MAKENULL(); } // Деструктор
			void INSERT(char* n); // Функция вставки
			void DELETE(char* n); // Функция удаления элемента
			int MEMBER(char* n) { return is_member(n); } // Функция для проверки принадлежности элемента словарю
			void MAKENULL(); // Функция для обнуления словаря
			void PRINT() const; // Вывод словаря
		private:
			static const int B=10;
			node table[B]; // Массив узлов
			int hash_f(char* n) const; // Хеш-функция
			int is_member(char* n); // Функция для проверки принадлежности элемента словарю
	};
}

int open_table::dictionary::hash_f(char* n) const // Хеш-функция
{
	int sum=0, i=0;
	while (n[i]!='\0') // Складываем коды символов, пока не дойдём до конца строки
		sum+=n[i++];
	sum%=B; // Делим на количество классов, остаток - искомое хеш-значение
	return sum;
}

int open_table::dictionary::is_member(char* n) // Функция для проверки принадлежности элемента словарю
{
	int pos=hash_f(n);
	node* temp=table+pos;
	if (strcmp(temp->name, n)==0) // Если строки совпадают, возвращаем 1
		return 1;
	node* x=temp->next; // 1ый элемент списка коллизий
	while (x!=0) // Проходим по списку коллизий в поиске элемента
	{
		if (strcmp(x->name, n)==0) // Если строки совпадают, возвращаем 1
			return 1;			
		x=x->next;
	}
	return 0; // Если значение так и не нашли, возвращаем 0
}

void open_table::dictionary::INSERT(char* n) // Функция вставки
{
	if (is_member(n)) return; // Если такое значение уже есть, выходим из функции
	int pos=hash_f(n); // Вычисляем хеш-значение для переданного имени
	if (table[pos].name[0]=='\0') // Если в вычисленной позиции ничего нет, то записываем переданное значение
	{
		strcpy(table[pos].name, n);
		return; 
	}
	// Если в позиции уже есть элемент, проверяем список коллизий
	node* temp=table+pos;
	node* x=table[pos].next; // 1-ый элемент списка коллизий
	while (x!=0) // Проходим список коллизий в поиске последнего элемента
	{
		x=x->next;
		temp=temp->next;
	}
	node* nd=new node(n, 0); // Вставляем переданный узел
	temp->next=nd; // Предыдущий ссылается на только что вставленный
}

void open_table::dictionary::DELETE(char* n) // Функция удаления элемента
{
	if (!is_member(n)) return;
	int pos=hash_f(n); // Вычисляем, в какой позиции находится переданное имя
	node* el=table+pos;
	node* prev; // Элемент, предшествующий удаляемому
	while (strcmp(el->name, n)!=0) // Ищем удаляемый элемент и элемент, предшествующий ему
	{
		prev=el;
		el=el->next;
	}
	if (el->next==0) // Если элемент последний
	{
		if (el==table+pos) // И при этом находится в самой ячейке массива
		{
			table[pos].name[0]='\0'; // Просто обнуляем
			return;
		}
		prev->next=0; // Иначе присваиваем предшествующему эл-ту указатель на 0 и удаляем сам элемент
		delete el;
		return;
	}
	// Если удаляемый элемент имеет за собой другие элементы
	strcpy(el->name, el->next->name); // Скопировали имя
	node* temp=el->next;
	el->next=el->next->next; // Скопировали указатель на следующий
	delete temp; // Сам элемент удалили

}


void open_table::dictionary::PRINT() const // Вывод словаря
{
	cout<<"\ntable:"<<endl;
	for (int i=0; i<B; i++) // Идём по массиву
	{
		cout<<i<<") ";
		if (table[i].name!='\0') // Если элемент не пустой, выводим его 
			cout<<table[i].name;
		node* x=table[i].next; 
		while (x!=0) // Выводим список коллизий
		{
			cout<<"-"<<x->name;
			x=x->next;
		}
		cout<<endl;
	}
}

void open_table::dictionary::MAKENULL() // Функция для обнуления словаря
{
	for (int i=0; i<B; i++) // Проходим по всему массиву
	{
		if (table[i].name!='\0') // Обнуляем значение элемента массива, если оно не пусто
			table[i].name[0]='\0'; 
		if (node* temp=table[i].next) // Если за элементом есть список коллизий
		{
			while (temp!=0) // Удаляем элементы из списка коллизий
			{
				node* next=temp->next;
				delete temp;
				temp=next;
			}
		} 
		table[i].next=0;
	}
}

namespace closed_table
{
	struct node // Узел
	{
		char name[10];
		int deleted; // Логическая переменная для определения удалённых эл-тов
		node(): deleted(0) { name[0]='\0'; }
	};
	
	class dictionary // Словарь
	{
		public:
			dictionary() {} // Конструктор
			~dictionary() { MAKENULL(); } // Деструктор
			void INSERT(char* n); // Функция вставки
			void DELETE(char* n); // Функция удаления элемента
			int MEMBER(char* n) const { return is_member(n); } // Функция для проверки принадлежности элемента словарю
			void MAKENULL(); // Функция для обнуления словаря
			void PRINT() const; // Вывод словаря
		private:
			static const int B=10;
			node table[B]; // Массив узлов
			int hash_f(char* n) const; // Хеш-функция
			int double_hash_f(int index, int i) const; // Функция для повторного хеширования
			int is_member(char* n) const; // Функция для проверки принадлежности элемента словарю
			int search_insert_pos(char* n) const;
	};
}

int closed_table::dictionary::hash_f(char* n) const // Хеш-функция
{
	int sum=0, i=0;
	while (n[i]!='\0') // Складываем коды символов, пока не дойдём до конца строки
		sum+=n[i++];
	sum%=B; // Делим на количество классов, остаток - искомое хеш-значение
	return sum;
}

int closed_table::dictionary::double_hash_f(int index, int i) const // Функция для повторного хеширования
{
	int res=(index+i)%B;
	return res;
}

int closed_table::dictionary::is_member(char* n) const // Функция для проверки принадлежности элемента словарю
{
	int index=hash_f(n); // Вычисляем хеш-значение
	int pos=index;
	int i=1; // Номер хеширования
	while (strcmp(table[pos].name, n)!=0)
	{ 
		if (table[pos].name[0]=='\0' && table[pos].deleted==0) // Если дошли до пустого элемента
			return 0;
		pos=double_hash_f(index, i++);
		if (pos==index)
			return 0;
	}
	return 1;
}

int closed_table::dictionary::search_insert_pos(char* n) const
{
	int insert_pos=-1;
	int index=hash_f(n); // Вычисляем хеш-значение
	int pos=index;
	int i=1; // Номер хеширования
	do // Пока не прошли весь массив освобождённых ячеек
	{ 
		// Если дошли до пустого элемента и не нашли места для вставки, то возвращаем этот индекс
		if (table[pos].name[0]=='\0') 
			return pos; // Запомнили позицию первого пустого элемента
		pos=double_hash_f(index, i++);
	}
	while (pos!=index);
	return insert_pos;
}

void closed_table::dictionary::INSERT(char* n) // Функция вставки
{
	if (!is_member(n)) // Если такого элемента нет в словаре
	{
		int insert_pos=search_insert_pos(n); // Номер позиции для вставки
		if (insert_pos!=-1) // Если нашлась пустая ячейка для вставки, копируем туда переданное значение
		{
			if (table[insert_pos].deleted==1) // Если ячейка была помечена как удалённая, меняем ей этот параметр
				table[insert_pos].deleted=0;
			strcpy(table[insert_pos].name, n);
		}
	}
}

void closed_table::dictionary::MAKENULL() // Функция для обнуления словаря
{
	for (int i=0; i<B; i++)
	{
		table[i].name[0]='\0';
		table[i].deleted=0;
	}
}

void closed_table::dictionary::DELETE(char* n) // Функция удаления элемента
{
	for (int i=0; i<B; i++)
	{
		if (strcmp(table[i].name, n)==0) // Если нашли элемент с переданным значением
		{
			table[i].name[0]='\0'; // На место имени ставим пустую строку
			table[i].deleted=1; // Указываем, что данный элемент был удалён
		}
	}
}

void closed_table::dictionary::PRINT() const // Вывод словаря
{
	cout<<"\ntable:"<<endl;
	for (int i=0; i<B; i++)
	{
		if ((table[i].name[0])!='\0')
		{
			cout<<i<<") "<<table[i].name<<endl;
		}
		if (table[i].deleted)
			cout<<i<<") "<<"~deleted~"<<endl;
		if (table[i].name[0]=='\0' && table[i].deleted==0)
			cout<<i<<") "<<"-empty-"<<endl;
	}
}

using namespace closed_table;
//using namespace open_table;

void scan(dictionary& d) // Функция, считывающая данные из файла
{
	ifstream file;
	file.open(FILE);
	char name[10];
	while(!file.eof()) // Пока не дойдём до конца файла, считываем данные и записываем их в конец списка
	{
		file>>name;
		d.INSERT(name);	
	}
	file.close(); 
}

int main()
{
	dictionary d;
	scan(d);
	cout<<"Initial table";
	d.PRINT();
	char ba[]="ba";
	d.DELETE(ba);
	cout<<"\nDeleting 'ba'";
	d.PRINT();
	char bba[]="bba";
	d.INSERT(bba);
	cout<<"\nInserting 'bba'";
	d.PRINT();
	char aab[]="aab";
	d.INSERT(aab);
	cout<<"\nInserting 'aab'";
	d.PRINT();
	char baa[]="baa";
	d.INSERT(baa);
	cout<<"\nInserting 'baa'";
	d.PRINT();
	char b[]="b";
	cout<<"\nIs 'b' a member? "<<d.MEMBER(b)<<endl;
	d.INSERT(b);
	cout<<"\nInserting 'b'";
	d.PRINT();
}
