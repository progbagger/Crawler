#include <fstream>
#include <string>
#include <list>

std::ifstream fin("input.txt");
std::ofstream fout("output.txt");

class Matrix
{
    int** M; int size;
    void rawClean()
    {
        for (int i = 0; i < this->size; i++)
            delete[] this->M[i];
        delete[] this->M;
    }

    void rawCopy(const Matrix& that)
    {
        this->size = that.size;
        this->M = new int* [this->size];
        for (int i = 0; i < this->size; i++)
            this->M[i] = new int[this->size];
        for (int i = 0; i < this->size; i++)
            for (int j = 0; j < this->size; j++)
                this->M[i][j] = that.M[i][j];
    }

    class Vector // вспомогательный класс для реализации доп. заданий 1 и 2 (взятие строки и столбца)
    {
        void rawClean()
        {
            delete[] this->V;
        }

        void rawCopy(const Vector& that)
        {
            this->size = that.size;
            this->V = new int* [this->size];
            for (int i = 0; i < this->size; i++)
                this->V[i] = that.V[i];
        }

    public:
        int** V, size;
        Vector()
        {
            this->size = 0;
            V = nullptr;
        }

        Vector(int s)
        {
            this->size = s;
            V = new int* [this->size];
            for (int i = 0; i < this->size; i++)
                V[i] = 0;
        }

        Vector(const Vector& that)
        {
            this->rawCopy(that);
        }

        ~Vector()
        {
            this->rawClean();
        }

        Vector& operator = (const Vector& that)
        {
            if (this != &that)
            {
                this->rawClean();
                this->rawCopy(that);
            }
            return *this;
        }

        int& operator[](int r) // корректный доступ к элементам строки/столбца
        {
            return *this->V[r - 1];
        }
    };

public:
    Matrix() // конструктор по умолчанию
    {
        size = 0;
        M = nullptr;
    }

    Matrix(const Matrix& that) // конструктор копирования
    {
        this->rawCopy(that);
    }

    ~Matrix()
    {
        this->rawClean();
    }

    Matrix(int s) // конструктор, создающий единичную матрицу размера s
    {
        this->size = s;
        M = new int* [this->size];
        for (int i = 0; i < this->size; i++)
            M[i] = new int[size];
        for (int i = 0; i < this->size; i++)
            for (int j = 0; j < this->size; j++)
                if (i == j)
                    M[i][j] = 1;
                else
                    M[i][j] = 0;
    }

    Matrix(int s, int* digits) // конструктор, создающий матрицу размера s и заполняющий её диагональ элементами из массива digits, а остальные элементы нулями
    {
        this->size = s;
        M = new int* [size];
        for (int i = 0; i < this->size; i++)
            M[i] = new int[size];
        for (int i = 0; i < this->size; i++)
            for (int j = 0; j < this->size; j++)
            {
                if (i == j)
                    M[i][j] = digits[i];
                else
                    M[i][j] = 0;
            }
    }

    void Print() // функция вывода матрицы
    {
        for (int i = 0; i < this->size; i++)
        {
            for (int j = 0; j < this->size; j++)
                fout << this->M[i][j] << '\t';
            fout << std::endl;
        }
    }

    void Enter() // функция ввода матрицы в программу
    {
        for (int i = 0; i < this->size; i++)
            for (int j = 0; j < this->size; j++)
                fin >> this->M[i][j];
    }

    Matrix& operator =(const Matrix& that)
    {
        if (this != &that)
        {
            this->rawClean();
            this->rawCopy(that);
        }
        return *this;
    }

    Matrix operator +(Matrix a)
    {
        if (this->size != a.size)
        {
            fout << "Error: different sizes";
            exit(0);
        }
        else
        {
            Matrix result(this->size);
            for (int i = 0; i < result.size; i++)
                for (int j = 0; j < result.size; j++)
                    result.M[i][j] = this->M[i][j] + a.M[i][j];
            return result;
        }
    }

    Matrix operator -(Matrix a)
    {
        if (this->size != a.size)
        {
            fout << "Error: different sizes";
            exit(0);
        }
        else
        {
            Matrix result(this->size);
            for (int i = 0; i < result.size; i++)
                for (int j = 0; j < result.size; j++)
                    result.M[i][j] = this->M[i][j] - a.M[i][j];
            return result;
        }
    }

