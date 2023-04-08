#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

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
	std::vector<std::vector<char>> maze;
	pos_t input_maze;
	pos_t current_position;
	matrix_size size;
};

class OpenFileError : public std::exception {
	public:
		const char* what() const throw() {
			return "Erro na leitura do arquivo";
		}
};
class FinishMaze : public std::exception {
	public:
		const char* what() const throw() {
			return "Labirinto finalizado!";
		}
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;


void print_maze(std::vector<std::vector<char>>& matrix)
{
	std::cout << "\n====================================================================================\n";
	for (const auto& linha:matrix)
	{
		for (const auto& caractere : linha)
		{
			std::cout << caractere << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n====================================================================================\n";
}

maze_info load_maze(const char* file_name)
{
	pos_t initial_pos;
	// Abre o arquivo para leitura 
	matrix_size matrix;
	std::ifstream file(file_name);

	// verifica se o arquivo foi aberto com sucesso
	if (!file.is_open()){
		throw OpenFileError();
	}

	file >> matrix.l >> matrix.c;	
	std::cout << matrix.l << " " << matrix.c << std::endl;

	// carrega os valores do arquivo em memória na matriz "maze"
	std::vector<std::vector<char>> maze(matrix.l, std::vector<char>(matrix.c));	
	for (int i = 0; i < matrix.l; ++i)
	{
		for (int j = 0; j < matrix.c; ++j)
		{
			file >> maze[i][j];
			char entrada = 'i';
			char c = maze[i][j];
			if (c == entrada){
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}

	maze_info info;
	info.maze = maze;
	info.size = matrix;
	info.input_maze = initial_pos;
	info.current_position = initial_pos;
	valid_positions.push(info.current_position);
	return info;
}

/* Adiciona o simbolo '.' no labirinto*/
void update_maze(maze_info& info){
	char symbol = '.';
	info.maze[info.current_position.i][info.current_position.j] = symbol;
}

/* Verifica as posições vizinhas à posição atual em busca de um caminho válido*/
void update_valid_position(maze_info info){
	char valid_symbol = 'x';
	if(info.current_position.j < info.size.c - 1  && info.maze[info.current_position.i][info.current_position.j+1] == valid_symbol){
		pos_t next;
		next.i = info.current_position.i;
		next.j = info.current_position.j + 1;
		valid_positions.push(next);
	}
	if(info.current_position.j > 0 && info.maze[info.current_position.i][info.current_position.j-1] == valid_symbol){
		pos_t next;
		next.i = info.current_position.i;
		next.j = info.current_position.j - 1;
		valid_positions.push(next);
	}
	if(info.current_position.i < info.size.l - 1 && info.maze[info.current_position.i+1][info.current_position.j] == valid_symbol){
		pos_t next;
		next.i = info.current_position.i+1;
		next.j = info.current_position.j;
		valid_positions.push(next);
	}
	if(info.current_position.i > 0 && info.maze[info.current_position.i-1][info.current_position.j] == valid_symbol){
		pos_t next;
		next.i = info.current_position.i-1;
		next.j = info.current_position.j;
		valid_positions.push(next);
	}
}

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
void walk(maze_info info)
{
	info.current_position = valid_positions.top();
	valid_positions.pop();
	update_maze(info=info);
	print_maze(info.maze);
	update_valid_position(info=info);
	if (valid_positions.empty()){
		throw FinishMaze();
	}
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		walk(info=info);
	}
}

int main(int argc, char* argv[])
{
	// carregar o labirinto com o nome do arquivo recebido como argumento (argv[])
	try{
		maze_info info = load_maze("maze2.txt");		
		walk(info);
	} catch (std::exception& e){
		std::cout << e.what() << std::endl;
		return 1;
	}
}
