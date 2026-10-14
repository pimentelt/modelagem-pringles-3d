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
#include "formiga.h"
#include "rlgl.h"

static const Color corFundoCena = {15, 36, 67, 255};

/**
 * @brief Desenha uma fração com numerador e denominador empilhados.
 *
 * Centraliza os dois textos em relação ao mais largo e traça uma linha entre
 * eles.
 *
 * @param fonte Fonte usada para desenhar os textos.
 * @param numerador Texto do numerador.
 * @param denominador Texto do denominador.
 * @param x Coordenada horizontal inicial da fração.
 * @param y Coordenada vertical inicial da fração.
 * @param tamanho Tamanho dos textos.
 * @param cor Cor dos textos e da linha divisória.
 *
 * @return float Largura ocupada pela fração, em pixels.
 */
static float desenharFracao(Font fonte, const char *numerador,
    const char *denominador, float x, float y, float tamanho, Color cor){
    float larguraNumerador = MeasureTextEx(fonte, numerador, tamanho, 1.0f).x;
    float larguraDenominador = MeasureTextEx(fonte, denominador, tamanho, 1.0f).x;
    float largura = fmaxf(larguraNumerador, larguraDenominador) + 6.0f;
    DrawTextEx(fonte, numerador,
        (Vector2){x + (largura - larguraNumerador) * 0.5f, y},
        tamanho, 1.0f, cor);
    DrawLineEx((Vector2){x, y + tamanho + 1.0f},
        (Vector2){x + largura, y + tamanho + 1.0f}, 1.5f, cor);
    DrawTextEx(fonte, denominador,
        (Vector2){x + (largura - larguraDenominador) * 0.5f, y + tamanho + 3.0f},
        tamanho, 1.0f, cor);
    return largura;
}

/**
 * @brief Desenha uma grade no chão da cena.
 *
 * Traça linhas nos eixos X e Z sobre o plano definido pela altura do chão,
 * usando espaçamento uniforme e a cor configurada para a grade.
 */
static void desenharGradeChao(void){
    const float alturaChao = -0.35f;
    const float limite = 3.0f;
    const float espacamento = 0.5f;
    const Color corGrade = {40, 56, 88, 255};

    for (float posicao = -limite; posicao <= limite; posicao += espacamento){
        DrawLine3D(
            (Vector3){posicao, alturaChao, -limite},
            (Vector3){posicao, alturaChao, limite},
            corGrade
        );
        DrawLine3D(
            (Vector3){-limite, alturaChao, posicao},
            (Vector3){limite, alturaChao, posicao},
            corGrade
        );
    }
}

/**
 * @brief Desenha o contorno da superfície de uma Pringles.
 *
 * Percorre uma elipse com 96 segmentos, calcula a altura da superfície em
 * cada ponto e conecta os pontos com cilindros para formar o contorno.
 *
 * @note Os cilindros são posicionados ligeiramente acima da superfície e
 * desenhados em duas espessuras, usando a cor de fundo da cena.
 */
static void desenharContornoPringles(void){
    const int segmentos = 96;
    const float raioX = 1.5f;
    const float raioZ = 1.0f;
    Vector3 anterior = {raioX, alturaPringles(raioX, 0.0f) + 0.012f, 0.0f};

    for (int i = 1; i <= segmentos; i++){
        float angulo = 2.0f * PI * (float)i / (float)segmentos;
        float x = raioX * cosf(angulo);
        float z = raioZ * sinf(angulo);
        Vector3 atual = {x, alturaPringles(x, z) + 0.012f, z};
        DrawCylinderEx(anterior, atual, 0.0285f, 0.0285f, 6, corFundoCena);
        DrawCylinderEx(anterior, atual, 0.0114f, 0.0114f, 6, corFundoCena);
        anterior = atual;
    }
}

/**
 * @brief Função principal do programa.
 *
 * Inicializa os recursos gráficos, configura a câmera e o material da
 * superfície, executa o loop principal de renderização e libera os
 * recursos alocados ao final da execução.
 *
 * @return int Retorna 0 quando o programa é encerrado normalmente.
 */
