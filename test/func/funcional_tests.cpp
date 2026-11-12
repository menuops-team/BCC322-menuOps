#include <cassert>
#include <string>
#include <vector>
#include <stdexcept>
#include "Burger.h"
#include "Resource.h"
#include "Supplier.h"
#include "Shipment.h"
#include "Batch.h"
#include "BatchStatus.h"
#include "Date.h"
#include "ResourceBatchService.h"

using namespace std;

// ---------------------------------------------------------------
// P1: Cadastrar insumo (UC1)
// ---------------------------------------------------------------
void testP1_cadastrarInsumo() {
    Burger burger = Burger();
    ResourceBatchService service (burger);

    // DADO QUE blend e pao ja estao cadastrados
    int blendId = service.createResource("Blend");
    burger.getShelfLifeTable().setShelfLife(blendId, 365);

    int paoId = service.createResource("Pao");
    burger.getShelfLifeTable().setShelfLife(paoId, 1);

    // QUANDO o estoquista cadastrar o tomate
    int tomateId = service.createResource("Tomate");
    burger.getShelfLifeTable().setShelfLife(tomateId, 7);

    // ENTAO os insumos serao blend, pao e tomate
    const vector<Resource*>& insumos = burger.getResources();
    assert(insumos.size() == 3);
    assert(insumos[0]->getName() == "Blend");
    assert(insumos[1]->getName() == "Pao");
    assert(insumos[2]->getName() == "Tomate");
    assert(burger.searchResource(tomateId) == insumos[2]);
    assert(burger.getShelfLifeTable().getShelfLife(tomateId) == 7);

}

// ---------------------------------------------------------------
// P2: Cadastrar fornecedor (UC2)
// ---------------------------------------------------------------
void testP2_cadastrarFornecedor() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE Frigorifico X e Padaria Y ja estao cadastrados
    service.createSupplier("Frigorifico X", "11.222.333/0001-81", "contato", "FOB");
    service.createSupplier("Padaria Y", "11.444.777/0001-61", "contato", "CIF");

    // QUANDO o gestor cadastrar Hortifruti Z
    int hortifrutiId = service.createSupplier("Hortifruti Z", "12.345.678/0001-95", "(31) 3555-0100", "FOB");

    // ENTAO os fornecedores serao X, Y e Z, e Z ficara ativo
    const vector<Supplier*>& fornecedores = burger.getSuppliers();
    assert(fornecedores.size() == 3);
    assert(fornecedores[0]->getName() == "Frigorifico X");
    assert(fornecedores[1]->getName() == "Padaria Y");
    assert(fornecedores[2]->getName() == "Hortifruti Z");

    Supplier* hortifruti = burger.searchSupplier(hortifrutiId);
    assert(hortifruti != nullptr);
    assert(hortifruti->isActive());
    assert(hortifruti->getCnpj() == "12.345.678/0001-95");
    assert(hortifruti->getContact() == "(31) 3555-0100");
    assert(hortifruti->getDeliveryTerms() == "FOB");

}

// ---------------------------------------------------------------
// P3: Registrar entrada de carga (UC3)
// ---------------------------------------------------------------
void testP3_registrarEntradaDeCarga() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE o Frigorifico X esta cadastrado e ha 20 kg de blend na doca
    int fornecedorId = service.createSupplier("Frigorifico X", "11.222.333/0001-81", "contato", "FOB");
    int blendId = service.createResource("Blend");

    // QUANDO o estoquista registrar a carga conferida com a NF
    Shipment carga(0, fornecedorId, Date(28, 10, 2026));
    carga.addResource(blendId, 20.0);
    carga.setAccepted(true);
    int cargaId = service.createShipment(carga);

    // ENTAO a carga fica Aceita e vinculada ao Frigorifico X
    Shipment* registrada = burger.searchShipment(cargaId);
    assert(registrada != nullptr);
    assert(registrada->isAccepted());
    assert(registrada->getSupplierId() == fornecedorId);
    assert(registrada->getReceiptDate() == Date(28, 10, 2026));

    const std::map<int, double>& recursos = registrada->getResources();
    assert(recursos.size() == 1);
    assert(recursos.count(blendId) == 1);
    assert(recursos.at(blendId) == 20.0);

    assert(burger.getShipments().size() == 1);
}

