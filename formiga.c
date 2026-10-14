/**
 * @file formiga.c
 * @author Tarsila Paiva Pimentel
 * @brief Implementação da movimentação e da representação visual da formiga.
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 * @details
 * Este arquivo implementa a inicialização, atualização e reinicialização da
 * formiga sobre a superfície de um parabolóide hiperbólico. Também fornece
 * funções para consultar sua posição, direção, gradiente e tangente, além de
 * desenhar seu corpo, seu caminho percorrido e os vetores auxiliares.
 */
#include <math.h>
#include "formiga.h"
#include "pringles.h"

/** Valor de pi usado nos cálculos angulares da animação. */
#define FORMIGA_PI 3.14159265358979323846f

/** Semieixo da região permitida da formiga na direção X. */
static const float semieixoX = 1.5f;
/** Semieixo da região permitida da formiga na direção Z. */
static const float semieixoZ = 1.0f;

/**
 * @brief Limita um valor a um intervalo fechado.
 *
 * @param valor Valor que será limitado.
 * @param minimo Limite inferior do intervalo.
 * @param maximo Limite superior do intervalo.
 * @return float Valor dentro do intervalo informado.
 */
static float limitar(float valor, float minimo, float maximo){
    if (valor < minimo) return minimo;
    if (valor > maximo) return maximo;
    return valor;
}

/**
 * @brief Normaliza um vetor tridimensional.
 *
 * Retorna o vetor unitário na direção informada. Para vetores com comprimento
 * praticamente nulo, retorna o vetor nulo.
 *
 * @param vetor Vetor que será normalizado.
 * @return Vector3 Vetor unitário ou vetor nulo.
 */
static Vector3 normalizar(Vector3 vetor){
    float comprimento = sqrtf(
        vetor.x * vetor.x + vetor.y * vetor.y + vetor.z * vetor.z
    );
    if (comprimento < 0.000001f) return (Vector3){0.0f, 0.0f, 0.0f};
    return (Vector3){vetor.x / comprimento, vetor.y / comprimento, vetor.z / comprimento};
}

/**
 * @brief Avança o gerador pseudoaleatório da formiga.
 *
 * Atualiza o estado interno com operações XOR e deslocamentos de bits.
 *
 * @param formiga Formiga cujo estado aleatório será atualizado.
 * @return uint32_t Próximo valor pseudoaleatório.
 */
static uint32_t proximoAleatorio(Formiga *formiga){
    uint32_t valor = formiga->estadoAleatorio;
    valor ^= valor << 13;
    valor ^= valor >> 17;
    valor ^= valor << 5;
    formiga->estadoAleatorio = valor;
    return valor;
}

/**
 * @brief Gera um valor pseudoaleatório no intervalo de zero a um.
 *
 * @param formiga Formiga usada como estado do gerador.
 * @return float Valor pseudoaleatório normalizado.
 */
static float aleatorioUnitario(Formiga *formiga){
    return (float)(proximoAleatorio(formiga) >> 8) / 16777215.0f;
}

/**
 * @brief Calcula um ponto da superfície para coordenadas X e Z.
 *
 * Se o ponto ultrapassar a região elíptica de movimento, suas coordenadas são
 * projetadas para perto da borda. O afastamento permite posicionar o ponto
 * acima da superfície.
 *
 * @param x Coordenada na direção do eixo X.
 * @param z Coordenada na direção do eixo Z.
 * @param afastamento Distância vertical adicionada à superfície.
 * @return Vector3 Ponto tridimensional correspondente.
 */
static Vector3 pontoNaSuperficie(float x, float z, float afastamento){
    float raioElipse = (x * x) / (semieixoX * semieixoX) +
        (z * z) / (semieixoZ * semieixoZ);
    if (raioElipse > 0.999f){
        float escala = 0.999f / sqrtf(raioElipse);
        x *= escala;
        z *= escala;
    }
    return (Vector3){x, alturaPringles(x, z) + afastamento, z};
}

/**
 * @brief Converte uma direção horizontal em uma direção tangente à superfície.
 *
 * Calcula a componente vertical a partir das derivadas da superfície e
 * normaliza o vetor resultante.
 *
 * @param x Coordenada atual na direção do eixo X.
 * @param z Coordenada atual na direção do eixo Z.
 * @param ux Componente horizontal da direção no eixo X.
 * @param uz Componente horizontal da direção no eixo Z.
 * @return Vector3 Vetor unitário tangente à superfície.
 */
