#include "funcional_tests.h"


int main() {
    testP1_cadastrarInsumo();
    testP2_cadastrarFornecedor();
    testP3_registrarEntradaDeCarga();
    testP4_abrirLotes();
    testP5_lotesPrioritarios();
    testA1_cadastrarInsumoDuplicado();
    testA2_cadastrarFornecedorCNPJInvalido();
    testA3_aceitarCargaComPesoDiferente();
    testA4_abrirLotesCargaNaoAceita();
    testA7_loteVencidoNaoAparece();
    return 0;
}