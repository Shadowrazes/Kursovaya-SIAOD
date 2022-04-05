#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iterator>
#include <map>

using namespace std;

const int N = 4000, elemOnPage = 20;

struct Elem
{
	char FIO[32];
	char streetName[18];
	short int houseNumber;
	short int apartmentNumber;
	char settlementDate[10];
};

struct List
{
	List* next = NULL;
	List* prev = NULL;
	Elem* elem = NULL;
};

struct codeData
{
	char symbol;
	double frequency = 0;
	vector<bool> code = {};
};

struct Vertex
{
	int weight = 0;
	Elem* elem = NULL;
	Vertex* left = NULL;
	Vertex* right = NULL;
};

struct Queue
{
	List* head;
	List* tail;
};

int startPage(List*& p, List*& head)
{
	p = head;
	int page, count;
	while (1)
	{
		cout << "Enter page number " << 1 << "-" << N / elemOnPage << " : ";
		cin >> page;
		if (page > 0 && page <= N / elemOnPage)
		{
			count = (page - 1) * elemOnPage;
			for (int i = 0; i < count; i++)
			{
				p = p->next;
			}
			return count;
		}
	}
}

bool compareStrokes(Elem*& p, Elem*& q)
{
	string first = p->streetName, second = q->streetName;
	string first1 = p->settlementDate, second1 = q->settlementDate;
	if (stoi(first1.substr(6, 2)) < stoi(second1.substr(6, 2)))
		return true;
	else if (stoi(first1.substr(6, 2)) > stoi(second1.substr(6, 2)))
		return false;
	else if (stoi(first1.substr(3, 2)) < stoi(second1.substr(3, 2)))
		return true;
	else if (stoi(first1.substr(3, 2)) > stoi(second1.substr(3, 2)))
		return false;
	else if (stoi(first1.substr(0, 2)) < stoi(second1.substr(0, 2)))
		return true;
	else if (stoi(first1.substr(0, 2)) > stoi(second1.substr(0, 2)))
		return false;
	
	for (int i = 0; i < first.size(); i++)
	{
		if (first[i] < second[i])
		return true;
		else if (first[i] > second[i])
			return false;
	}
	return false;
}

void splitting(List*& head, List*& firstList, List*& secondList, int& listSize)
{
	firstList = head; secondList = head->next;
	List* p = firstList;
	List* q = secondList;
	listSize = 1;
	while (q != NULL)
	{
		listSize++;
		p->next = q->next;
		p = q;
		q = q->next;
	}
}

void mergeSeries(Queue& seriesList, List*& firstList, List*& secondList, int seriesSize1, int seriesSize2)
{
	while (seriesSize1 != 0 && seriesSize2 != 0)
	{
		if (compareStrokes(firstList->elem, secondList->elem))
		{
			if (seriesList.head != NULL)
			{
				seriesList.tail->next = firstList;
				firstList->prev = seriesList.tail;
				seriesList.tail = firstList;
			}
			else
			{
				seriesList.tail = seriesList.head = firstList;
				firstList->prev = NULL;
			}
			firstList = firstList->next;
			seriesList.tail->next = NULL;
			seriesSize1--;
		}
		else
		{
			if (seriesList.head != NULL)
			{
				seriesList.tail->next = secondList;
				secondList->prev = seriesList.tail;
				seriesList.tail = secondList;
			}
			else
			{
				seriesList.tail = seriesList.head = secondList;
				secondList->prev = NULL;
			}
			secondList = secondList->next;
			seriesList.tail->next = NULL;
			seriesSize2--;
		}
	}
	while (seriesSize1 > 0)
	{
		if (seriesList.head != NULL)
		{
			seriesList.tail->next = firstList;
			firstList->prev = seriesList.tail;
			seriesList.tail = firstList;
		}
		else
		{
			seriesList.tail = seriesList.head = firstList;
			firstList->prev = NULL;
		}
		firstList = firstList->next;
		seriesList.tail->next = NULL;
		seriesSize1--;
	}

	while (seriesSize2 > 0)
	{
		if (seriesList.head != NULL)
		{
			seriesList.tail->next = secondList;
			secondList->prev = seriesList.tail;
			seriesList.tail = secondList;
		}
		else
		{
			seriesList.tail = seriesList.head = secondList;
			secondList->prev = NULL;
		}
		secondList = secondList->next;
		seriesList.tail->next = NULL;
		seriesSize2--;
	}
}

