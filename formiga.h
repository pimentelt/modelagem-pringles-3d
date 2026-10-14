/**
 * @file formiga.h
 * @author Tarsila Paiva Pimentel
 * @brief Declarações das estruturas e funções usadas para simular e desenhar
 * uma formiga sobre a superfície de um parabolóide hiperbólico.
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 * @details
 * Este arquivo define a estrutura Formiga, que armazena o estado da formiga,
 * seus dados de movimentação e o histórico de posições percorridas. Também
 * declara funções para inicializar, atualizar, reiniciar, consultar e desenhar
 * a formiga e seus elementos visuais.
 */
#ifndef FORMIGA_H
#define FORMIGA_H

#include <stdbool.h>
#include <stdint.h>
#include "raylib.h"

#define FORMIGA_HISTORICO_MAX 512

/**
 * @brief Representa uma formiga e os dados de sua movimentação.
 *
 * Armazena a posição, a orientação, os temporizadores, o estado aleatório
 * e o histórico de posições percorridas.
 */
typedef struct Formiga{
    float x;                      /**< Posição no eixo X. */
    float z;                      /**< Posição no eixo Z. */
    float angulo;                 /**< Ângulo de orientação, em radianos. */
    float velocidade;             /**< Velocidade de deslocamento. */
    float tempo;                  /**< Tempo acumulado de simulação. */
    float tempoAteVirar;          /**< Tempo restante até a próxima mudança de direção. */
    float tempoAteRegistrar;      /**< Tempo restante até registrar uma posição no histórico. */
    uint32_t semente;             /**< Semente usada para inicializar os valores aleatórios. */
    uint32_t estadoAleatorio;     /**< Estado atual do gerador de números aleatórios. */
    bool pausada;                 /**< Indica se a movimentação está pausada. */
    Vector3 historico[FORMIGA_HISTORICO_MAX]; /**< Posições registradas durante o percurso. */
    int quantidadeHistorico;      /**< Número de posições válidas no histórico. */
    int proximoHistorico;         /**< Índice da próxima posição a ser sobrescrita. */
} Formiga;

/**
 * @brief Inicializa a formiga com uma semente para os valores aleatórios.
 *
 * @param formiga Ponteiro para a formiga que será inicializada.
 * @param semente Semente usada para gerar a sequência aleatória.
 */
void inicializarFormiga(Formiga *formiga, uint32_t semente);

/**
 * @brief Atualiza o estado e a posição da formiga.
 *
 * @param formiga Ponteiro para a formiga que será atualizada.
 * @param deltaTempo Intervalo de tempo decorrido desde a última atualização.
 */
void atualizarFormiga(Formiga *formiga, float deltaTempo);

/**
 * @brief Reinicia a formiga para seu estado inicial.
 *
 * @param formiga Ponteiro para a formiga que será reiniciada.
 */
void reiniciarFormiga(Formiga *formiga);

/**
 * @brief Obtém a posição atual da formiga na cena.
 *
 * @param formiga Ponteiro constante para a formiga.
 *
 * @return Vector3 Posição atual da formiga.
 */
Vector3 obterPosicaoFormiga(const Formiga *formiga);

/**
 * @brief Obtém o gradiente da superfície na posição da formiga.
 *
 * @param formiga Ponteiro constante para a formiga.
 *
 * @return Vector3 Gradiente da superfície no ponto atual.
 */
Vector3 obterGradienteFormiga(const Formiga *formiga);

/**
 * @brief Obtém o vetor de direção da formiga.
 *
 * @param formiga Ponteiro constante para a formiga.
 *
 * @return Vector3 Direção atual da formiga.
 */
Vector3 obterDirecaoFormiga(const Formiga *formiga);

/**
 * @brief Obtém o vetor tangente à superfície na posição da formiga.
 *
 * @param formiga Ponteiro constante para a formiga.
 *
 * @return Vector3 Vetor tangente à superfície.
 */
Vector3 obterTangenteFormiga(const Formiga *formiga);

/**
 * @brief Desenha o corpo da formiga.
 *
 * @param formiga Ponteiro constante para a formiga que será desenhada.
 */
void desenharCorpoFormiga(const Formiga *formiga);

/**
 * @brief Desenha elementos visuais auxiliares da formiga.
 *
 * @param formiga Ponteiro constante para a formiga.
 * @param mostrarRetas Indica se as retas auxiliares devem ser exibidas.
 * @param mostrarCaminho Indica se o caminho percorrido deve ser exibido.
 */
void desenharElementosFormiga(const Formiga *formiga, bool mostrarRetas,
    bool mostrarCaminho);

/**
 * @brief Desenha a formiga.
 *
 * @param formiga Ponteiro constante para a formiga que será desenhada.
 */
void desenharFormiga(const Formiga *formiga);

#endif
