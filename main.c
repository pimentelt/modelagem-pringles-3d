/**
 * @file main.c
 * @author Tarsila Paiva Pimentel
 * @brief Programa principal para a modelagem e visualização computacional
 * de um paraboloide hiperbólico em ambiente tridimensional utilizando raylib.
 * @version 0.1
 * @date 2026-09-02
 * 
 * @copyright Copyright (c) 2026
 * 
 * @details
 * Inicializa a janela gráfica, configura a câmera tridimensional e realiza
 * a renderização de uma malha que representa um paraboloide hiperbólico.
 *
 * O programa permite redimensionamento da janela, alternância para tela cheia
 * através da tecla F11 e interação com a câmera utilizando o botão esquerdo
 * do mouse.
 *
 * A superfície matemática é gerada pela função definida em pringles.c e
 * representada graficamente por uma Mesh do raylib.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "raylib.h"
#include "pringles.h"
#include "rlgl.h"

/**
 * @brief Função principal do programa.
 *
 * Inicializa os recursos gráficos, configura a câmera e o material da
 * superfície, executa o loop principal de renderização e libera os
 * recursos alocados ao final da execução.
 *
 * @return int Retorna 0 quando o programa é encerrado normalmente.
 */
int main(int argc, char *argv[]){
    // Inicialização e parametrização do contexto gráfico
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Modelagem e Visualização 3D de um Parabolóide Hiperbólico");
    MaximizeWindow();

    SetTargetFPS(60);

    // Configuração dos parâmetros da câmera tridimensional
    Camera3D camera = {0};
    camera.position = (Vector3){6.0f, 5.0f, 6.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};

    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Instanciação da malha e definição das propriedades materiais
    Mesh meshPringles = criarMeshPringles();
    Material materialPringles = LoadMaterialDefault();
    materialPringles.maps[MATERIAL_MAP_DIFFUSE].color = YELLOW;

    // Laço principal de processamento e renderização
    while (!WindowShouldClose()){
        // Alternância do modo de exibição da janela
        if (IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }

        int largura = GetScreenWidth();
        int altura = GetScreenHeight();

        // Atualização dos parâmetros da câmera a partir da entrada do usuário
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 mouseDelta = GetMouseDelta();

            camera.target.x += mouseDelta.x * 0.01f;
            camera.target.y -= mouseDelta.y * 0.01f;
        }

        // Composição e renderização da cena tridimensional
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawGrid(40, 0.5f);

        // Desabilita a eliminação de faces posteriores da malha (alteração posterior na versão finalizada*)
        rlDisableBackfaceCulling();

        DrawMesh(
            meshPringles,
            materialPringles,
            (Matrix){1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f}
        );

        rlEnableBackfaceCulling();
        EndMode3D();

        DrawText(
            "Visualização Computacional de um Parabolóide Hiperbólico",
            30,
            30,
            30,
            BLACK
        );

        DrawText(
            TextFormat("Resolução atual: %d x %d", largura, altura),
            30,
            70,
            20,
            DARKGRAY
        );

        EndDrawing();
    }
    UnloadMesh(meshPringles);
    CloseWindow();
    return 0;
}