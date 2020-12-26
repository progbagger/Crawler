#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <set>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

struct Adj_matrix
{
    vector<vector<string>> table;
    int size;

    void rawCopy(const Adj_matrix& that)
    {
        this->size = that.size;
        table.resize(size);
        for (int i = 0; i < size; i++)
        {
            table[i].resize(size);
            for (int j = 0; j < size; j++)
                this->table[i][j] = that.table[i][j];
        }
    }

    void rawClean() {}

    Adj_matrix() { size = 0; };

    Adj_matrix(const Adj_matrix& that)
    {
        rawCopy(that);
    }

    ~Adj_matrix()
    {
        rawClean();
    }

    Adj_matrix& operator = (const Adj_matrix& that)
    {
        if (this != &that)
        {
            rawClean();
            rawCopy(that);
        }
        return *this;
    }

    Adj_matrix(int s, int p, vector<vector<int>>& from_to, const string& weights)
    {
        size = s;
        table.resize(size);
        for (int i = 0; i < size; i++)
            table[i].resize(size);
        for (int i = 0; i < p; i++)
            table[from_to[i][0]][from_to[i][1]] += weights[i];
        for (auto& i : table)
            for (auto& j : i) {
                for (int k = 0; k < j.length(); k++)
                    for (int l = k + 1; l < j.length(); l++)
                        if (j[k] == j[l])
                            j.erase(l);
            }
    }

    void print() {
        fout << '\t';
        for (int i = 0; i < size; i++)
            fout << i << '\t';
        fout << endl;
        int counter = 0;
        for (const auto &i : table) {
            fout << counter++ << '\t';
            for (const auto &j : i)
                if (j.empty())
                    fout << "-\t";
                else
                    fout << j << '\t';
            fout << endl;
        }
    }
};

class Automaton
{
    int size, top, end_count;
    set<int> end;
    Adj_matrix m;

    void rawCopy(const Automaton& that)
    {
        size = that.size;
        top = that.top;
        end_count = that.end_count;
        end = that.end;
        m = Adj_matrix(that.m);
    }

public:

    Automaton() { size = 0; top = end_count = -1; }

    Automaton(const Automaton& that) { rawCopy(that); }

    ~Automaton() {}

    Automaton& operator = (const Automaton& that) {
        if (this != &that)
            rawCopy(that);
        return *this;
    }

    Automaton(const int& s, const int& start, const int& end, set<int>& f_end, const int& p, vector<vector<int>>& from_to, const string& weights) {
        size = s; end_count = end;
        m = Adj_matrix(size, p, from_to, weights);
        top = start;
        this->end = f_end;
    }

    Automaton(const string& expression) {
        int new_top = 0;
        int new_size = 1;
        int state_before_brackets = 0;
        int prev_state = 0;
        vector<vector<int>> transitions;
        set<int> terminate_states;
        string transitions_weights;
        vector<int> temp_states;
        string temp_weights;
        for (int i = 0; i < expression.length() - 1; i++) {
            if (expression[i] == '(')
                state_before_brackets = new_size - 1;
            else if (expression[i] == ')' && expression[i + 1] == '*') {
                prev_state = state_before_brackets;
                for (int j = 0; j < temp_states.size(); j++) {
                    transitions.push_back(vector<int>{temp_states[j], state_before_brackets});
                    transitions_weights.push_back(temp_weights[j]);
                }
                temp_weights.clear();
                temp_states.clear();
                i++;
            }
            else if (expression[i] == ')') {
                new_size++;
                for (int j = 0; j < temp_states.size(); j++) {
                    transitions.push_back(vector<int>{temp_states[j], new_size - 1});
                    transitions_weights.push_back(temp_weights[j]);
                }
                temp_weights.clear();
                temp_states.clear();
                prev_state = new_size - 1;
            }
            else if (expression[i + 1] == ')') {
                temp_states.push_back(new_size - 1);
                temp_weights.push_back(expression[i]);
            }
            else if (expression[i + 1] == '*') {
                transitions.push_back(vector<int>{prev_state, prev_state});
                transitions_weights.push_back(expression[i]);
                i++;
            }
            else if (expression[i + 1] == '|') {
                temp_states.push_back(new_size - 1);
                temp_weights.push_back(expression[i]);
                prev_state = state_before_brackets;
                i++;
            }
            else {
            new_size++;
            transitions.push_back(vector<int>{prev_state, new_size - 1});
            transitions_weights.push_back(expression[i]);
            prev_state = new_size - 1;
            }
        }
        if (!expression.empty()) {
            if (expression[expression.length() - 1] != ')' && expression[expression.length() - 1] != '*') {
                new_size++;
                transitions.push_back(vector<int>{prev_state, new_size - 1});
                transitions_weights.push_back(expression[expression.length() - 1]);
                prev_state = new_size - 1;
            }
            else if (expression[expression.length() - 1] == ')') {
                new_size++;
                for (int j = 0; j < temp_states.size(); j++) {
                    transitions.push_back(vector<int>{temp_states[j], new_size - 1});
                    transitions_weights.push_back(temp_weights[j]);
                }
                temp_weights.clear();
                temp_states.clear();
            }
        }
        terminate_states.insert(new_size - 1);
        *this = Automaton(new_size, new_top, 1, terminate_states, transitions.size(), transitions, transitions_weights).determinate();
    }

