#include <fstream>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

class Animal
{
protected:

	int x, y, d, k;
	int age, speed, type;

public:

	Animal(int x, int y, int d, int k)
	{
		this->x = x; this->y = y; this->d = d; this->k = k; this->age = 0; this->speed = 1; this->type = 1;
	}

	void moveAnimal()
	{
		switch (d)
		{
		case 0:
			y -= speed;
			break;
		case 1:
			x += speed;
			break;
		case 2:
			y += speed;
			break;
		case 3:
			x -= speed;
			break;
		}
		if (!((age + 1) % k) && age)
			d++;
		d %= 4;
	}

	void ageAnimal()
	{
		age++;
	}

	  virtual bool reproductAnimal()
	{
		if (age == 5 || age == 10)
			return true;
		else
			return false;
	}

	virtual bool killAnimal()
	{
		if (age == 10)
			return true;
		else
			return false;
	}

	virtual void incSatiety() = 0;

	int getx()
	{
		return x;
	}

	int gety()
	{
		return y;
	}

	void period(int n, int m)
	{
		if (x < 0)
			x += n;
		if (x >= n)
			x %= n;
		if (y < 0)
			y += n;
		if (y >= n)
			y %= n;
	}

	int getSpeed()
	{
		return speed;
	}

	int getAge()
	{
		return age;
	}

	int getd()
	{
		return d;
	}

	int getk()
	{
		return k;
	}

	int getType()
	{
		return type;
	}

	virtual int getSatiety() = 0;
};

class Rabbit : public Animal
{
public:

	Rabbit(int x, int y, int d, int k) : Animal(x, y, d, k) { this->speed = 1; }

	virtual void incSatiety()
	{
		fout << "?";
	}

	virtual int getSatiety()
	{
		return -1;
	}
};

class Wolf : public Animal
{
protected:

	int satiety, pAge;

public:

	Wolf(int x, int y, int d, int k, int pAge = -1) : Animal(x, y, d, k)
	{
		this->pAge = pAge; this->satiety = 0; this->speed = 2; this->type = 2;
	}

	virtual bool reproductAnimal()
	{
		if (!(satiety % 2) && satiety)
		{
			satiety = 0;
			return true;
		}
		else
			return false;
	}

	virtual bool killAnimal()
	{
		if (age == 15)
			return true;
		else
			return false;
	}

	int getSatiety()
	{
		return satiety;
	}

	virtual void incSatiety()
	{
		this->satiety++;
	}

	int getpAge()
	{
		return pAge;
	}
};

class Hyena : public Wolf
{
public:

	Hyena(int x, int y, int d, int k, int pAge = -1) : Wolf(x, y, d, k, pAge)
	{
		this->type = 3;
	}
};

struct Node
{
	Animal* data;
	class Node* next;
	friend class List;

	Node(Animal* that) 
	{
		data = that;
		next = nullptr;
	}
};

class List
{
	Node* head;

	Node* prev(Node* node)
	{
		if (Empty())
			return nullptr;
		if (node == head)
			return nullptr;
		Node* t = head;
		while (t->next != node)
			t = t->next;
		return t;
	}

public:

	List() { head = nullptr; }
	~List()
	{
		this->clear();
	}
	bool Empty() { return head == nullptr; }
	Animal& getElement(Node* t) { return *t->data; }
	Node* getHead() { return head; }

	Node* next(Node* node)
	{
		if (Empty())
			return nullptr;
		return node->next;
	}

	Node* add(Animal* that, Node* node = nullptr)
	{
		Node* e = new Node(that);
		if (node == nullptr)
		{
			if (head == nullptr)
			{
				e->next = nullptr;
				head = e;
			}
			else
			{
				Node* p = head;
				while (p->next != nullptr)
					p = p->next;
				p->next = e;
			}
			return e;
		}
		e->next = node->next;
		node->next = e;
		return e;
	}

	Node* del(Node* node)
	{
		if (node == nullptr)
			return nullptr;
		if (node == head)
		{
			head = node->next;
			delete node;
			return head;
		}
		Node* p = this->prev(node);
		p->next = node->next;
		delete node;
		return p;
	}

	Node* getLast()
	{
		Node* t = head;
		while (next(t) != nullptr)
			t = next(t);
		return t;
	}

	void clear()
	{
		Node* t = head;
		if (t == nullptr)
			return;
		do
		{
			Node* h = t;
			t = this->next(t);
			delete h;
		} while (t != nullptr);
		head = nullptr;
	}
};

class Simulation
{
	int N, M, T;
	List animals;

public:

	Simulation(int n, int m, int t, int r = 0, int w = 0, int h = 0)
	{
		this->N = n; this->M = m; this->T = t;
		for (int i = 0; i < r; i++)
		{
			int x, y, d, k;
			fin >> x >> y >> d >> k;
			animals.add(new Rabbit(x, y, d, k));
		}
		for (int i = 0; i < w; i++)
		{
			int x, y, d, k;
			fin >> x >> y >> d >> k;
			animals.add(new Wolf(x, y, d, k));
		}
		for (int i = 0; i < h; i++)
		{
			int x, y, d, k;
			fin >> x >> y >> d >> k;
			animals.add(new Hyena(x, y, d, k));
		}
	}

	void Move()
	{
		Node* t = animals.getHead();
		if (t == nullptr)
			return;
		while (t != nullptr)
		{
			animals.getElement(t).moveAnimal();
			animals.getElement(t).period(N, M);
			t = animals.next(t);
		}
	}

	void Feed()
	{
		Node* t = animals.getHead();
		while (t != nullptr)
		{
			Node* p;
			switch (animals.getElement(t).getType())
			{
			case 1:
				break;
			case 2:
				p = animals.getHead();
				while (p != nullptr)
				{
					if (animals.getElement(p).getType() == 1 && (animals.getElement(t).getx() == animals.getElement(p).getx() && animals.getElement(t).gety() == animals.getElement(p).gety()))
					{
						Node* pp = p;
						p = animals.next(p);
						animals.del(pp);
						animals.getElement(t).incSatiety();
					}
					else
						p = animals.next(p);
				}
				t = nullptr;
				break;
			case 3:
				p = animals.getHead();
				while (p != nullptr)
				{
					if (p != t)
					{
						if (animals.getElement(t).getSatiety() < 2 && (animals.getElement(t).getx() == animals.getElement(p).getx() && animals.getElement(t).gety() == animals.getElement(p).gety()))
						{
							Node* pp = p;
							p = animals.next(p);
							animals.del(pp);
							animals.getElement(t).incSatiety();
						}
						else
							break;
					}
					else
						p = animals.next(p);
				}
				t = nullptr;
				break;
			}
			if (t != nullptr)
				t = animals.next(t);
		}
	}

	void Aging()
	{
		Node* t = animals.getHead();
		if (t == nullptr)
			return;
		while (t != nullptr)
		{
			animals.getElement(t).ageAnimal();
			t = animals.next(t);
		}
	}

	void Reproduction()
	{
		Node* t = animals.getHead();
		if (t == nullptr)
			return;
		while (t != nullptr)
		{
			if (animals.getElement(t).reproductAnimal())
				if (animals.getElement(t).getType() == 1)
					animals.add(new Rabbit(animals.getElement(t).getx(), animals.getElement(t).gety(), animals.getElement(t).getd(), animals.getElement(t).getk()));
				else if (animals.getElement(t).getType() == 2)
					animals.add(new Wolf(animals.getElement(t).getx(), animals.getElement(t).gety(), animals.getElement(t).getd(), animals.getElement(t).getk(), animals.getElement(t).getAge()));
				else
					animals.add(new Hyena(animals.getElement(t).getx(), animals.getElement(t).gety(), animals.getElement(t).getd(), animals.getElement(t).getk(), animals.getElement(t).getAge()));
			t = animals.next(t);
		}
	}

	void Extinction()
	{
		Node* t = animals.getHead();
		if (t == nullptr)
			return;
		while (t != nullptr)
		{
			if (animals.getElement(t).killAnimal())
			{
				Node* temp = t;
				t = animals.next(t);
				animals.del(temp);
			}
			else
				t = animals.next(t);
		}
	}

	void Start()
	{
		for (int i = 0; i < this->T; i++)
		{
			Move();
			Feed();
			Aging();
			Reproduction();
			Extinction();
		}
	}

	void Print()
	{
		int** field = new int* [N];
		for (int i = 0; i < N; i++)
			field[i] = new int[M];
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				field[i][j] = 0;
		Node* t = animals.getHead();
		while (t != nullptr)
		{
			int tx = animals.getElement(t).getx(), ty = animals.getElement(t).gety();
			if (animals.getElement(t).getType() == 1)
				field[tx][ty]++;
			else
				field[tx][ty]--;
			t = animals.next(t);
		}
		for (int j = 0; j < M; j++)
		{
			for (int i = 0; i < N; i++)
				if (!field[i][j])
					fout << '#';
				else
					fout << field[i][j];
			fout << endl;
		}
		fout << endl;
		for (int i = 0; i < N; i++)
			delete[] field[i];
		delete[] field;
	}
};

int main()
{
	int n, m, t, r, w;
	fin >> n >> m >> t >> r >> w;
	Simulation thisIsMadness(n, m, t, r, w);
	thisIsMadness.Start();
	thisIsMadness.Print();
	fin.close();
	fout.close();
	return 0;
}