static Vector3 tangenteParaDirecao(float x, float z, float ux, float uz){
    float derivadaX = 0.4f * x;
    float derivadaZ = -0.4f * z;
    float componenteY = derivadaX * ux + derivadaZ * uz;
    return normalizar((Vector3){ux, componenteY, uz});
}

/**
 * @brief Registra a posição atual no histórico circular da formiga.
 *
 * A posição é armazenada ligeiramente acima da superfície. Quando o histórico
 * atinge sua capacidade máxima, as posições mais antigas são sobrescritas.
 *
 * @param formiga Formiga cuja posição será registrada.
 */
static void registrarPosicao(Formiga *formiga){
    Vector3 ponto = pontoNaSuperficie(formiga->x, formiga->z, 0.012f);
    formiga->historico[formiga->proximoHistorico] = ponto;
    formiga->proximoHistorico =
        (formiga->proximoHistorico + 1) % FORMIGA_HISTORICO_MAX;
    if (formiga->quantidadeHistorico < FORMIGA_HISTORICO_MAX){
        formiga->quantidadeHistorico++;
    }
}

/**
 * @brief Inicializa a formiga e prepara seu estado de movimento.
 *
 * Define posição inicial, velocidade, orientação, estado pseudoaleatório e
 * registra a posição inicial no histórico. Se a semente for zero, usa uma
 * semente padrão.
 *
 * @param formiga Formiga que será inicializada; não faz nada se for NULL.
 * @param semente Semente usada pelo gerador pseudoaleatório.
 */
void inicializarFormiga(Formiga *formiga, uint32_t semente){
    if (formiga == NULL) return;

    *formiga = (Formiga){0};
    formiga->semente = semente == 0 ? 0x00C0FFEEu : semente;
    formiga->velocidade = 0.28f;
    formiga->angulo = 0.72f;
    formiga->estadoAleatorio = formiga->semente;
    formiga->tempoAteVirar = 0.18f + aleatorioUnitario(formiga) * 0.3f;
    registrarPosicao(formiga);
}

/**
 * @brief Atualiza o movimento e o histórico da formiga.
 *
 * Simula uma caminhada aleatória sobre a superfície, divide o intervalo em
 * passos curtos e reflete a direção quando a formiga alcança a borda elíptica.
 * Não altera a formiga se ela estiver pausada ou se o intervalo não for
 * positivo.
 *
 * @param formiga Formiga que será atualizada.
 * @param deltaTempo Tempo decorrido desde a última atualização, em segundos.
 */
void atualizarFormiga(Formiga *formiga, float deltaTempo){
    if (formiga == NULL || formiga->pausada || deltaTempo <= 0.0f) return;

    // Passos curtos deixam as mudanças aleatórias e a reflexão na borda estáveis.
    int quantidadePassos = (int)ceilf(deltaTempo / 0.01f);
    if (quantidadePassos < 1) quantidadePassos = 1;
    float passoTempo = deltaTempo / quantidadePassos;

    for (int passo = 0; passo < quantidadePassos; passo++){
        formiga->tempo += passoTempo;
        formiga->tempoAteVirar -= passoTempo;

        // Caminhada aleatória persistente: pequenas mudanças de rumo em intervalos
        // aleatórios, com algumas viradas maiores ocasionais.
        if (formiga->tempoAteVirar <= 0.0f){
            float virada = (aleatorioUnitario(formiga) - 0.5f) * 1.5f;
            if (aleatorioUnitario(formiga) < 0.12f){
                virada += (aleatorioUnitario(formiga) < 0.5f ? -1.0f : 1.0f) * 1.1f;
            }
            formiga->angulo += virada;
            formiga->tempoAteVirar = 0.16f + aleatorioUnitario(formiga) * 0.42f;
        }

        float ux = cosf(formiga->angulo);
        float uz = sinf(formiga->angulo);
        Vector3 tangente = tangenteParaDirecao(formiga->x, formiga->z, ux, uz);
        float deslocamento = formiga->velocidade * passoTempo;
        float novoX = formiga->x + tangente.x * deslocamento;
        float novoZ = formiga->z + tangente.z * deslocamento;
        float dentroElipse = (novoX * novoX) / (semieixoX * semieixoX) +
            (novoZ * novoZ) / (semieixoZ * semieixoZ);

        if (dentroElipse > 1.0f){
            // Projeta o ponto na borda e reflete a direção pela normal da elipse.
            float escala = 0.999f / sqrtf(dentroElipse);
            novoX *= escala;
            novoZ *= escala;
            float normalX = novoX / (semieixoX * semieixoX);
            float normalZ = novoZ / (semieixoZ * semieixoZ);
            float produto = ux * normalX + uz * normalZ;
            float normaNormal = normalX * normalX + normalZ * normalZ;
            if (normaNormal > 0.000001f){
                ux -= 2.0f * produto / normaNormal * normalX;
                uz -= 2.0f * produto / normaNormal * normalZ;
                formiga->angulo = atan2f(uz, ux);
            }
        }

        formiga->x = limitar(novoX, -semieixoX, semieixoX);
        formiga->z = limitar(novoZ, -semieixoZ, semieixoZ);

        // Proteção adicional para manter a posição dentro do domínio elíptico.
        float raioElipse = (formiga->x * formiga->x) / (semieixoX * semieixoX) +
            (formiga->z * formiga->z) / (semieixoZ * semieixoZ);
        if (raioElipse > 1.0f){
            float escala = 0.999f / sqrtf(raioElipse);
            formiga->x *= escala;
            formiga->z *= escala;
        }

        formiga->tempoAteRegistrar -= passoTempo;
        if (formiga->tempoAteRegistrar <= 0.0f){
            registrarPosicao(formiga);
            formiga->tempoAteRegistrar = 0.035f;
        }
    }
}