    void print() {
        fout << size << endl << top << endl << end_count << endl;
        for (const auto i : end)
            fout << i << endl;
        int transitions_count = 0;
        for (int i = 0; i < m.size; i++)
            for (int j = 0; j < m.size; j++)
                if (!m.table[i][j].empty())
                    transitions_count += m.table[i][j].length();
        fout << transitions_count << endl;
        for (int i = 0; i < m.size; i++)
            for (int j = 0; j < m.size; j++)
                if (!m.table[i][j].empty())
                    fout << i << ' ' << j << ' ' << m.table[i][j] << endl;
        fout << endl;
        m.print();
        fout << endl;
    }

    bool check_end(int state)
    {
        bool result = false;
        for (auto& i : this->end)
            if (state == i)
            {
                result = true;
                break;
            }
        return result;
    }

    void detect(const string& expression, int c_index = -1)
    {
        static string c_expression;
        static bool result = false, end = false;
        static int counter = 0;
        counter++;
        int c = c_index;
        if (c == -1)
            c = top;
        for (int i = 0; i < size; i++)
        {
            if (result && c_expression.empty())
            {
                result = end = false;
                fout << "YES\n";
                counter = 0;
                return;
            }
            else if (result)
            {
                c_expression.pop_back();
                counter--;
                return;
            }
            if (c_expression.length() == expression.length())
            {
                if (c_expression == expression && check_end(c))
                {
                    result = true;
                    c_expression.pop_back();
                    counter--;
                    return;
                }
                else
                {
                    c_expression.pop_back();
                    counter--;
                    return;
                }
            }
            for (int j = 0; j < m.table[c][i].length(); j++)
            {
                if (result && c_expression.empty())
                {
                    result = end = false;
                    counter = 0;
                    fout << "YES\n";
                    return;
                }
                else if (result)
                {
                    c_expression.pop_back();
                    counter--;
                    return;
                }
                if (m.table[c][i][j] == expression[c_expression.length()])
                {
                    c_expression += m.table[c][i][j];
                    detect(expression, i);
                }
            }
        }
        if (counter == 1)
            end = true;
        if (!result && end)
        {
            fout << "NO\n";
            result = end = false;
            c_expression = "";
            counter = 0;
            return;
        }
        else if (result && end)
        {
            fout << "YES\n";
            result = end = false;
            c_expression = "";
            counter = 0;
            return;
        }
        counter--;
        c_expression.pop_back();
    }

