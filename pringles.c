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
 * A malha é constituída por anéis concêntricos distribuídos em um domínio
 * elíptico e por triângulos que conectam vértices adjacentes.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "raylib.h"
#include "pringles.h"

/**
 * @brief Calcula a cor dourada de um ponto da superfície.
 *
 * Combina a iluminação difusa com o escurecimento das bordas e uma textura
 * sutil para determinar a cor final do ponto.
 *
 * @param posX Coordenada do ponto na direção do eixo X.
 * @param posZ Coordenada do ponto na direção do eixo Z.
 * @param raioRelativo Distância relativa do ponto ao centro da superfície.
 * @param normal Vetor normal à superfície no ponto informado.
 *
 * @return Color Cor resultante, com opacidade total.
 */
static Color corDourada(float posX, float posZ, float raioRelativo, Vector3 normal){
    const Vector3 direcaoLuz = {-0.45f, 0.82f, -0.35f};
    float luzDifusa = normal.x * direcaoLuz.x +
        normal.y * direcaoLuz.y + normal.z * direcaoLuz.z;
    if (luzDifusa < 0.0f) luzDifusa = 0.0f;

    float iluminacao = 0.94f + 0.06f * luzDifusa;
    float bordaTostada = powf(raioRelativo, 12.0f);
    float textura = sinf(17.0f * posX + 8.0f * posZ) *
        cosf(21.0f * posZ - 5.0f * posX);

    Color cor = {
        (unsigned char)((232.0f - 8.0f * bordaTostada + 0.7f * textura) * iluminacao),
        (unsigned char)((182.0f - 12.0f * bordaTostada + 0.7f * textura) * iluminacao),
        (unsigned char)((82.0f - 8.0f * bordaTostada + 0.7f * textura) * iluminacao),
        255
    };
    return cor;
}

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
    // Semieixos da elipse que delimita o formato da superfície
    const float raioX = 1.5f;
    const float raioZ = 1.0f;
    // Discretização radial e angular do domínio elíptico
    const int quantidadeAneis = 32;
    const int quantidadeSetores = 64;

    // Um vértice central mais os vértices de cada anel elíptico
    Mesh mesh = {0};
    mesh.vertexCount = 1 + quantidadeAneis * quantidadeSetores;
    mesh.triangleCount = quantidadeSetores +
        (quantidadeAneis - 1) * quantidadeSetores * 2;
    mesh.vertices = malloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.normals = malloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.colors = malloc(mesh.vertexCount * 4 * sizeof(unsigned char));
    mesh.indices = malloc(mesh.triangleCount * 3 * sizeof(unsigned short));

    if (mesh.vertices == NULL || mesh.normals == NULL ||
        mesh.colors == NULL || mesh.indices == NULL){
        free(mesh.vertices);
        free(mesh.normals);
        free(mesh.colors);
        free(mesh.indices);
        return (Mesh){0};
    }

    // O vértice central também pertence à superfície matemática
    mesh.vertices[0] = 0.0f;
    mesh.vertices[1] = alturaPringles(0.0f, 0.0f);
    mesh.vertices[2] = 0.0f;
    mesh.normals[0] = 0.0f;
    mesh.normals[1] = 1.0f;
    mesh.normals[2] = 0.0f;
    Color corCentro = corDourada(0.0f, 0.0f, 0.0f, (Vector3){0.0f, 1.0f, 0.0f});
    mesh.colors[0] = corCentro.r;
    mesh.colors[1] = corCentro.g;
    mesh.colors[2] = corCentro.b;
    mesh.colors[3] = corCentro.a;

    // Cada anel tem raio crescente e mantém o contorno externo elíptico
    for (int anel = 1; anel <= quantidadeAneis; anel++){
        float raioRelativo = (float)anel / quantidadeAneis;

        for (int setor = 0; setor < quantidadeSetores; setor++){
            float angulo = 2.0f * PI * setor / quantidadeSetores;
            int indiceVertice = 1 + (anel - 1) * quantidadeSetores + setor;
            float posX = raioX * raioRelativo * cosf(angulo);
            float posZ = raioZ * raioRelativo * sinf(angulo);
            float comprimentoNormal = sqrtf(
                0.16f * posX * posX + 1.0f + 0.16f * posZ * posZ
            );

            mesh.vertices[indiceVertice * 3 + 0] = posX;
            mesh.vertices[indiceVertice * 3 + 1] = alturaPringles(posX, posZ);
            mesh.vertices[indiceVertice * 3 + 2] = posZ;

            // Normal analítica da superfície y = 0,2(x² - z²)
            mesh.normals[indiceVertice * 3 + 0] = -0.4f * posX / comprimentoNormal;
            mesh.normals[indiceVertice * 3 + 1] = 1.0f / comprimentoNormal;
            mesh.normals[indiceVertice * 3 + 2] = 0.4f * posZ / comprimentoNormal;

            Vector3 normal = {
                mesh.normals[indiceVertice * 3 + 0],
                mesh.normals[indiceVertice * 3 + 1],
                mesh.normals[indiceVertice * 3 + 2]
            };
            Color cor = corDourada(posX, posZ, raioRelativo, normal);
            mesh.colors[indiceVertice * 4 + 0] = cor.r;
            mesh.colors[indiceVertice * 4 + 1] = cor.g;
            mesh.colors[indiceVertice * 4 + 2] = cor.b;
            mesh.colors[indiceVertice * 4 + 3] = cor.a;
        }
    }

    int indiceTriangulo = 0;

    // Triângulos que ligam o centro ao primeiro anel
    for (int setor = 0; setor < quantidadeSetores; setor++){
        int proximoSetor = (setor + 1) % quantidadeSetores;
        mesh.indices[indiceTriangulo * 3 + 0] = 0;
        mesh.indices[indiceTriangulo * 3 + 1] = 1 + proximoSetor;
        mesh.indices[indiceTriangulo * 3 + 2] = 1 + setor;
        indiceTriangulo++;
    }

    // Triangulação entre os anéis, fechando cada setor da elipse
    for (int anel = 1; anel < quantidadeAneis; anel++){
        for (int setor = 0; setor < quantidadeSetores; setor++){
            int proximoSetor = (setor + 1) % quantidadeSetores;
            int interno = 1 + (anel - 1) * quantidadeSetores + setor;
            int internoProximo = 1 + (anel - 1) * quantidadeSetores + proximoSetor;
            int externo = 1 + anel * quantidadeSetores + setor;
            int externoProximo = 1 + anel * quantidadeSetores + proximoSetor;

            mesh.indices[indiceTriangulo * 3 + 0] = interno;
            mesh.indices[indiceTriangulo * 3 + 1] = externoProximo;
            mesh.indices[indiceTriangulo * 3 + 2] = externo;
            indiceTriangulo++;

            mesh.indices[indiceTriangulo * 3 + 0] = interno;
            mesh.indices[indiceTriangulo * 3 + 1] = internoProximo;
            mesh.indices[indiceTriangulo * 3 + 2] = externoProximo;
            indiceTriangulo++;
        }
    }

    // Transferência da malha para os recursos de processamento gráfico
    UploadMesh(&mesh, false);
    return mesh;
}