/**
 * @brief Reinicializa a formiga preservando sua semente.
 *
 * @param formiga Formiga que será reiniciada; não faz nada se for NULL.
 */
void reiniciarFormiga(Formiga *formiga){
    if (formiga == NULL) return;
    uint32_t semente = formiga->semente;
    inicializarFormiga(formiga, semente);
}

/**
 * @brief Obtém a posição tridimensional atual da formiga.
 *
 * @param formiga Formiga consultada.
 * @return Vector3 Posição sobre a superfície ou vetor nulo se formiga for NULL.
 */
Vector3 obterPosicaoFormiga(const Formiga *formiga){
    if (formiga == NULL) return (Vector3){0.0f, 0.0f, 0.0f};
    return pontoNaSuperficie(formiga->x, formiga->z, 0.0f);
}

/**
 * @brief Obtém o gradiente da superfície na posição atual da formiga.
 *
 * @param formiga Formiga consultada.
 * @return Vector3 Gradiente no ponto atual ou vetor nulo se formiga for NULL.
 */
Vector3 obterGradienteFormiga(const Formiga *formiga){
    if (formiga == NULL) return (Vector3){0.0f, 0.0f, 0.0f};
    return (Vector3){0.4f * formiga->x, 0.0f, -0.4f * formiga->z};
}

/**
 * @brief Obtém a direção horizontal atual da formiga.
 *
 * @param formiga Formiga consultada.
 * @return Vector3 Vetor de direção no plano XZ ou vetor nulo se formiga for NULL.
 */
Vector3 obterDirecaoFormiga(const Formiga *formiga){
    if (formiga == NULL) return (Vector3){0.0f, 0.0f, 0.0f};
    return (Vector3){cosf(formiga->angulo), 0.0f, sinf(formiga->angulo)};
}

/**
 * @brief Obtém a direção tangente à superfície seguida pela formiga.
 *
 * @param formiga Formiga consultada.
 * @return Vector3 Vetor tangente unitário ou vetor nulo se formiga for NULL.
 */
Vector3 obterTangenteFormiga(const Formiga *formiga){
    if (formiga == NULL) return (Vector3){0.0f, 0.0f, 0.0f};
    Vector3 direcao = obterDirecaoFormiga(formiga);
    return tangenteParaDirecao(formiga->x, formiga->z, direcao.x, direcao.z);
}

/**
 * @brief Desenha uma seta tridimensional usando segmentos de linha.
 *
 * @param inicio Ponto inicial da seta.
 * @param direcao Direção da seta.
 * @param comprimento Comprimento total da seta.
 * @param cor Cor usada nos segmentos.
 */
