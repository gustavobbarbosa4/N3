#include <stdio.h>
#include <stdlib.h>

typedef struct Crianca {
    int matricula;
    char nome[50];
    char sexo;
    int idade;
    float peso;
    float altura;
    int status;
} crianca_t;

void lerArquivoBinario(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "rb"); // Abre o arquivo em modo binário para leitura
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    crianca_t crianca;
    printf("Lendo registros do arquivo %s:\n\n", nome_arquivo);

    while (fread(&crianca, sizeof(crianca_t), 1, arquivo) == 1) {
        printf("Matrícula: %d\n", crianca.matricula);
        printf("Nome: %s\n", crianca.nome);
        printf("Sexo: %c\n", crianca.sexo);
        printf("Idade: %d\n", crianca.idade);
        printf("Peso: %.2f kg\n", crianca.peso);
        printf("Altura: %.2f m\n", crianca.altura);
        printf("Status: %d\n", crianca.status);
        printf("-----------------------\n");
    }

    fclose(arquivo); // Fecha o arquivo
}

int main() {
    const char* nome_arquivo = "pessoas.bin";
    lerArquivoBinario(nome_arquivo);
    return 0;
}
