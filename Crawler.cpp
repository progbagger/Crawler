#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <vector>
#include <queue>
#include <set>
#include <string>

using namespace std;

// ввод/вывод обычный
ifstream fin("input.txt");
ofstream fout("output.txt");

// сборка ссылки (ссылка имеет вид <a href="<протокол>://<адрес>">)
const string protocol = "file://", reference_start = "<a href=\"" + protocol, reference_end = "\">";

// местоположения выгрузки/загрузки
const string download = "D:\\5.1.1\\Test\\", upload = "D:\\5.1.1\\Downloaded\\";

// определяем максимальное количество потоков
const size_t max_threads = thread::hardware_concurrency();

// сам "паучок"
class Crawler
{
private:

	// управление потоками
	size_t thread_count;
	vector<thread> threads;
	atomic<size_t> workers_number;
	mutex locker;

	// вся очередь и множество посещённых адресов
	queue<string> q;
	set<string> visited;

	// время работы
	long time;

public:

	Crawler(const size_t& threads_number = max_threads)
	{
		thread_count = threads_number;
		time = 0;
		workers_number = 0;
	}

	void copy_file(const string& file_name)
	{
		ifstream t_fin(download + file_name);
		ofstream t_fout(upload + file_name);
		t_fout << t_fin.rdbuf();
		t_fin.close();
		t_fout.close();
	}

	string get_file() // получение файла из очереди
	{
		locker.lock();
		string result = "";
		if (!q.empty())
		{
			result = q.front();
			q.pop();
		}
		locker.unlock();
		return result;
	}

	void find_references(const string& file_name) // извлечение ссылок из файла и добавление их в очередь
	{
		if (visited.find(file_name) != visited.end())
			return;
		string t;
		vector<string> references;
		// копируем файл
		ifstream copying(download + file_name);
		copy_file(file_name);
		getline(copying, t);
		copying.close();
		// создаём вектора ссылок
		for (size_t i = 0; i < t.rfind(reference_end); )
		{
			// местоположение начала и конца ссылки
			size_t c_ref_start = t.find(reference_start, i) + reference_start.length(), c_ref_end = t.find(reference_end, i);
			string c_ref = t.substr(c_ref_start, c_ref_end - c_ref_start);
			references.push_back(c_ref);
			i = c_ref_end + 1;
		}
		// добавляем файлы-ссылки в очередь
		for (const auto& i : references)
		{
			locker.lock();
			if (visited.find(i) == visited.end())
				q.push(i);
			locker.unlock();
		}
		// помечаем файл как посещённый
		locker.lock();
		visited.insert(file_name);
		locker.unlock();
	}

	void work()
	{
		while (!q.empty() || workers_number)
		{
			string current = get_file();
			if (!current.empty())
			{
				++workers_number;
				find_references(current);
				--workers_number;
			}
		}
	}

	void start(const string& start_url)
	{
		clock_t start = clock();
		q.push(start_url);
		for (size_t i = 0; i < thread_count; i++)
			threads.push_back(std::thread(&Crawler::work, this));
		for (auto& i : threads)
			i.join();
		clock_t end = clock();
		time = end - start;
	}

	void print_info() // печать статистики
	{
		fout << "Threads: " << thread_count << " | ";
		fout << "visited pages: " << visited.size() << " | ";
		fout << "wasted time: " << time << endl;
	}

	long get_time()
	{
		return time;
	}

	size_t get_visited_count()
	{
		return visited.size();
	}
};

int main()
{
	string input_filename;
	int thread_count;
	fin >> input_filename >> thread_count;
	for (size_t i = 0; i < max_threads; i++)
	{
		Crawler virus(i + 1);
		virus.start(input_filename);
		virus.print_info();
		if (i + 1 == thread_count)
			cout << virus.get_visited_count() << ' ' << virus.get_time();
	}

	fin.close();
	fout.close();
	return 0;
}