static void desenharSeta(Vector3 inicio, Vector3 direcao, float comprimento, Color cor){
    Vector3 unitario = normalizar(direcao);
    float norma = sqrtf(unitario.x * unitario.x + unitario.y * unitario.y + unitario.z * unitario.z);
    if (norma < 0.5f) return;

    Vector3 fim = {
        inicio.x + unitario.x * comprimento,
        inicio.y + unitario.y * comprimento,
        inicio.z + unitario.z * comprimento
    };
    DrawLine3D(inicio, fim, cor);

    Vector3 lateral = normalizar((Vector3){-unitario.z, 0.0f, unitario.x});
    if (fabsf(lateral.x) + fabsf(lateral.z) < 0.001f){
        lateral = (Vector3){1.0f, 0.0f, 0.0f};
    }
    float tamanhoPonta = comprimento * 0.20f;
    for (int lado = -1; lado <= 1; lado += 2){
        Vector3 ponta = {
            fim.x - unitario.x * tamanhoPonta + lateral.x * tamanhoPonta * 0.55f * lado,
            fim.y - unitario.y * tamanhoPonta,
            fim.z - unitario.z * tamanhoPonta + lateral.z * tamanhoPonta * 0.55f * lado
        };
        DrawLine3D(fim, ponta, cor);
    }
}

/**
 * @brief Desenha o caminho armazenado no histórico da formiga.
 *
 * Conecta as posições válidas na ordem em que foram registradas, respeitando
 * a organização circular do histórico.
 *
 * @param formiga Formiga cujo caminho será desenhado.
 */
static void desenharTrilha(const Formiga *formiga){
    if (formiga->quantidadeHistorico < 2) return;
    int primeiro = (formiga->proximoHistorico - formiga->quantidadeHistorico +
        FORMIGA_HISTORICO_MAX) % FORMIGA_HISTORICO_MAX;
    for (int i = 0; i < formiga->quantidadeHistorico - 1; i++){
        int indiceA = (primeiro + i) % FORMIGA_HISTORICO_MAX;
        int indiceB = (indiceA + 1) % FORMIGA_HISTORICO_MAX;
        DrawLine3D(formiga->historico[indiceA], formiga->historico[indiceB],
            (Color){36, 226, 220, 255});
    }
}

/**
 * @brief Desenha o corpo facetado da formiga sobre a superfície.
 *
 * Representa abdômen, tórax, cabeça e seis pernas. O balanço das pernas é
 * animado conforme o tempo acumulado e fica imóvel quando a formiga está
 * pausada.
 *
 * @param formiga Formiga que será desenhada; não faz nada se for NULL.
 */
void desenharCorpoFormiga(const Formiga *formiga){
    if (formiga == NULL) return;

    float x = formiga->x;
    float z = formiga->z;
    Vector3 direcao = obterDirecaoFormiga(formiga);
    float frenteX = direcao.x;
    float frenteZ = direcao.z;
    float ladoX = -frenteZ;
    float ladoZ = frenteX;

    const Color contorno = {17, 20, 38, 255};
    const Color carapaca = {236, 67, 139, 255};
    const Color corpo = {38, 41, 67, 255};
    const Color perna = {22, 26, 45, 255};

    const float escala = 0.648f;
    const float escalaPerna = 0.68f;
    Vector3 abdomen = pontoNaSuperficie(x - frenteX * 0.105f * escala, z - frenteZ * 0.105f * escala, 0.075f * escala);
    Vector3 torax = pontoNaSuperficie(x - frenteX * 0.005f * escala, z - frenteZ * 0.005f * escala, 0.060f * escala);
    Vector3 cabeca = pontoNaSuperficie(x + frenteX * 0.085f * escala, z + frenteZ * 0.085f * escala, 0.054f * escala);

    // Formas facetadas e cores chapadas dão à formiga um aspecto gráfico estilizado.
    DrawSphereEx(abdomen, 0.084f * escala, 4, 6, contorno);
    DrawSphereEx(abdomen, 0.070f * escala, 4, 6, carapaca);
    DrawSphereEx(torax, 0.058f * escala, 4, 6, contorno);
    DrawSphereEx(torax, 0.047f * escala, 4, 6, corpo);
    DrawSphereEx(cabeca, 0.054f * escala, 4, 6, contorno);
    DrawSphereEx(cabeca, 0.044f * escala, 4, 6, corpo);

    for (int par = 0; par < 3; par++){
        const float posicoesPernas[3] = {0.035f, -0.020f, -0.075f};
        for (int lado = -1; lado <= 1; lado += 2){
            float fase = formiga->tempo * 17.0f + par * FORMIGA_PI +
                (lado > 0 ? FORMIGA_PI : 0.0f);
            float balanco = formiga->pausada ? 0.0f :
                0.045f * escala * escalaPerna * sinf(fase);
            float ancoraX = x + frenteX * posicoesPernas[par] * escala +
                ladoX * 0.032f * escala * escalaPerna * lado;
            float ancoraZ = z + frenteZ * posicoesPernas[par] * escala +
                ladoZ * 0.032f * escala * escalaPerna * lado;
            float joelhoX = ancoraX + frenteX * balanco * 0.35f +
                ladoX * 0.085f * escala * escalaPerna * lado;
            float joelhoZ = ancoraZ + frenteZ * balanco * 0.35f +
                ladoZ * 0.085f * escala * escalaPerna * lado;
            float peX = ancoraX + frenteX * balanco +
                ladoX * 0.135f * escala * escalaPerna * lado;
            float peZ = ancoraZ + frenteZ * balanco +
                ladoZ * 0.135f * escala * escalaPerna * lado;

            Vector3 ancora = pontoNaSuperficie(ancoraX, ancoraZ, 0.043f * escala);
            Vector3 joelho = pontoNaSuperficie(joelhoX, joelhoZ, 0.030f * escala);
            Vector3 pe = pontoNaSuperficie(peX, peZ, 0.008f * escala);
            DrawLine3D(ancora, joelho, perna);
            DrawLine3D(joelho, pe, perna);
        }
    }

}

