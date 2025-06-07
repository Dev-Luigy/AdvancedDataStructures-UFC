#include "structures/Data/Set.hpp"
#include <iostream>

using namespace std;

int main() {
  Set<int> sets[3];
  int opcao, conjunto, valor, outro;

  do {
    cout << "\n========= MENU =========\n";
    cout << " 0. Sair\n";
    cout << " 1. Inserir elemento\n";
    cout << " 2. Remover elemento\n";
    cout << " 3. Verificar se elemento pertence ao conjunto\n";
    cout << " 4. Imprimir conjunto\n";
    cout << " 5. União de dois conjuntos\n";
    cout << " 6. Interseção de dois conjuntos\n";
    cout << " 7. Diferença entre dois conjuntos\n";
    cout << " 8. Diferença simétrica entre dois conjuntos\n";
    cout << " 9. Verificar igualdade entre dois conjuntos\n";
    cout << "10. Verificar se um conjunto é subconjunto de outro\n";
    cout << "11. Verificar se conjunto está vazio\n";
    cout << "16. Limpar conjunto\n";
    cout << "=========================\n";
    cout << "Escolha uma opção: ";
    cin >> opcao;

    if (opcao == 0)
      break;

    if (opcao >= 1 && opcao <= 16) {
      cout << "Escolha o conjunto (0, 1 ou 2): ";
      cin >> conjunto;

      if (conjunto < 0 || conjunto > 2) {
        cout << "Conjunto inválido!\n";
        continue;
      }
    }

    switch (opcao) {
    case 1: // Inserir
      cout << "Valor a inserir: ";
      cin >> valor;
      sets[conjunto].insert(valor);
      break;

    case 2: // Remover
      cout << "Valor a remover: ";
      cin >> valor;
      sets[conjunto].remove(valor);
      break;

    case 3: // Pertinência
      cout << "Valor a verificar: ";
      cin >> valor;
      if (sets[conjunto].contains(valor))
        cout << "Valor pertence ao conjunto.\n";
      else
        cout << "Valor NÃO pertence ao conjunto.\n";
      break;

    case 4: // Imprimir
      cout << "\nElementos do conjunto " << conjunto << ":";
      cout << "\n";
      sets[conjunto].show();
      break;

    case 5: // União
      cout << "Escolha o outro conjunto para união: ";
      cin >> outro;
      if (outro < 0 || outro > 2) {
        cout << "Conjunto inválido!\n";
        break;
      }
      cout << "\n";
      (sets[conjunto] + sets[outro]).show();
      break;

    case 6: // Interseção
      cout << "Escolha o outro conjunto para interseção: ";
      cin >> outro;
      if (outro < 0 || outro > 2) {
        cout << "Conjunto inválido!\n";
        break;
      }
      cout << "\n";
      (sets[conjunto] & sets[outro]).show();
      break;

    case 7: // Diferença
      cout << "Escolha o outro conjunto para diferença (A - B): ";
      cin >> outro;
      if (outro < 0 || outro > 2) {
        cout << "Conjunto inválido!\n";
        break;
      }
      cout << "\n";
      (sets[conjunto] - sets[outro]).show();
      break;

    case 8: // Diferença Simétrica
      cout << "Escolha o outro conjunto para diferença simétrica: ";
      cin >> outro;
      if (outro < 0 || outro > 2) {
        cout << "Conjunto inválido!\n";
        break;
      }
      cout << "\n";
      sets[conjunto].symmetricDifference(sets[outro]).show();
      break;

    case 9: // Igualdade
      cout << "Escolha o outro conjunto para comparação: ";
      cin >> outro;
      if (outro < 0 || outro > 2) {
        cout << "Conjunto inválido!\n";
        break;
      }
      if (sets[conjunto] == sets[outro])
        cout << "Os conjuntos são IGUAIS.\n";
      else
        cout << "Os conjuntos são DIFERENTES.\n";
      break;

    case 10: // Subconjunto
      cout << "Verificar se o conjunto " << conjunto << " é subconjunto de: ";
      cin >> outro;
      if (outro < 0 || outro > 2) {
        cout << "Conjunto inválido!\n";
        break;
      }
      if (sets[conjunto].isSubset(sets[outro]))
        cout << "É subconjunto.\n";
      else
        cout << "NÃO é subconjunto.\n";
      break;

    case 11: // Está vazio
      if (sets[conjunto].isEmpty())
        cout << "O conjunto está vazio.\n";
      else
        cout << "O conjunto NÃO está vazio.\n";
      break;

    case 16: // Limpar conjunto
      sets[conjunto].clear();
      cout << "Conjunto limpo.\n";
      break;

    default:
      cout << "Opção inválida!\n";
      break;
    }

  } while (true);

  cout << "Programa encerrado.\n";
  return 0;
}
