/**
 * @file pringles.c
 * @author Tarsila Paiva Pimentel
 * @brief Definição da função matemática e construção da malha tridimensional
 * correspondente a um paraboloide hiperbólico.
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 * @details
 * Este módulo implementa a função responsável pela determinação da
 * coordenada vertical da superfície e a geração de sua representação
 * computacional por meio de uma malha poligonal.
 *
 * A superfície é definida por uma função de duas variáveis, na qual as
 * coordenadas de entrada correspondem às componentes horizontais da
 * superfície e o valor retornado corresponde à sua altura.
 *
 * A malha é constituída por vértices distribuídos sobre um domínio
 * retangular e por triângulos que conectam vértices adjacentes, produzindo
 * uma aproximação discreta da superfície contínua.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "raylib.h"
#include "pringles.h"

/**
 * @brief Determina a coordenada vertical da superfície do paraboloide hiperbólico.
 *
 * Avalia a função de duas variáveis responsável pela representação matemática
 * da superfície, utilizando as coordenadas horizontais como argumentos.
 *
 * @param abscissa Coordenada da superfície na direção do eixo X.
 * @param ordenada Coordenada da superfície na direção do eixo Z.
 *
 * @return float Valor da coordenada vertical da superfície no ponto informado.
 */
float alturaPringles(float abscissa, float ordenada){
    // Parâmetro associado à intensidade da curvatura da superfície
    float alturaCurvatura = 0.2f;
    return alturaCurvatura * (
        abscissa * abscissa -
        ordenada * ordenada
    );
}

/**
 * @brief Constrói a malha tridimensional do paraboloide hiperbólico.
 *
 * Realiza a discretização do domínio da função matemática e utiliza os
 * pontos obtidos para construir uma malha poligonal composta por vértices
 * e triângulos.
 *
 * Cada vértice possui três componentes espaciais (X, Y e Z), sendo a
 * coordenada Y determinada pela função @ref alturaPringles.
 *
 * @return Mesh Estrutura contendo a malha tridimensional da superfície.
 */
Mesh criarMeshPringles(void){
    // Discretização do domínio nas direções dos eixos X e Z
    int quantidadeX = 31;
    int quantidadeZ = 21;
    // Intervalo entre pontos consecutivos da discretização
    float passo = 0.1f;

    // Inicialização da estrutura responsável pela representação da malha
    Mesh mesh = {0};
    mesh.vertexCount = quantidadeX * quantidadeZ;
    mesh.triangleCount = (quantidadeX - 1) * (quantidadeZ - 1) * 2;
    mesh.vertices = malloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.indices = malloc(mesh.triangleCount * 3 * sizeof(unsigned short));

    int indiceVertice = 0;

    // Percurso da discretização do domínio nas direções X e Z
    for (int z = 0; z < quantidadeZ; z++){
        for (int x = 0; x < quantidadeX; x++){
            float posX = -1.5f + x * passo;
            float posZ = -1.0f + z * passo;
            float posY = alturaPringles(posX, posZ);

            mesh.vertices[indiceVertice * 3 + 0] = posX;
            mesh.vertices[indiceVertice * 3 + 1] = posY;
            mesh.vertices[indiceVertice * 3 + 2] = posZ;

            indiceVertice++;
        }
    }

    int indiceTriangulo = 0;

    // Percurso das células adjacentes da discretização
    for (int z = 0; z < quantidadeZ - 1; z++){
        for (int x = 0; x < quantidadeX - 1; x++){
            int superiorEsquerdo = z * quantidadeX + x;
            int superiorDireito = superiorEsquerdo + 1;
            int inferiorEsquerdo = (z + 1) * quantidadeX + x;
            int inferiorDireito = inferiorEsquerdo + 1;

            mesh.indices[indiceTriangulo * 3 + 0] = superiorEsquerdo;
            mesh.indices[indiceTriangulo * 3 + 1] = superiorDireito;
            mesh.indices[indiceTriangulo * 3 + 2] = inferiorEsquerdo;

            indiceTriangulo++;

            mesh.indices[indiceTriangulo * 3 + 0] = superiorDireito;
            mesh.indices[indiceTriangulo * 3 + 1] = inferiorDireito;
            mesh.indices[indiceTriangulo * 3 + 2] = inferiorEsquerdo;

            indiceTriangulo++;
        }
    }

    // Transferência da malha para os recursos de processamento gráfico
    UploadMesh(&mesh, false);
    return mesh;
}