void mergeSort(List*& head, List*& tail)
{
	List* firstList;
	List* secondList;
	Queue seriesList[2];
	int listSize = 0, seriesSize = 1;
	int currentSeriesSzFirst, currentSeriesSzSecond;
	splitting(head, firstList, secondList, listSize);
	
	while (seriesSize < listSize)
	{
		seriesList[0].head = seriesList[0].tail = NULL;
		seriesList[1].head = seriesList[1].tail = NULL;
		int currentQueue = 0;
		int currentlistSize = listSize;
		while (currentlistSize > 0)
		{
			if (currentlistSize >= seriesSize)
				currentSeriesSzFirst = seriesSize;
			else
				currentSeriesSzFirst = currentlistSize;
			currentlistSize -= currentSeriesSzFirst;
			if (currentlistSize >= seriesSize)
				currentSeriesSzSecond = seriesSize;
			else
				currentSeriesSzSecond = currentlistSize;
			currentlistSize -= currentSeriesSzSecond;

			mergeSeries(seriesList[currentQueue], firstList, secondList, currentSeriesSzFirst, currentSeriesSzSecond);
			currentQueue = 1 - currentQueue;
		}
		firstList = seriesList[0].head;
		secondList = seriesList[1].head;
		seriesSize *= 2;
	}
	seriesList[0].tail->next = NULL;
	tail = seriesList[0].tail;
	head = seriesList[0].head;
}

void SDPrec(Vertex* data, Vertex*& root)
{
	if (!root)
		root = data;
	else if (data->elem->apartmentNumber < root->elem->apartmentNumber)
		SDPrec(data, root->left);
	else if (data->elem->apartmentNumber > root->elem->apartmentNumber)
		SDPrec(data, root->right);
	else
		return;
}

void weightSort(vector<Vertex*>& elems, int left, int right)
{
	int i = left, j = right, med = (left + right) / 2;
	int x = elems[med]->weight;
	while (i <= j)
	{
		while (elems[i]->weight > x) i++;
		while (elems[j]->weight < x) j--;
		if (i <= j)
		{
			swap(elems[i], elems[j]);
			i++; j--;
		}
	}
	if (left < j)
		weightSort(elems, left, j);
	if(i < right)
		weightSort(elems, i, right);
}

void frequencySort(vector<codeData>& codes, int left, int right)
{
	int i = left, j = right, med = (left + right) / 2;
	double x = codes[med].frequency;
	while (i <= j)
	{
		while (codes[i].frequency > x) i++;
		while (codes[j].frequency < x) j--;
		if (i <= j)
		{
			swap(codes[i], codes[j]);
			i++; j--;
		}
	}
	if (left < j)
		frequencySort(codes, left, j);
	if (i < right)
		frequencySort(codes, i, right);
}

void A1(vector<Vertex*>& elems, Vertex*& root)
{
	weightSort(elems, 0, elems.size() - 1);
	for (const auto &it : elems)
	{
		SDPrec(it, root);
	}
}

void treeSearch(Vertex*& root, int& count, int& key, bool& isFound)
{
	if (root)
	{
		Vertex* p = root;
		while (p && p->elem->apartmentNumber != key)
		{
			if (p->elem->apartmentNumber > key)
				p = p->left;
			else if (p->elem->apartmentNumber < key)
				p = p->right;
		}
		if (p)
		{
			isFound = true;
			cout << count << ") " << p->elem->FIO << "\t" << p->elem->streetName << "\t" 
				<< p->elem->houseNumber << "\t\t" << p->elem->apartmentNumber << "\t\t" << p->elem->settlementDate << endl;
			count++;
		}
		else
			isFound = false;
	}
}

void roundTreeDown(Vertex* root, int& count)
{
	if (root)
	{
		cout << count << ") " << root->elem->FIO << "\t" << root->elem->streetName << "\t"
			<< root->elem->houseNumber << "\t\t" << root->elem->apartmentNumber << "\t\t" << root->elem->settlementDate << endl;
		count++;
		roundTreeDown(root->left, count);
		roundTreeDown(root->right, count);
	}
}

void deleteTree(Vertex*& root)
{
	if (root->left)
		deleteTree(root->left);
	if (root->right)
		deleteTree(root->right);
	free(root);
}

bool isNumber(const string& input)
{
	for (const auto &it : input)
	{
		if (it < 48 || it > 57)
			return false;
	}
	return true;
}