// ---------------------------------------------------------------
// P4: Abrir lotes (UC4)
// ---------------------------------------------------------------
void testP4_abrirLotes() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE existe uma carga aceita de 20 kg de blend (validade 7 dias)
    int fornecedorId = service.createSupplier("Frigorifico X", "11.222.333/0001-81", "contato", "FOB");
    int blendId = service.createResource("Blend");
    burger.getShelfLifeTable().setShelfLife(blendId, 7);

    Shipment carga(0, fornecedorId, Date(28, 10, 2026));
    carga.addResource(blendId, 20.0);
    carga.setAccepted(true);
    int cargaId = service.createShipment(carga);

    // ...e sem lotes registrados
    assert(burger.getInventory().isEmpty());

    // QUANDO o estoquista abrir os lotes
    vector<int> ids = service.openBatches(cargaId);

    // ENTAO sera criado um lote de 20 kg, vencendo em 04/11/2026, AVAILABLE
    assert(ids.size() == 1);
    assert(burger.getInventory().getBatchCount() == 1);

    const Batch& lote = burger.getInventory().getBatches()[0];
    assert(lote.getId() == ids[0]);
    assert(lote.getResourceId() == blendId);
    assert(lote.getShipmentId() == cargaId);
    assert(lote.getQuantity() == 20.0);
    assert(lote.getExpirationDate() == Date(4, 11, 2026));
    assert(lote.getStatus() == BatchStatus::AVAILABLE);

}

// ---------------------------------------------------------------
// P5: Indicar lotes para consumo prioritario (UC5)
// ---------------------------------------------------------------
void testP5_lotesPrioritarios() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE existem dois lotes de blend (20 kg vence 04/11, 10 kg vence 07/11)
    int fornecedorId = service.createSupplier("Frigorifico X", "11.222.333/0001-81", "contato", "FOB");
    int blendId = service.createResource("Blend");
    burger.getShelfLifeTable().setShelfLife(blendId, 7);

    Shipment carga1(0, fornecedorId, Date(28, 10, 2026));
    carga1.addResource(blendId, 20.0);
    carga1.setAccepted(true);
    int carga1Id = service.createShipment(carga1);

    Shipment carga2(0, fornecedorId, Date(31, 10, 2026));
    carga2.addResource(blendId, 10.0);
    carga2.setAccepted(true);
    int carga2Id = service.createShipment(carga2);

    // Abertura na ordem inversa, para o teste realmente exigir a ordenacao
    int lote2Id = service.openBatches(carga2Id)[0];
    int lote1Id = service.openBatches(carga1Id)[0];

    // QUANDO o chef consultar os lotes em 02/11/2026
    std::vector<Batch> prioritarios = service.getPriorityBatches();

    // ENTAO [lote 1, lote 2], lote 1 NEAR_EXPIRATION e lote 2 AVAILABLE
    assert(prioritarios.size() == 2);

    assert(prioritarios[0].getId() == lote1Id);
    assert(prioritarios[0].getQuantity() == 20.0);
    assert(prioritarios[0].getExpirationDate() == Date(4, 11, 2026));
    assert(prioritarios[0].getStatus() == BatchStatus::AVAILABLE);

    assert(prioritarios[1].getId() == lote2Id);
    assert(prioritarios[1].getQuantity() == 10.0);
    assert(prioritarios[1].getExpirationDate() == Date(7, 11, 2026));
    assert(prioritarios[1].getStatus() == BatchStatus::AVAILABLE);

    // a consulta nao altera o dominio
    assert(burger.getInventory().getBatchCount() == 2);

}

// ---------------------------------------------------------------
// A1: Cadastrar insumo duplicado (UC1)
// ---------------------------------------------------------------
void testA1_cadastrarInsumoDuplicado() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE blend e pao ja estao cadastrados
    int blendId = service.createResource("Blend");
    burger.getShelfLifeTable().setShelfLife(blendId, 7);
    int paoId = service.createResource("Pao");
    burger.getShelfLifeTable().setShelfLife(paoId, 1);

    // QUANDO o estoquista tentar cadastrar outro insumo chamado blend
    bool lancou = false;
    try {
        service.createResource("Blend");
    } catch (const std::runtime_error& e) {
        lancou = (std::string(e.what()) == "Resource already exists.");
    }

    // ENTAO o cadastro e recusado e continuam blend e pao
    assert(lancou);
    const vector<Resource*>& insumos = burger.getResources();
    assert(insumos.size() == 2);
    assert(insumos[0]->getName() == "Blend");
    assert(insumos[1]->getName() == "Pao");
}

// ---------------------------------------------------------------
// A2: Cadastrar fornecedor com CNPJ invalido (UC2)
// ---------------------------------------------------------------
void testA2_cadastrarFornecedorCNPJInvalido() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE Frigorifico X e Padaria Y ja estao cadastrados
    service.createSupplier("Frigorifico X", "11.222.333/0001-81", "contato", "FOB");
    service.createSupplier("Padaria Y", "11.444.777/0001-61", "contato", "CIF");

    // QUANDO o gestor tentar cadastrar Hortifruti Z com CNPJ invalido
    bool lancou = false;
    try {
        service.createSupplier("Hortifruti Z", "", "(31) 3555-0100", "FOB");
    } catch (const std::runtime_error& e) {
        lancou = (std::string(e.what()) == "Invalid supplier data.");
    }

    

    // ENTAO o cadastro e recusado e continuam X e Y
    assert(lancou);
    const vector<Supplier*>& fornecedores = burger.getSuppliers();
    assert(fornecedores.size() == 2);
    assert(fornecedores[0]->getName() == "Frigorifico X");
    assert(fornecedores[1]->getName() == "Padaria Y");
}