/**
 * @brief Desenha o caminho e os vetores auxiliares da formiga.
 *
 * Os vetores exibidos incluem a tangente de movimento, o gradiente, as
 * derivadas parciais e a normal da superfície.
 *
 * @param formiga Formiga usada como referência; não faz nada se for NULL.
 * @param mostrarRetas Indica se os vetores auxiliares devem ser desenhados.
 * @param mostrarCaminho Indica se o histórico do percurso deve ser desenhado.
 */
void desenharElementosFormiga(const Formiga *formiga, bool mostrarRetas,
    bool mostrarCaminho){
    if (formiga == NULL) return;
    if (mostrarCaminho) desenharTrilha(formiga);
    if (!mostrarRetas) return;

    Vector3 posicao = obterPosicaoFormiga(formiga);
    Vector3 origemTangente = {posicao.x, posicao.y + 0.045f, posicao.z};
    desenharSeta(origemTangente, obterTangenteFormiga(formiga), 0.38f,
        (Color){42, 220, 247, 255});

    Vector3 gradiente = obterGradienteFormiga(formiga);
    Vector3 origemGradiente = {posicao.x + 0.055f, posicao.y + 0.075f,
        posicao.z + 0.045f};
    desenharSeta(origemGradiente, (Vector3){gradiente.x, 0.0f, gradiente.z}, 0.34f,
        (Color){255, 78, 190, 255});

    // Derivadas parciais são vetores tangentes nas direções dos eixos x e z.
    Vector3 derivadaX = {1.0f, gradiente.x, 0.0f};
    Vector3 derivadaZ = {0.0f, gradiente.z, 1.0f};
    Vector3 origemFx = {posicao.x - 0.055f, posicao.y + 0.045f,
        posicao.z - 0.045f};
    Vector3 origemFz = {posicao.x + 0.055f, posicao.y + 0.045f,
        posicao.z - 0.045f};
    desenharSeta(origemFx, derivadaX, 0.34f, (Color){255, 220, 65, 255});
    desenharSeta(origemFz, derivadaZ, 0.34f, (Color){255, 130, 55, 255});

    // Normal n=(-f_x, 1, -f_z), perpendicular às duas tangentes da superfície.
    Vector3 normal = normalizar((Vector3){-gradiente.x, 1.0f, -gradiente.z});
    Vector3 origemNormal = {posicao.x, posicao.y + 0.10f,
        posicao.z + 0.055f};
    desenharSeta(origemNormal, normal, 0.38f, (Color){160, 255, 110, 255});
}

/**
 * @brief Desenha a formiga com seu caminho e seus vetores auxiliares.
 *
 * É uma função de conveniência que desenha o corpo, o caminho percorrido e as
 * retas auxiliares.
 *
 * @param formiga Formiga que será desenhada.
 */
void desenharFormiga(const Formiga *formiga){
    desenharCorpoFormiga(formiga);
    desenharElementosFormiga(formiga, true, true);
}
