#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#define SIZE 2000
using namespace std;

const int m = int(pow(10, 9) + 9);
const int p = 31;
int curr = 0;
struct company
{
	string name;
	string profit_tax;
	string address;
};

vector<company>read_file(string file_name)
{
	ifstream ifs;
	ifs.open(file_name);
	if (!ifs)
	{
		cout << " can't open file, please check again.\n";
		exit(1);
	}
	
	vector<company> List_company;
	string temp;
	company c;
	
	getline(ifs, temp);// bo dong dau tien - dong gioi thieu
	while (!ifs.eof())
	{
		getline(ifs, temp, '|');
		c.name = temp;
		getline(ifs, temp, '|');
		c.profit_tax = temp;
		getline(ifs, temp, '\n');
		c.address = temp;

		List_company.push_back(c);
	}
	ifs.close();
	return List_company;
}


unsigned long long exponential(int x, int expo) // tim so mu 
{
	unsigned long long result = 1;
	for (int i = 0; i < expo; i++)
	{
		result = result * x;
		result = result % m;
	}

	return result;
}

unsigned long long Hashing(string company_name)
{
	unsigned long long index = 0;
	int start = 0;

	if (company_name.size() > 20)
	{
		start = company_name.size() - 20;
	}

	for (size_t i = start; i < company_name.size(); i++)
	{
		index = index + (unsigned long long)((int)(company_name[i]) * exponential(p, i)) % m;
	}

	index = index % m;

	return index;
}

void insert(company*& hash_table, company c)
{ 
	if (++curr <= SIZE)
	{
		int index = Hashing(c.name);
		index = index % SIZE;
		while (hash_table[index].name != "")
		{
			index = (index + 1) % SIZE;
		}
		hash_table[index] = c;
	}
}

company* CreateHashTable(vector<company> List)
{
	company* hash_table = new company[SIZE];

	for (size_t i = 0; i < List.size(); i++)
	{
		insert(hash_table, List[i]);
	}

	return hash_table;
}

company* Search(company* hash_table, string company_name)
{
	int index = Hashing(company_name);
	index = index % SIZE;
	int cur = index;

	do {
		if (hash_table[cur].name == company_name)
		{
			return &hash_table[cur];
		}
		else
		{
			cur = (cur + 1) % SIZE;
		}
	} while (cur != index);

	return nullptr;
}

void Traverse(company* hash_table)
{
	for (int i = 0; i < SIZE; i++)
	{
		if (hash_table[i].name != "")
		{
			cout << "\nBucket " << i << " : ";
			cout << "\n	Company name: " << hash_table[i].name;
			cout << "\n	Company profit tax: " << hash_table[i].profit_tax;
			cout << "\n	Company address: " << hash_table[i].address;
		}
	}
}


int main()
{
	company* hash_table = new company[SIZE];
	vector<company> list_data = read_file("MST.txt");
	company insert_company, * search_company;

	hash_table = CreateHashTable(list_data);

	Traverse(hash_table );

	insert_company.name = "CONG TY TNHH NANG MAI";
	insert_company.profit_tax = "113113113";
	insert_company.address = "So 333 Nguyen Trai, Quan 1, Thanh pho Ho Chi Minh";

	
	insert(hash_table, insert_company);
	Traverse(hash_table);

	
	search_company = Search(hash_table, "CONG TY TNHH THUONG MAI DICH VU THIET BI M.K.K");
	cout << "\n \n Information of CONG TY TNHH THUONG MAI DICH VU THIET BI M.K.K:\n";
	cout << "- Name: " << search_company->name;
	cout << "\n- Profit tax: " << search_company->profit_tax;
	cout << "\n- Address: " << search_company->address;

	delete[] hash_table;
	return 0;
}