    Matrix operator *(Matrix a)
    {
        if (this->size != a.size)
        {
            fout << "Error: different sizes";
            exit(0);
        }
        else
        {
            Matrix result(this->size);
            for (int i = 0; i < result.size; i++) // зануление результирующей матрицы
                result.M[i][i] = 0;
            for (int i = 0; i < result.size; i++)
                for (int j = 0; j < result.size; j++)
                    for (int k = 0; k < result.size; k++)
                        result.M[i][j] += this->M[i][k] * a.M[k][j];
            return result;
        }
    }

    bool operator ==(Matrix a)
    {
        bool result = 1;
        if (this->size != a.size)
            result = 0;
        else
            for (int i = 0; i < this->size; i++)
                for (int j = 0; j < this->size; j++)
                    if (this->M[i][j] != a.M[i][j])
                    {
                        result = 0;
                        goto end;
                    }
    end:
        return result;
    }

    bool operator !=(Matrix a)
    {
        bool result = 0;
        if (this->size != a.size)
            result = 1;
        else
            for (int i = 0; i < this->size; i++)
                for (int j = 0; j < this->size; j++)
                    if (this->M[i][j] == a.M[i][j])
                    {
                        result = 1;
                        break;
                    }
        return result;
    }

    Matrix operator !() // оператор транспонирования матрицы
    {
        Matrix result(this->size);
        for (int i = 0; i < this->size; i++)
            for (int j = 0; j < this->size; j++)
                result.M[i][j] = this->M[j][i];
        return result;
    }

    Matrix operator ()(int a, int b) // взятие минора - матрица без строки a и столбца b
    {
        Matrix result(this->size - 1);
        for (int i = 0; i < a - 1; i++)
            for (int j = 0; j < b - 1; j++)
                result.M[i][j] = this->M[i][j];
        for (int i = a - 1; i < result.size; i++)
            for (int j = 0; j < b - 1; j++)
                result.M[i][j] = this->M[i + 1][j];
        for (int i = 0; i < a - 1; i++)
            for (int j = b - 1; j < result.size; j++)
                result.M[i][j] = this->M[i][j + 1];
        for (int i = a - 1; i < result.size; i++)
            for (int j = b - 1; j < result.size; j++)
                result.M[i][j] = this->M[i + 1][j + 1];
        return result;
    }

    Vector operator [](int r) // оператор взятия строки
    {
        Vector result(this->size);
        for (int i = 0; i < this->size; i++)
            result.V[i] = &this->M[r - 1][i];
        return result;
    }

    Vector operator ()(int r) // оператор взятия столбца
    {
        Vector result(this->size);
        for (int i = 0; i < result.size; i++)
            result.V[i] = &this->M[i][r - 1];
        return result;
    }

    unsigned int getSize()
    {
        return this->size;
    }

    int subHash() const
    {
        if (!size)
            return -1;
        else
            return M[0][0] + M[0][size - 1] + M[size - 1][0] + M[size - 1][size - 1];
    }
};

template <typename K>
size_t hashick(const K& k)
{
    std::hash<K> hasher;
    return hasher(k);
}

template <>
size_t hashick(const Matrix& k)
{
    std::hash<int> hasher;
    return hasher(k.subHash());
}

template <typename K, typename V>
struct Pair
{
    K key; V value; bool status;

    Pair() : key(), value(), status(false) {}

    Pair(const K& k, const V& v) : key(k), value(v), status(true) {}

    ~Pair() {}

    Pair& operator = (const Pair& that)
    {
        if (this != &that)
        {
            key = that.key;
            value = that.value;
            status = that.status;
        }
        return *this;
    }

    bool operator == (const Pair& that)
    {
        return (key == that.key) && (value == that.value) && (status == that.status);
    }
};

template <typename K, typename V>
class HashMap
{
protected:

    unsigned int size, c_size;
    float loadF;
    Pair<K, V>* data;

    void rawClean()
    {
        delete[] data;
    }

