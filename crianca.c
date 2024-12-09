#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO_BINARIO "pessoas.bin"

typedef struct Crianca {
    int matricula;
    char nome[50];
    char sexo;
    int idade;
    float peso;
    float altura;
    int status;  
    struct Crianca *prox;
} crianca_t;

typedef enum {
    ABAIXO = 0,
    NORMAL = 1,
    SOBREPESO = 2,
    OBESIDADE = 3
} IMCInfantil;

// Fun??o para obter a categoria de IMC infantil
IMCInfantil obter_categoria_imc(float imc) {
    if (imc < 18.5) {
        return ABAIXO;
    } else if (imc >= 18.5 && imc < 24.9) {
        return NORMAL;
    } else if (imc >= 25 && imc < 29.9) {
        return SOBREPESO;
    } else {
        return OBESIDADE;
    }
}

// Fun??o para ler os dados de uma crian?a pela matr?cula
crianca_t* ler_dados_crianca(int matricula) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    crianca_t *crianca = malloc(sizeof(crianca_t));
    while (fread(crianca, sizeof(crianca_t), 1, arquivo) == 1) {
        if (crianca->matricula == matricula) {
            fclose(arquivo);
            return crianca;
        }
    }

    printf("Criança com matricula %d não encontrada.\n", matricula);
    free(crianca);
    fclose(arquivo);
    return NULL;
}

// Fun??o para ler 10 crian?as a partir da ?ltima matr?cula
crianca_t* ler_criancas_10(int ultima_matricula) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    crianca_t *head = NULL, *tail = NULL, *temp;
    crianca_t buffer;
    int count = 0;

    while (fread(&buffer, sizeof(crianca_t), 1, arquivo) == 1) {
        if (buffer.matricula > ultima_matricula && count < 10) {
            temp = malloc(sizeof(crianca_t));
            *temp = buffer;
            temp->prox = NULL;

            if (!head)
                head = temp;
            else
                tail->prox = temp;

            tail = temp;
            count++;
        }
    }

    fclose(arquivo);
    return head;
}

// Fun??o para cadastrar uma nova crian?a
int cadastrar_crianca(crianca_t* nova_crianca) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "ab");  // Abrindo no modo "append"
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    // Calcular o IMC e determinar a categoria
    float imc = nova_crianca->peso / (nova_crianca->altura * nova_crianca->altura);
    nova_crianca->status = obter_categoria_imc(imc);

    fwrite(nova_crianca, sizeof(crianca_t), 1, arquivo);

    fclose(arquivo);
    printf("Criança cadastrada com sucesso!\n");
    return 1;
}

// Fun??o para exibir a lista de crian?as
void exibir_lista(crianca_t* lista) {
    crianca_t* atual = lista;
    while (atual) {
        printf("Matricula: %d\n", atual->matricula);
        printf("Nome: %s\n", atual->nome);
        printf("Sexo: %c\n", atual->sexo);
        printf("Idade: %d\n", atual->idade);
        printf("Peso: %.2f kg\n", atual->peso);
        printf("Altura: %.2f m\n", atual->altura);
        printf("Status: ");
        
        switch (atual->status) {
            case ABAIXO:
                printf("Abaixo do peso\n");
                break;
            case NORMAL:
                printf("Normal\n");
                break;
            case SOBREPESO:
                printf("Sobrepeso\n");
                break;
            case OBESIDADE:
                printf("Obesidade\n");
                break;
        }

        printf("-----------------------\n");
        atual = atual->prox;
    }
}

int main() {
    int opcao, matricula;
    crianca_t nova_crianca;
    crianca_t* lista = NULL;
    crianca_t* crianca = NULL;
    
    setlocale(LC_ALL, "portuguese");

   do {
    printf("\n===== Menu =====\n");
    printf("1. Consultar criança pela matrícula\n");
    printf("2. Listar 10 crianças a partir de uma matrícula\n");
    printf("3. Cadastrar nova criança\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    switch (opcao) {
        case 1:
            // Consulta pela matrícula
            printf("\nDigite a matrícula da criança: ");
            scanf("%d", &matricula);
            while (getchar() != '\n');
            crianca = ler_dados_crianca(matricula);
            if (crianca) {
                printf("Dados da criança:\n");
                printf("Matrícula: %d\n", crianca->matricula);
                printf("Nome: %s\n", crianca->nome);
                printf("Sexo: %c\n", crianca->sexo);
                printf("Idade: %d\n", crianca->idade);
                printf("Peso: %.2f kg\n", crianca->peso);
                printf("Altura: %.2f m\n", crianca->altura);
                printf("Status: ");
                switch (crianca->status) {
                    case ABAIXO:
                        printf("Abaixo do peso\n");
                        break;
                    case NORMAL:
                        printf("Normal\n");
                        break;
                    case SOBREPESO:
                        printf("Sobrepeso\n");
                        break;
                    case OBESIDADE:
                        printf("Obesidade\n");
                        break;
                }
                free(crianca);
            }
            break;
        
        case 2:
            // Listar 10 crianças
            printf("\nDigite a última matrícula lida (0 para começar do início): ");
            scanf("%d", &matricula);
            while (getchar() != '\n');
            lista = ler_criancas_10(matricula);
            if (lista) {
                printf("\nLista de crianças:\n");
                exibir_lista(lista);
                crianca_t* temp;
                while (lista) {
                    temp = lista;
                    lista = lista->prox;
                    free(temp);
                }
            } else {
                printf("Nenhuma criança encontrada.\n");
            }
            break;
        
        case 3:
            // Cadastrar nova criança
            printf("\nDigite os dados da nova criança:\n");
            printf("Matrícula: ");
            scanf("%d", &nova_crianca.matricula);
            while (getchar() != '\n');
            printf("Nome: ");
            fgets(nova_crianca.nome, sizeof(nova_crianca.nome), stdin);
            nova_crianca.nome[strcspn(nova_crianca.nome, "\n")] = '\0';
            printf("Sexo (M/F): ");
            scanf(" %c", &nova_crianca.sexo);
            while (getchar() != '\n');
            printf("Idade: ");
            scanf("%d", &nova_crianca.idade);
            while (getchar() != '\n');
            printf("Peso (kg): ");
            scanf("%f", &nova_crianca.peso);
            while (getchar() != '\n');
            printf("Altura (m): ");
            scanf("%f", &nova_crianca.altura);
            while (getchar() != '\n');
            nova_crianca.prox = NULL;
            if (cadastrar_crianca(&nova_crianca)) {
                printf("Criança cadastrada com sucesso!\n");
            }
            break;

        case 4:
            printf("\nSaindo...\n");
            break;

        default:
            printf("\nOpção inválida! Tente novamente.\n");
            break;
    }
} while (opcao != 4);

    return 0;
}