void searchAll(const vector<List*> &elements)
{
	system("cls");
	string year, apartmentNum;
	int yNum, atNum, count;
	Vertex* root = NULL;
	Queue searchList;
	List* p;
	while (1)
	{
		while (1)
		{
			cout << "Input year in format YY or 'b' to back: ";
			cin >> year;
			if (isNumber(year))
				yNum = stoi(year);
			else
			{
				if (year == "b" || year == "B")
				{
					if (root)
						deleteTree(root);
					return;
				}
				system("cls");
				cout << "Incorrect year!" << endl;
				continue;
			}
			if (yNum >= 0 && yNum <= 99)
			{
				system("cls");
				string temp;
				int L = 0, R = N - 1, mid;
				while (L < R)
				{
					mid = (L + R) / 2;
					temp = elements[mid]->elem->settlementDate;
					if (stoi(temp.substr(6, 2)) < yNum) L = mid + 1;
					else R = mid;
				}
				temp = elements[L]->elem->settlementDate;

				if (stoi(temp.substr(6, 2)) != yNum)
				{
					cout << "No one element is found" << endl;
					continue;
				}
				int first = L;
				L = 0, R = N - 1;
				while (L < R)
				{
					mid = round((float)(L + R) / 2);
					temp = elements[mid]->elem->settlementDate;
					if (stoi(temp.substr(6, 2)) > yNum) R = mid - 1;
					else L = mid;
				}

				system("cls");
				searchList = { elements[first], elements[R] };
				p = searchList.head;
				count = 1;
				if(root)
					deleteTree(root);
				vector<Vertex*> elems;
				map <short int, Vertex*> weights;

				cout << "\t\t" << "FIO" << "\t\t\t" << "Street name" << "\t   "
					<< "House number" << "\t  " << "Apartment number" << "  " << "Date of settlement" << endl;
				while (p != searchList.tail->next)
				{
					cout << count << ") " << p->elem->FIO << "\t" << p->elem->streetName
						<< "\t" << p->elem->houseNumber << "\t\t" << p->elem->apartmentNumber << "\t\t" << p->elem->settlementDate << endl;
					p = p->next;
					count++;

					if(weights[p->elem->apartmentNumber] == NULL)
						weights[p->elem->apartmentNumber] = new Vertex;
					weights[p->elem->apartmentNumber]->weight++;
					weights[p->elem->apartmentNumber]->elem = p->elem;
				}

				for (const auto& it : weights)
					elems.push_back(it.second);
				weights.clear();
				cout << "------------------------------------------------------------------------------------------------------------------------" << endl;

				count = 1;
				A1(elems, root);
				roundTreeDown(root, count);

				string confirm;
				cout << endl << "Search by apartment number?(Y/Other): ";
				cin >> confirm;
				if (confirm == "Y" || confirm == "y")
					break;
			}
			else
				cout << "Incorrect format, try again." << endl;
		}
		while (1)
		{
			cout << "Input apartment number or 'b' to back: ";
			cin >> apartmentNum;
			if (isNumber(apartmentNum))
				atNum = stoi(apartmentNum);
			else
			{
				if (apartmentNum == "b" || apartmentNum == "B")
				{
					system("cls");
					break;
				}
				cout << "Incorrect apartment number!" << endl;
				continue;
			}
			system("cls");
			count = 1;
			bool isFound = 0;

			cout << "\t\t" << "FIO" << "\t\t\t" << "Street name" << "\t   " << "House number" << "\t  " << "Apartment number" << "  " << "Date of settlement" << endl;
			treeSearch(root, count, atNum, isFound);

			if (!isFound)
			{
				system("cls");
				cout << "No one element is found" << endl;
			}
		}
	}
}

int Med(int L, int R, const vector<codeData>& codes)
{
	double sL = 0;
	int m = 0;

	for (int i = L; i < R; i++)
		sL += codes[i].frequency;

	double sR = codes[R].frequency;
	m = R;

	while (sL >= sR)
	{
		m--;
		sL -= codes[m].frequency;
		sR += codes[m].frequency;
	}
	return m;
}

void Fano(int L, int R, vector<codeData>& codes)
{
	if (L < R)
	{
		int m = Med(L, R, codes);
		for (int i = L; i <= R; i++)
		{
			if (i <= m)
				codes[i].code.push_back(0);
			else
				codes[i].code.push_back(1);
		}
		Fano(L, m, codes);
		Fano(m + 1, R, codes);
	}
}

double entropy(const vector<codeData>& codes)
{
	double entropy_ = 0;
	for (const auto &it : codes)
	{
		entropy_ += it.frequency * log2(it.frequency);
	}
	return -entropy_;
}

double averageL(const vector<codeData>& codes)
{
	double averageL_ = 0;
	for (const auto& it : codes)
	{
		averageL_ += it.frequency * it.code.size();
	}
	return averageL_;
}

bool symbolFilter(int code)
{
	if ((code > 127 && code < 256) ||(code > 47 && code < 58) || code == 45 || code == 95 || code == 32)
		return true;
	return false;
}