    void rawCopy(const HashMap<K, V>& that)
    {
        size = that.size;
        c_size = that.c_size;
        loadF = that.loadF;
        data = new Pair<K, V>[size + 1];
        for (unsigned int i = 0; i <= size; i++)
            data[i] = that.data[i];
    }

public:

    HashMap(int s = 8, float l = 0.75) : size(s), c_size(0), loadF(l)
    {
        data = new Pair<K, V>[size + 1];
    }

    HashMap(const HashMap<K, V>& that)
    {
        rawCopy(that);
    }

    ~HashMap()
    {
        rawClean();
    }

    HashMap& operator = (const HashMap& that)
    {
        if (this != &that)
        {
            rawClean();
            rawCopy(that);
        }
        return *this;
    }

    unsigned int getSize()
    {
        return c_size;
    }

    unsigned int hash1(const K& k)
    {
        return hashick(k) % size;
    }

    unsigned int hash2(const K& k)
    {
        return hashick(k) % (size - 1) + 1;
    }

    void rehash(unsigned int s)
    {
        HashMap tMap(s);
        for (int i = 0; i < size; i++)
            if (data[i].status)
                tMap.add(data[i].key, data[i].value);
        *this = tMap;
    }

    V& get(const K& k)
    {
        unsigned int index1 = this->hash1(k);
        if (data[index1].key == k)
            return data[index1].value;
        else
        {
            unsigned int index2 = hash2(k);
            if (data[index2].key == k)
                return data[index2].value;
            else
            {
                unsigned int counter = 0;
                for (int i = (index2 + index1) % size; counter++ < size; i = (i + 1) % size)
                    if (data[i].key == k)
                        return data[i].value;
            }
        }
    }

    virtual void add(const K& k, const V& v)
    {
        if (c_size / size >= loadF)
            rehash(size * 2);
        unsigned int index1 = hash1(k);
        if (data[index1].status && data[index1].key != k)
        {
            unsigned int index2 = hash2(k);
            if (data[index2].status && data[index2].key != k)
            {
                unsigned int counter = 0;
                for (int i = (index2 + index1) % size; counter++ < size; i = (i + 1) % size)
                {
                    if (data[i].status == 0)
                    {
                        data[i].value = v; data[i].key = k; data[i].status = true;
                        ++c_size;
                        break;
                    }
                    else if (data[i].status == 1 && data[i].key == k)
                    {
                        data[i].value = v;
                        break;
                    }
                }
            }
            else if (data[index2].status && data[index2].key == k)
                data[index2].value = v;
            else
            {
                data[index2].value = v; data[index2].key = k; data[index2].status = true;
                ++c_size;
            }
        }
        else if (data[index1].status && data[index1].key == k)
            data[index1].value = v;
        else
        {
            data[index1].value = v; data[index1].key = k; data[index1].status = true;
            ++c_size;
        }
    }

    virtual void remove(K& k)
    {
        unsigned int index1 = hash1(k);
        if (data[index1].key == k)
        {
            data[index1].status = false;
            --c_size;
        }
        else
        {
            unsigned int index2 = hash2(k);
            if (data[index2].key == k)
            {
                data[index2].status = false;
                --c_size;
            }
            else
            {
                unsigned int counter = 0;
                for (int i = (index2 + index1) % size; counter++ < size; i = (i + 1) % size)
                    if (data[i].key == k)
                    {
                        data[i].status = false;
                        --c_size;
                        break;
                    }
            }
        }
        if (c_size / size <= 1 - loadF)
            rehash(size / 2);
    }

    Pair<K, V>* next(Pair<K, V>* that)
    {
        that++;
        while (!that->status && that != &data[size])
            that++;
        return that;
    }

    class Iterator
    {
        Pair<K, V>* current;
        HashMap* those;

    public:

        Iterator() : current(nullptr), those(nullptr) {}

        Iterator(const Iterator& that)
        {
            current = that.current;
            those = that.those;
        }

        Iterator(Pair<K, V>& that, HashMap<K, V>& thatt) : current(&that), those(&thatt)
        {
            current = &that;
            those = &thatt;
        }

        ~Iterator() {}

        Iterator& operator = (const Iterator& that)
        {
            if (this != &that)
            {
                current = &that.current;
                those = &that.those;
            }
            return *this;
        }

