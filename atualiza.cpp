#include <iostream>
#include <string>
#include <chrono>
#include <thread>

void atualizarValor(int valor) {
  std::cout << "Valor atual: " << valor;

  // Espera um segundo para simular um processo de atualização
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Volta o cursor para o início da linha e imprime espaços em branco para limpar o valor antigo
  std::cout << '\r' << std::string(20, ' ');

  // Volta o cursor para o início da linha e imprime o novo valor
  std::cout << '\r' << "Novo valor: " << valor;

  // Espera mais um segundo para simular a conclusão do processo de atualização
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    std::cout << "Hello World";
  int valor = 0;
  std::cout << "Valor atual: " << valor;
  while (true) {
    atualizarValor(valor);
    valor++;
  }

  return 0;
}