    Automaton& determinate()
    {
        set<char> alphabet;
        for (int i = 0; i < m.size; i++)
            for (int j = 0; j < m.size; j++)
                for (const auto& k : m.table[i][j])
                    alphabet.insert(k);
        set<int> first{ this->top };
        vector<vector<set<int>>> new_transitions;
        string new_weights;
        list<set<int>> q;
        q.push_back(first);
        set<set<int>> former /* множество всех состояний нового автомата */, new_end;
        former.insert(first);
        // основной цикл
        while (!q.empty())
        {
            set<int> current(q.front());
            q.pop_front();
            // начало: проходимся по известному алфавиту и смотрим, куда ведут
            // переходы по каждому символу
            for (const auto& i : alphabet)
            {
                set<int> new_current; // новое множество-кандидат на добавление в очередь
                for (const auto& j : current)
                    for (int k = 0; k < m.size; k++)
                        for (const auto& l : m.table[j][k])
                            if (i == l)
                            {
                                new_current.insert(k);
                                break;
                            }
                if (!new_current.empty()) { // построение перехода из множества из очереди в построенное множество
                    new_transitions.push_back(vector<set<int>>{current, new_current});
                    new_weights += i;
                }
                // проверка на конечное состояние
                bool set_end_flag = false;
                for (const auto& j : this->end)
                    for (const auto& k : new_current)
                        if (j == k) {
                            set_end_flag = true;
                            goto end_set_end_check;
                        }
            end_set_end_check:;
                if (set_end_flag)
                    new_end.insert(new_current);
                // проверка на необходимость добавления множества в очередь
                bool set_checked = false;
                for (const auto& k : former)
                    if (new_current == k)
                    {
                        set_checked = true;
                        break;
                    }
                if (!set_checked)
                    q.push_back(new_current);
                former.insert(new_current);
            }
        }
        // перенумерация состояний
        int new_size = former.size();
        int new_start = this->top;
        int new_transitions_count = new_transitions.size();
        int new_terminate_count = new_end.size();
        // перенумерация собственной персоной
        int current_state_counter = 0;
        set<int> new_terminate_states;
        // проверка начального состояния на терминальное состояние
        for (auto& i : new_end)
            if (i == first) {
                new_terminate_states.insert(new_start);
            }
        // удаление начальной вершины из множества всех вершин и конечных состояний, так как она уже перенумерована с самого начала
        former.erase(first);
        new_end.erase(first);
        // перенумерация всего
        auto i = former.begin();
        if (new_start == 0)
            current_state_counter = 1;
        for (i; i != former.end(); i++) {
            if (new_start == current_state_counter) {
                current_state_counter++;
                i--;
                continue;
            }
            for (auto& j : new_transitions) {
                if (j[0] == *i)
                    j[0] = set<int>{ current_state_counter };
                if (j[1] == *i)
                    j[1] = set<int>{ current_state_counter };
            }
            for (const auto& j : new_end)
                if (*i == j) {
                    new_terminate_states.insert(current_state_counter);
                    break;
                }
            current_state_counter++;
        }
        // новые перенумерованные переходы
        vector<vector<int>> new_from_to;
        for (auto i : new_transitions) {
            auto j1 = i[0].begin();
            auto j2 = i[1].begin();
            new_from_to.push_back(vector<int>{*j1, * j2});
        }
        *this = Automaton(new_size, new_start, new_terminate_count, new_terminate_states, new_transitions_count, new_from_to, new_weights);
        return *this;
    }
};

int main()
{

    // Запись входных данных
    int N, k, f, p, T;
    set<int> f_end;
    fin >> N >> k >> f;
    for (int i = 0; i < f; i++)
    {
        int temp;
        fin >> temp;
        f_end.insert(temp);
    }
    fin >> p;
    vector<vector<int>> from_to(p);
    for (int i = 0; i < p; i++)
        from_to[i].resize(2);
    string weights;
    for (int i = 0; i < p; i++)
    {
        char temp;
        fin >> from_to[i][0] >> from_to[i][1] >> temp;
        weights += temp;
    }
    // Сам автомат
    Automaton A(N, k, f, f_end, p, from_to, weights);
    A.determinate();
    fin >> T;
    string expression;
    for (int i = 0; i < T; i++)
    {
        fin >> expression;
        A.detect(expression);
    }

    /*
    string expression;
    fin >> expression;
    Automaton A(expression);
    A.print();
    int N;
    fin >> N;
    for (int i = 0; i < N; i++) {
        string temp;
        fin >> temp;
        A.detect(temp);
    }
    */
    
    fin.close();
    fout.close();
    return 0;
}
