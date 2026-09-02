/**
 * @file pringles.h
 * @author Tarsila Paiva Pimentel
 * @brief Declarações das funções utilizadas na modelagem e visualização
 * tridimensional de um parabolóide hiperbólico.
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 * @details
 * Este arquivo contém as declarações das funções responsáveis pela
 * avaliação da superfície matemática e pela construção de sua representação
 * computacional por meio de uma malha tridimensional utilizando raylib.
 */
#ifndef PRINGLES_H
#define PRINGLES_H

#include "raylib.h"

/**
 * @brief Determina a coordenada vertical da superfície do paraboloide hiperbólico.
 *
 * @param abscissa Coordenada da superfície na direção do eixo X.
 * @param ordenada Coordenada da superfície na direção do eixo Z.
 *
 * @return float Valor da coordenada vertical da superfície.
 */
float alturaPringles(float abscissa, float ordenada);

/**
 * @brief Constrói a malha tridimensional do paraboloide hiperbólico.
 *
 * @return Mesh Estrutura contendo a malha tridimensional da superfície.
 */
Mesh criarMeshPringles(void);

#endif