void coding(vector<codeData> &codes, int& sumFrequencies, bool& coded)
{
	if (!coded)
	{
		FILE* dataBase = fopen("testBase4.dat", "rb");
		int succes;
		unsigned char symbol;

		while (!feof(dataBase))
		{
			succes = fscanf(dataBase, "%c", &symbol);
			codes[static_cast<int>(symbol)].frequency++;
			sumFrequencies++;
		}
		fclose(dataBase);
		int vectorSize = codes.size();
		for (int i = 0; i < codes.size(); i++)
		{
			if (codes[i].frequency != 0)
				codes[i].symbol = i;
		}
		for (int i = vectorSize - 1; i >= 0; i--)
		{
			if (codes[i].frequency == 0)
				codes.erase(codes.begin() + i);
			else
				codes[i].frequency /= sumFrequencies;
		}
		int L = 0, R = codes.size() - 1;

		frequencySort(codes, L, R);
		Fano(L, R, codes);
		coded = true;
	}

	system("cls");
	cout << "Symbol\t" << "Frequency\t" << "WordCode\t" << "WordCodeLength" << fixed << endl;
	for (const auto &codesIt : codes)
	{
		cout << codesIt.symbol << "\t" << codesIt.frequency << "\t";
		for (const auto &codeIt : codesIt.code)
			cout << codeIt;
		cout << "\t\t" << codesIt.code.size() << endl;
	}
	cout << endl << "AverageLength < Entropy + 1" << endl;
	cout << averageL(codes) << " < " << entropy(codes) << " + " << 1 << endl;
	cout << "Number of characters: " << sumFrequencies << endl;
	cout << "Number of different characters: " << codes.size() << endl;
	system("pause");
}

void initMas(List*& head, vector <List*> &elements)
{
	List* p = head;
	for (auto& it : elements)
	{
		it = p;
		p = p->next;
	}
}

void printNextPage(List*&p, List*& backup, List*& tail, int& count)
{
	if (p == tail)
		return;
	system("cls");
	backup = p;
	cout << "\t\t" << "FIO" << "\t\t\t" << "Street name" << "\t   " 
		<< "House number" << "\t  " << "Apartment number" << "  " << "Date of settlement" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < elemOnPage; i++)
	{
		count++;
		cout << count << ") " << p->elem->FIO << "\t" << p->elem->streetName << "\t"
			<< p->elem->houseNumber << "\t\t" << p->elem->apartmentNumber << "\t\t" << p->elem->settlementDate << endl;
		p = p->next;
	}
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	if (p == NULL)
		p = tail;
}

void printPrevPage(List*&p, List*& backup, List*& tail, int& count)
{
	if (count == elemOnPage)
		return;
	count -= elemOnPage * 2;
	p = backup;
	for (int i = 0; i < elemOnPage; i++)
		p = p->prev;
	backup = p;
	printNextPage(p, backup, tail, count);
}

void showMenu(const bool &sort)
{
	cout << "[1]Next page" << "\t\t\t" << "[2]Previous page" << "\t\t" << "[3]Choose page" << endl;
	if (sort)
		cout << "[4]Sort List(Already sorted)";
	else
		cout << "[4]Sort List(Now not sorted)";
	cout << "\t" << "[5]Search(After sort)" << "\t\t" << "[6]Coding" << "\t" << "[7]Exit" << endl;
	cout << "Input command: ";
}

void readFile(Queue& q)
{
	int success = 0;
	FILE* buff;
	buff = fopen("testBase4.dat", "rb");

	q.head  = new List;
	q.head->prev = NULL;
	List* p = q.head;

	for (int i = 0; i < N; i++)
	{
		p->elem = new Elem;
		success = fread(p->elem, sizeof(Elem), 1, buff);
		if (i != N - 1)
		{
			p->next = new List;
			p->next->prev = p;
			p = p->next;
		}
		else
		{
			p->next = NULL;
			q.tail = p;
		}
	}
}

int main()
{
	List* p;
	Queue q;
	readFile(q);
	p = q.head;

	vector <List*> elements(N);
	vector<codeData> codes(256);
	short int action;
	int count = 0;
	bool sort = false;
	bool coded = false;
	int sumFrequencies = 0;
	List* backup = p;

	printNextPage(p, backup, q.tail, count);

	while (1)
	{
		showMenu(sort);
		cin >>  action;
		switch (action)
		{
		case 1:
			printNextPage(p, backup, q.tail, count);
			break;
		case 2:
			printPrevPage(p, backup, q.tail, count);
			break;
		case 3:
			count = startPage(p, q.head);
			printNextPage(p, backup, q.tail, count);
			break;
		case 4:
			if (!sort)
			{
				mergeSort(q.head, q.tail);
				p = q.head;
				count = 0;
				sort = true;
				initMas(q.head, elements);
				printNextPage(p, backup, q.tail, count);
			}
			break;
		case 5:
			if (sort)
			{
				searchAll(elements);
				p = backup;
				count -= elemOnPage;
				printNextPage(p, backup, q.tail, count);
			}
			break;
		case 6:
			coding(codes, sumFrequencies, coded);
			p = backup;
			count -= elemOnPage;
			printNextPage(p, backup, q.tail, count);
			break;
		case 7:
			p = q.head->next;
			while (p != q.tail)
			{
				free(p->prev);
				p = p->next;
			}
			free(p);
			elements.clear();
			codes.clear();
			return 0;
		}
	}
}