        Iterator& operator ++ ()
        {
            current = those->next(current);
            return *this;
        }

        bool operator != (const Iterator& that)
        {
            return (current->status != that.current->status) || (current->key != that.current->key) || (current->value != that.current->value);
        }

        Pair<K, V>* operator -> ()
        {
            return current;
        }
        Pair<K, V>& operator * ()
        {
            return *current;
        }
    };

    Iterator begin()
    {
        int i = 0;
        while (!data[i].status && i < size)
            i++;
        Iterator result(data[i], *this);
        return result;
    }

    Iterator end()
    {
        int i = size - 1;
        while (!data[i].status && i >= 0)
            i--;
        if (i == -1)
            i = size - 1;
        Iterator result(data[i + 1], *this);
        return result;
    }

    unsigned int calculateUnicue()
    {
        if (!c_size)
            return 0;
        unsigned int result = 0; bool unique;
        for (int i = 0; i < size; i++)
        {
            unique = true;
            if (data[i].status) {
                for (int j = 0; j < i; j++)
                    if (data[j].status && data[i].value == data[j].value)
                    {
                        unique = false;
                        break;
                    }
                if (unique)
                    result++;
            }
        }
        return result;
    }
};

template <typename K, typename V>
class MultiHashMap : public HashMap<K, V>
{
public:

    void add(K k, V v)
    {
        unsigned int index1 = this->hash1(k);
        if (this->data[index1].status)
        {
            unsigned int index2 = this->hash2(k);
            if (this->data[index2].status)
            {
                for (int i = (index2 + index1) % this->size; true; i = (i + 1) % this->size)
                    if (!this->data[i].status)
                    {
                        this->data[i].value = v; this->data[i].key = k; this->data[i].status = true;
                        break;
                    }
            }
            else
                this->data[index2].value = v; this->data[index2].key = k; this->data[index2].status = true;
        }
        else
            this->data[index1].value = v; this->data[index1].key = k; this->data[index1].status = true;
        ++this->c_size;
        if (this->c_size / this->size >= this->loadF)
            this->rehash(this->size * 2);
    }

    void remove(K k)
    {
        for (int i = 0; i < this->size; i++)
            if (this->data[i].status && this->data[i].key == k)
                this->data[i].status = false;
        if (this->c_size / this->size <= 1 - this->loadF)
            this->rehash(this->size / 2);
    }

    std::list<V> getAll(K k)
    {
        std::list<V> result;
        for (int i = 0; i < this->size; i++)
            if (this->data[i].status)
                if (this->data[i].key == k)
                    result.push_back(this->data[i].value);
        return result;
    }

    unsigned int countAll(K k)
    {
        unsigned int result = 0;
        for (int i = 0; i < this->size; i++)
            if (this->data[i].status)
                if (this->data[i].key == k)
                    ++result;
        return result;
    }
};

template <typename K, typename V>
void execute(int n)
{
    HashMap<K, V> map(8);
    std::string c; K k; V v;
    for (int i = 0; i < n; i++)
    {
        fin >> c;
        if (c == "A")
        {
            fin >> k >> v;
            map.add(k, v);
        }
        if (c == "R")
        {
            fin >> k;
            map.remove(k);
        }
    }
    fout << map.getSize() << ' ' << map.calculateUnicue() << std::endl;
    for (typename HashMap<K, V>::Iterator i = map.begin(); i != map.end(); ++i)
        fout << i->key << ' ' << i->value << std::endl;
}

int main()
{
    std::string k, t;
    int n;
    fin >> k >> t >> n;
    if (k == "I")
    {
        if (t == "I")
            execute<int, int>(n);
        if (t == "D")
            execute<int, double>(n);
        if (t == "S")
            execute<int, std::string>(n);
    }
    if (k == "D")
    {
        if (t == "I")
            execute<double, int>(n);
        if (t == "D")
            execute<double, double>(n);
        if (t == "S")
            execute<double, std::string>(n);
    }
    if (k == "S")
    {
        if (t == "I")
            execute<std::string, int>(n);
        if (t == "D")
            execute<std::string, double>(n);
        if (t == "S")
            execute<std::string, std::string>(n);
    }

    fin.close();
    fout.close();
    return 0;
}