// ---------------------------------------------------------------
// A3: Carga com divergencia de peso nao e aceita (UC3)
// ---------------------------------------------------------------
void testA3_aceitarCargaComPesoDiferente() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE ha na doca uma carga de 20 kg de blend do Frigorifico X
    int fornecedorId = service.createSupplier("Frigorifico X", "11.222.333/0001-81", "contato", "FOB");
    int blendId = service.createResource("Blend");
    burger.getShelfLifeTable().setShelfLife(blendId, 7);


    Shipment carga(0, fornecedorId, Date(28, 10, 2026));
    carga.addResource(blendId, 20.0);

    int cargaId = service.createShipment(carga);

    // QUANDO o estoquista conferir e a pesagem mostrar 18 kg
    bool aceita = service.acceptShipment(cargaId, {18.0});

    // ENTAO a carga nao e aceita e nao e possivel abrir lotes dela
    assert(!aceita);
    assert(!burger.searchShipment(cargaId)->isAccepted());

    bool lancou = false;
    try {
        service.openBatches(cargaId);
    } catch (const std::runtime_error& e) {
        lancou = (std::string(e.what()) == "Shipment has not been accepted.");
    }
    assert(lancou);
    assert(burger.getInventory().isEmpty());
}

// ---------------------------------------------------------------
// A4: Abrir lotes de carga nao aceita (UC4)
// ---------------------------------------------------------------
void testA4_abrirLotesCargaNaoAceita() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE existe uma carga de 20 kg de blend que nao foi aceita
    int fornecedorId = service.createSupplier("Frigorifico X", "11.222.333/0001-81", "contato", "FOB");
    int blendId = service.createResource("Blend");
    burger.getShelfLifeTable().setShelfLife(blendId, 7);

    Shipment carga(0, fornecedorId, Date(28, 10, 2026));
    carga.addResource(blendId, 20.0);

    int cargaId = service.createShipment(carga);   // sem acceptShipment

    // QUANDO o estoquista tentar abrir os lotes
    bool lancou = false;
    try {
        service.openBatches(cargaId);
    } catch (const std::runtime_error& e) {
        lancou = (std::string(e.what()) == "Shipment has not been accepted.");
    }

    // ENTAO a excecao e lancada e nenhum lote e criado
    assert(lancou);
    assert(burger.getInventory().isEmpty());
}

// ---------------------------------------------------------------
// A7: Lote vencido nao aparece na lista prioritaria (UC5)
// ---------------------------------------------------------------
void testA7_loteVencidoNaoAparece() {
    Burger burger = Burger();
    ResourceBatchService service(burger);

    // DADO QUE o inventario tem blend (vence 04/11) e pao (venceu 01/11)
    int fornecedorId = service.createSupplier("Frigorifico X", "11.222.333/0001-81", "contato", "FOB");
    int blendId = service.createResource("Blend");
    burger.getShelfLifeTable().setShelfLife(blendId, 7);
    int paoId = service.createResource("Pao");
    burger.getShelfLifeTable().setShelfLife(paoId, 1);


    Shipment cargaBlend(0, fornecedorId, Date(28, 10, 2026));
    cargaBlend.addResource(blendId, 20.0);
    int cargaBlendId = service.createShipment(cargaBlend);

    service.acceptShipment(cargaBlendId, {20.0});

    Shipment cargaPao(0, fornecedorId, Date(31, 10, 2026));
    cargaPao.addResource(paoId, 10.0);
    int cargaPaoId = service.createShipment(cargaPao);

    service.acceptShipment(cargaPaoId, {10.0});

    int lote1Id = service.openBatches(cargaBlendId)[0];
    int lote3Id = service.openBatches(cargaPaoId)[0];

    // QUANDO o chef consultar os lotes em 02/11/2026
    service.reevaluateBatchStatus(Date(2, 11, 2026));
    vector<Batch> prioritarios = service.getPriorityBatches();

    // ENTAO o lote vencido nao aparece e a lista e [lote 1]
    assert(prioritarios.size() == 1);
    assert(prioritarios[0].getId() == lote1Id);
    assert(prioritarios[0].getId() != lote3Id);

    // o lote vencido continua no inventario, marcado como EXPIRED
    assert(burger.getInventory().getBatchCount() == 2);
}