int main(void){
    const char *titulo = "FORMIGA CAMINHANDO SOB PRINGLES";
    // Inicialização e parametrização do contexto gráfico
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "FORMIGA CAMINHANDO SOB PRINGLES");
    MaximizeWindow();

    int monitorAtual = GetCurrentMonitor();
    int larguraFormiga = (int)(GetMonitorWidth(monitorAtual) * 0.80f);
    int alturaFormiga = (int)(GetMonitorHeight(monitorAtual) * 0.80f);
    if (larguraFormiga < 320) larguraFormiga = 320;
    if (alturaFormiga < 240) alturaFormiga = 240;
    RenderTexture2D texturaFormiga = LoadRenderTexture(larguraFormiga, alturaFormiga);
    SetTextureFilter(texturaFormiga.texture, TEXTURE_FILTER_POINT);

    SetTargetFPS(60);
    // Configuração dos parâmetros da câmera tridimensional
    Camera3D camera = {0};
    camera.position = (Vector3){4.8f, 3.8f, 4.8f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};

    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Instanciação da malha e definição das propriedades materiais
    Mesh meshPringles = criarMeshPringles();
    Material materialPringles = LoadMaterialDefault();
    // As cores dos vértices já incluem o dourado, a luz suave e o tostado.
    materialPringles.maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
    Formiga formiga;
    inicializarFormiga(&formiga, 0x00C0FFEEu);
    const char *modosVisualizacao[4] = {
        "retas e caminho ocultos",
        "somente retas",
        "somente caminho",
        "retas e caminho"
    };
    int modoVisualizacao = 3;

    // Carrega o TTF da Cambria diretamente; o TTC pode falhar no carregador do raylib.
    const char *caminhoFonteTitulo = "C:/Windows/Fonts/segoeui.ttf";
    const char *caminhoFonteMatematica = "C:/Windows/Fonts/cambriab.ttf";
    Font fonteTitulo = GetFontDefault();
    Font fonteMatematica = GetFontDefault();
    bool fonteTituloCarregada = false;
    bool fonteMatematicaCarregada = false;
    int quantidadePontosCodigo = 0;
    const char *textoFonte =
        "Formiga aleatória sobre um paraboloide hiperbólico "
        "0123456789x Resolução posição velocidade semente rad "
        "f(x,z)=0.2 D: 1.00 ≤ +/− |,() .* _= × · ²"
        "∇f ∂f θ cos sin ᵤ T̂ n̂ gradiente tangente aleatória persistente "
        "ESPAÇO pausa R reinicia setas esquerda direita ajustam rumo x_y z "
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int *pontosCodigo = LoadCodepoints(textoFonte, &quantidadePontosCodigo);
    if (pontosCodigo != NULL){
        if (FileExists(caminhoFonteTitulo)){
            fonteTitulo = LoadFontEx(
                caminhoFonteTitulo,
                30,
                pontosCodigo,
                quantidadePontosCodigo
            );
            fonteTituloCarregada = fonteTitulo.texture.id != 0;
        }
        if (FileExists(caminhoFonteMatematica)){
            fonteMatematica = LoadFontEx(
                caminhoFonteMatematica,
                64,
                pontosCodigo,
                quantidadePontosCodigo
            );
            fonteMatematicaCarregada = fonteMatematica.texture.id != 0;
        }
        UnloadCodepoints(pontosCodigo);
    }
    Font fonteResultados = fonteMatematicaCarregada ? fonteMatematica : fonteTitulo;

    // Laço principal de processamento e renderização
    while (!WindowShouldClose()){
        // Alternância do modo de exibição da janela
        if (IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }

        int largura = GetScreenWidth();
        int altura = GetScreenHeight();
        float deltaTempo = GetFrameTime();

        float movimentoZoom = GetMouseWheelMove();
        if (fabsf(movimentoZoom) > 0.0f){
            float dx = camera.position.x - camera.target.x;
            float dy = camera.position.y - camera.target.y;
            float dz = camera.position.z - camera.target.z;
            float distancia = sqrtf(dx * dx + dy * dy + dz * dz);
            float novaDistancia = distancia * powf(0.88f, movimentoZoom);
            if (novaDistancia < 2.5f) novaDistancia = 2.5f;
            if (novaDistancia > 18.0f) novaDistancia = 18.0f;
            float escalaZoom = novaDistancia / distancia;
            camera.position = (Vector3){
                camera.target.x + dx * escalaZoom,
                camera.target.y + dy * escalaZoom,
                camera.target.z + dz * escalaZoom
            };
        }

        if (IsKeyPressed(KEY_SPACE)) formiga.pausada = !formiga.pausada;
        if (IsKeyPressed(KEY_M)) modoVisualizacao = (modoVisualizacao + 1) % 4;
        if (IsKeyPressed(KEY_R)) reiniciarFormiga(&formiga);
        if (IsKeyPressed(KEY_N)){
            uint32_t novaSemente = formiga.semente ^
                (uint32_t)(GetTime() * 1000000.0) ^ 0x9E3779B9u;
            inicializarFormiga(&formiga, novaSemente);
        }
        if (IsKeyDown(KEY_RIGHT)) formiga.velocidade += 0.7f * deltaTempo;
        if (IsKeyDown(KEY_LEFT)) formiga.velocidade -= 0.7f * deltaTempo;
        if (formiga.velocidade < 0.15f) formiga.velocidade = 0.15f;
        if (formiga.velocidade > 1.3f) formiga.velocidade = 1.3f;
        atualizarFormiga(&formiga, deltaTempo);

        // Atualização dos parâmetros da câmera a partir da entrada do usuário
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 mouseDelta = GetMouseDelta();

            camera.target.x += mouseDelta.x * 0.01f;
            camera.target.y -= mouseDelta.y * 0.01f;
        }

        BeginDrawing();
        ClearBackground(corFundoCena);
        BeginMode3D(camera);
        desenharGradeChao();

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
        desenharContornoPringles();

        rlEnableBackfaceCulling();
        EndMode3D();

        // Isola o corpo da formiga numa textura 20% menor e pixelada.
        BeginTextureMode(texturaFormiga);
        ClearBackground((Color){0, 0, 0, 0});
        BeginMode3D(camera);
        desenharCorpoFormiga(&formiga);
        EndMode3D();
        EndTextureMode();

        DrawTexturePro(
            texturaFormiga.texture,
            (Rectangle){0.0f, 0.0f, (float)larguraFormiga, -(float)alturaFormiga},
            (Rectangle){0.0f, 0.0f, (float)largura, (float)altura},
            (Vector2){0.0f, 0.0f},
            0.0f,
            WHITE
        );

        // Trilha e vetores ficam na resolução integral para continuarem nítidos.
        BeginMode3D(camera);
        desenharElementosFormiga(
            &formiga,
            modoVisualizacao == 1 || modoVisualizacao == 3,
            modoVisualizacao == 2 || modoVisualizacao == 3
        );
        EndMode3D();

        DrawTextEx(
            fonteResultados,
            titulo,
            (Vector2){30.0f, 28.0f},
            30.0f,
            1.0f,
            (Color){245, 244, 255, 255}
        );

        Vector3 posicaoFormiga = obterPosicaoFormiga(&formiga);
        Vector3 gradienteFormiga = obterGradienteFormiga(&formiga);
        Vector3 direcaoFormiga = obterDirecaoFormiga(&formiga);
        float derivadaDirecional = gradienteFormiga.x * direcaoFormiga.x +
            gradienteFormiga.z * direcaoFormiga.z;

        DrawTextEx(fonteResultados,
            "Pringles: y=f(x,z)=0.2(x²−z²),  x²/2.25+z²≤1",
            (Vector2){36.0f, 82.0f}, 21.0f, 1.0f, WHITE);
        DrawTextEx(
            fonteResultados,
            TextFormat("Posição P=(x,y,z)=(%.3f, %.3f, %.3f)",
                posicaoFormiga.x, posicaoFormiga.y, posicaoFormiga.z),
            (Vector2){36.0f, 115.0f}, 21.0f, 1.0f, WHITE);

        DrawTextEx(fonteResultados,
            TextFormat("∇f(P)=(∂f/∂x, ∂f/∂z)=(%.3f, %.3f)",
                gradienteFormiga.x, gradienteFormiga.z),
            (Vector2){36.0f, 151.0f}, 20.0f, 1.0f, (Color){255, 112, 202, 255});

        Color corParciais = (Color){255, 218, 107, 255};
        float larguraFracao = desenharFracao(fonteResultados, "∂f", "∂x",
            36.0f, 185.0f, 18.0f, corParciais);
        DrawTextEx(fonteResultados,
            TextFormat("= 0.4x = %.3f", gradienteFormiga.x),
            (Vector2){36.0f + larguraFracao + 8.0f, 196.0f},
            18.0f, 1.0f, corParciais);
        float xParcialZ = 280.0f;
        larguraFracao = desenharFracao(fonteResultados, "∂f", "∂z",
            xParcialZ, 185.0f, 18.0f, corParciais);
        DrawTextEx(fonteResultados,
            TextFormat("= −0.4z = %.3f", gradienteFormiga.z),
            (Vector2){xParcialZ + larguraFracao + 8.0f, 196.0f},
            18.0f, 1.0f, corParciais);
        DrawTextEx(
            fonteResultados,
            TextFormat("Dᵤf(P)=∇f(P)·u=%.3f | u=(%.2f, %.2f)",
                derivadaDirecional, direcaoFormiga.x, direcaoFormiga.z),
            (Vector2){36.0f, 231.0f}, 20.0f, 1.0f, (Color){75, 225, 250, 255});
        DrawTextEx(fonteResultados, "Setas:", (Vector2){36.0f, 270.0f},
            18.0f, 1.0f, WHITE);
        DrawTextEx(fonteResultados, "Amarela: tangente (eixo x)",
            (Vector2){36.0f, 294.0f}, 18.0f, 1.0f, (Color){255, 220, 65, 255});
        DrawTextEx(fonteResultados, "Laranja: tangente (eixo z)",
            (Vector2){36.0f, 317.0f}, 18.0f, 1.0f, (Color){255, 130, 55, 255});
        DrawTextEx(fonteResultados, "Rosa: gradiente ∇f",
            (Vector2){36.0f, 340.0f}, 18.0f, 1.0f, (Color){255, 78, 190, 255});
        DrawTextEx(fonteResultados, "Ciano: trajetória da formiga",
            (Vector2){36.0f, 363.0f}, 18.0f, 1.0f, (Color){42, 220, 247, 255});
        DrawTextEx(fonteResultados, "Verde: normal à superfície",
            (Vector2){36.0f, 386.0f}, 18.0f, 1.0f, (Color){160, 255, 110, 255});

        DrawTextEx(fonteResultados,
            TextFormat("M: %s | ESPAÇO pausa | R reinicia | scroll: zoom",
                modosVisualizacao[modoVisualizacao]),
            (Vector2){24.0f, (float)altura - 52.0f}, 16.0f, 1.0f,
            (Color){245, 244, 255, 255});
        DrawTextEx(fonteResultados,
            TextFormat("Resolução: %d × %d", largura, altura),
            (Vector2){24.0f, (float)altura - 28.0f}, 16.0f, 1.0f,
            (Color){150, 226, 244, 255});

        EndDrawing();
    }
    if (fonteTituloCarregada) UnloadFont(fonteTitulo);
    if (fonteMatematicaCarregada) UnloadFont(fonteMatematica);
    UnloadRenderTexture(texturaFormiga);
    UnloadMesh(meshPringles);
    CloseWindow();
    return 0;
}
