#include <iostream>
#include <cstdlib>

#define M 2
#define MM (2 * M) // Ordem da árvore B (número máximo de chaves = 2*M)

typedef long TipoChave;

typedef struct Registro {
    TipoChave Chave;
    // Outros componentes do registro (payload do Map)
} Registro;

typedef struct Pagina *Apontador;

typedef struct Pagina {
    short n;                // Número de chaves atualmente na página
    Registro r[MM];         // Array de registros
    Apontador p[MM + 1];    // Array de ponteiros para as sub-páginas filhas
} Pagina;

void Insere(Registro Reg, Apontador *Ap);
void Ins(Registro Reg, Apontador Ap, short *Cresceu, Registro *RegRetorno, Apontador *ApRetorno);
void Pesquisa(Registro *x, Apontador Ap);

void Pesquisa(Registro *x, Apontador Ap) {
    long i = 1;
    if (Ap == NULL) {
        std::cout << "Registro nao esta presente na arvore.\n";
        return;
    }
    while (i < Ap->n && x->Chave > Ap->r[i - 1].Chave) {
        i++;
    }
    if (x->Chave == Ap->r[i - 1].Chave) {
        *x = Ap->r[i - 1];
        return;
    }
    if (x->Chave < Ap->r[i - 1].Chave) {
        Pesquisa(x, Ap->p[i - 1]);
    } else {
        Pesquisa(x, Ap->p[i]);
    }
}

void Ins(Registro Reg, Apontador Ap, short *Cresceu, Registro *RegRetorno, Apontador *ApRetorno) {
    long i = 1;
    long j;
    Apontador ApTemp;

    if (Ap == NULL) {
        *Cresceu = 1;
        *RegRetorno = Reg;
        *ApRetorno = NULL;
        return;
    }

    while (i < Ap->n && Reg.Chave > Ap->r[i - 1].Chave) {
        i++;
    }

    if (Reg.Chave == Ap->r[i - 1].Chave) {
        std::cout << "Erro: Registro ja existe na arvore.\n";
        *Cresceu = 0;
        return;
    }

    if (Reg.Chave < Ap->r[i - 1].Chave) {
        i--;
    }

    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

    if (!*Cresceu) return;

    if (Ap->n < MM) { // A página ainda tem espaço
        *Cresceu = 0;
        // Insere o registro de forma ordenada na página
        j = Ap->n;
        while (j > i && RegRetorno->Chave < Ap->r[j - 1].Chave) {
            Ap->r[j] = Ap->r[j - 1];
            Ap->p[j + 1] = Ap->p[j];
            j--;
        }
        Ap->r[j] = *RegRetorno;
        Ap->p[j + 1] = *ApRetorno;
        Ap->n++;
    } else { // Overflow: a página estourou e precisa ser dividida (Split)
        ApTemp = (Apontador)malloc(sizeof(Pagina));
        long meio = M + 1; // Ponto de divisão clássico do Ziviani
        
        ApTemp->n = 0;
        ApTemp->p[0] = NULL;

        // Copia a metade superior para a nova página temporária
        if (i < meio) {
            j = meio - 1;
            *RegRetorno = Ap->r[j - 1];
            ApTemp->p[0] = Ap->p[j];
            
            for (long k = meio; k <= MM; k++) {
                ApTemp->r[ApTemp->n] = Ap->r[k - 1];
                ApTemp->p[ApTemp->n + 1] = Ap->p[k];
                ApTemp->n++;
            }
            Ap->n = j - 1;
            // Insere o novo elemento na sub-página da esquerda
            // (Lógica de inserção idêntica ao ajuste ordenado abaixo)
            long idx = Ap->n;
            while (idx > i && Reg.Chave < Ap->r[idx - 1].Chave) {
                Ap->r[idx] = Ap->r[idx - 1];
                Ap->p[idx + 1] = Ap->p[idx];
                idx--;
            }
            Ap->r[idx] = Reg;
            Ap->p[idx + 1] = *ApRetorno;
            Ap->n++;
        } else {
            j = meio;
            *RegRetorno = Ap->r[j - 1];
            ApTemp->p[0] = Ap->p[j];
            
            for (long k = meio + 1; k <= MM; k++) {
                ApTemp->r[ApTemp->n] = Ap->r[k - 1];
                ApTemp->p[ApTemp->n + 1] = Ap->p[k];
                ApTemp->n++;
            }
            Ap->n = j - 1;
            // Insere o novo elemento na sub-página da direita
            long idx = ApTemp->n;
            while (idx > 0 && Reg.Chave < ApTemp->r[idx - 1].Chave) {
                ApTemp->r[idx] = ApTemp->r[idx - 1];
                ApTemp->p[idx + 1] = ApTemp->p[idx];
                idx--;
            }
            ApTemp->r[idx] = Reg;
            ApTemp->p[idx + 1] = *ApRetorno;
            ApTemp->n++;
        }
        *ApRetorno = ApTemp;
    }
}

void Insere(Registro Reg, Apontador *Ap) {
    short Cresceu;
    Registro RegRetorno;
    Apontador ApRetorno, ApTemp;

    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

    if (Cresceu) { // A árvore cresce na altura pela raiz
        ApTemp = (Apontador)malloc(sizeof(Pagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[0] = *Ap;
        ApTemp->p[1] = ApRetorno;
        *Ap = ApTemp;
    }
}