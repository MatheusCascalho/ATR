#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include <mutex>

using namespace std;

mutex m;

// Posição a ser explorada
struct pos_t
{
	int i;
	int j;
};

struct matrix_size
{
	int l;
	int c;
};

struct maze_info
{
	pos_t input_maze;
	pos_t current_position;
	matrix_size size;
};

class OpenFileError : public exception {
	public:
		const char* what() const throw() {
			return "Erro na leitura do arquivo";
		}
};

class FinishMaze : public exception {
	public:
		const char* what() const throw() {
			return "Labirinto finalizado!";
		}
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
stack<pos_t> position_buffer;

vector<vector<char>> global_maze;

void print_maze()
{
	cout << "\n====================================================================================\n";
	for (const auto& linha:global_maze)
	{
		for (const auto& caractere : linha)
		{
			cout << caractere << " ";
		}
		cout << "\n";
	}
	cout << "\n====================================================================================\n";
}

maze_info load_maze(const char* file_name)
{
	pos_t initial_pos;
	// Abre o arquivo para leitura 
	matrix_size matrix;
	ifstream file(file_name);

	// verifica se o arquivo foi aberto com sucesso
	if (!file.is_open()){
		throw OpenFileError();
	}

	file >> matrix.l >> matrix.c;	
	cout << matrix.l << " " << matrix.c << endl;

	string line;
	int i = 0;
	while(getline(file, line)){
        if(i == 0){
			i++;
			continue;
		}
		else {
			vector<char> chars(line.begin(), line.end());
			global_maze.push_back(chars);
		}
		
	}
	// carrega os valores do arquivo em memória na matriz "maze"
	for (int i = 0; i < matrix.l; ++i)
	{
		for (int j = 0; j < matrix.c; ++j)
		{
			char entrada = 'i';
			char c = global_maze[i][j];
			if (c == entrada){
				initial_pos.i = i;
				initial_pos.j = j;
				break;
			}
		}
	}

	maze_info info;
	info.size = matrix;
	info.input_maze = initial_pos;
	info.current_position = initial_pos;
	return info;
}



// thread create_thread(pos_t next, maze_info info){
// 		maze_info next_info;
		
// 		next_info.size = info.size;
// 		next_info.input_maze = info.input_maze;
// 		next_info.current_position = next;

// 		thread t(walk, next_info);
// 		return t;
// }

/* Verifica as posições vizinhas à posição atual em busca de um caminho válido*/
vector<pos_t> get_valid_position(maze_info info){
	char valid_symbol = 'x';
	vector<pos_t> valid_positions;
	// vector<thread> threads;
	
	if(info.current_position.j < info.size.c - 1  && global_maze[info.current_position.i][info.current_position.j+1] == valid_symbol){
		pos_t next;

		next.i = info.current_position.i;
		next.j = info.current_position.j + 1;

		// thread t(create_thread(next, info));
  		// threads.push_back(t);

		valid_positions.push_back(next);
	}
	if(info.current_position.j > 0 && global_maze[info.current_position.i][info.current_position.j-1] == valid_symbol){
		pos_t next;
		next.i = info.current_position.i;
		next.j = info.current_position.j - 1;
		
		// thread t(create_thread(next, info));
  		// threads.push_back(t);

		valid_positions.push_back(next);
	}
	if(info.current_position.i < info.size.l - 1 && global_maze[info.current_position.i+1][info.current_position.j] == valid_symbol){
		pos_t next;
		next.i = info.current_position.i+1;
		next.j = info.current_position.j;
		
		// thread t(create_thread(next, info));
  		// threads.push_back(t);

		valid_positions.push_back(next);
	}
	if(info.current_position.i > 0 && global_maze[info.current_position.i-1][info.current_position.j] == valid_symbol){
		pos_t next;
		next.i = info.current_position.i-1;
		next.j = info.current_position.j;
		
		// thread t(create_thread(next, info));
  		// threads.push_back(t);

		valid_positions.push_back(next);
	}
	return valid_positions;
}

/* Adiciona o simbolo '.' no labirinto*/
void walk(maze_info& info){
	char symbol = '.';
	global_maze[info.current_position.i][info.current_position.j] = symbol;
	
}

vector<thread> threads;

/* Função Recursiva para caminhar sobre o labirinto
	caminha:
		pega a posição atual
		atualiza labirinto com posição atual
		imprime o labirinto
		atualiza a lista de caminhos possíveis
		

		SE não existe posições válidas:
			para o programa;
		SE existe posição válida:
			caminha()
	
*/
// void walk(maze_info info)
// {
// 	update_maze(info=info);
// 	print_maze();
// 	maze_info next_info;
// 	pos_t next = valid_positions.top();
// 	next_info.size = info.size;
// 	next_info.input_maze = info.input_maze;
// 	next_info.current_position = next;
// }

// void search(maze_info& info){
// 	info.current_position = valid_positions.top();
// 	valid_positions.pop();
// 	walk(info);
// }

thread new_search(maze_info info){
	// m.lock();
	walk(info);
	vector<pos_t> valid_positions;
	valid_positions = get_valid_position(info);
	if(!valid_positions.empty()){
		info.current_position = valid_positions.back();
		valid_positions.pop_back();
		walk(info);
		print_maze();
		valid_positions = get_valid_position(info);
		for(auto& position: valid_positions){
			position_buffer.push(position);
		}

	}
	// m.unlock();
}

int main(int argc, char* argv[])
{
	// carregar o labirinto com o nome do arquivo recebido como argumento (argv[])
	try{
		maze_info info = load_maze("maze2.txt");			
		// if (valid_positions.empty()){
		// 	throw FinishMaze();
		// }
		// else {
			// while(!valid_positions.empty()){
		print_maze();
		vector<pos_t> valid_positions;
		valid_positions = get_valid_position(info);
		while(!valid_positions.empty()){
			info.current_position = valid_positions.back();
			valid_positions.pop_back();
			if(valid_positions.size()>0){
				for (int i = 0; i < valid_positions.size(); i++){
					maze_info current_info(info);
					current_info.current_position = valid_positions.back();
					valid_positions.pop_back();
					thread t(new_search, ref(current_info));
					threads.push_back(move(t));
					this_thread::sleep_for(chrono::milliseconds(50));
				}
			}
			if(position_buffer.size()>0){
				for (int i = 0; i < position_buffer.size(); i++){
					maze_info current_info(info);
					current_info.current_position = position_buffer.top();
					position_buffer.pop();
					thread t(new_search, ref(current_info));
					threads.push_back(move(t));
					this_thread::sleep_for(chrono::milliseconds(50));
				}
			}
			walk(info);
			print_maze();
			valid_positions = get_valid_position(info);				
			this_thread::sleep_for(chrono::milliseconds(50));
		}
		
		for(auto& thread : threads){
			thread.join();
		}
	} catch (exception& e){
		cout << e.what() << endl;
		return 1;
	}
}
