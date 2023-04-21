#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>

const int largura = 900, altura = 600;

float posYchao = (3 * altura) / 4;
int QuadroDePasso = 2, QuadroDePasso1 = 1;

// Definir a posição e tamanho do botão "Play"
Rectangle playButton = { largura/2 - 50, altura/2 - 100, 100, 50 };
    
// Definir a posição e tamanho do botão "Configurações"
Rectangle configButton = { largura/2 - 100, altura/2, 200, 50 };

// Definir a posição e tamanho do botão "Sair"
Rectangle exitButton = { largura/2 - 50, altura/2 + 100, 100, 50 };
    
// Definir a posição e tamnho do botão "Pause"
Rectangle pauseButton = {830, 0, 50, 50};
    
// Ligar ou desligar a musica
Rectangle musicButton = { largura/2 - 90, altura/2 + 200, 180, 50 };

void initGame(void);
bool quadroPasso(int frameInicial);
bool tocouChao(Texture2D *scarfy, Vector2 *scarfyPosicao);

int main(void)
{
    int musica = 0, veso = 0;
    
    InitWindow(largura, altura, "project-cin");
    SetTargetFPS(60);
    
    InitAudioDevice();
    Sound open = LoadSound("sounds/open.mp3");
    SetSoundVolume(open, 0.1);
    
    while (!WindowShouldClose()){
        
        if (musica == 0){
            PlaySound(open);
            musica = 1;
        }if (CheckCollisionPointRec(GetMousePosition(), playButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            veso = 1;
            initGame();
        }if (CheckCollisionPointRec(GetMousePosition(), configButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            //Configuracoes
        }if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            CloseWindow();
            CloseAudioDevice(); 
        }if (CheckCollisionPointRec(GetMousePosition(), musicButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(musica == 1){
                PauseSound(open);
                musica = 2;
            }else if(musica == 2){
                ResumeSound(open);
                musica = 1;
            }
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            if (veso == 0){
                DrawRectangleRec(playButton, BLUE);
                DrawRectangleLinesEx(playButton, 3, BLACK);
                DrawText("Jogar", playButton.x + 20, playButton.y + 15, 20, BLACK);
                
                DrawRectangleRec(configButton, YELLOW);
                DrawRectangleLinesEx(configButton, 3, BLACK);
                DrawText("Configurações", configButton.x + 30, configButton.y + 15, 20, BLACK);

                DrawRectangleRec(exitButton, RED);
                DrawRectangleLinesEx(exitButton, 3, BLACK);
                DrawText("Sair", exitButton.x + 35, exitButton.y + 15, 20, BLACK);
                if(musica == 1){
                    DrawRectangleRec(musicButton, GREEN);
                    DrawRectangleLinesEx(musicButton, 3, BLACK);
                    DrawText("LIGADA", musicButton.x + 55
                    , musicButton.y + 15, 20, BLACK);
                }else {
                    DrawRectangleRec(musicButton, RED);
                    DrawRectangleLinesEx(musicButton, 3, BLACK);
                    DrawText("DESLIGADA", musicButton.x + 35, musicButton.y + 15, 20, BLACK);
                }
            }
        EndDrawing();
        
        if(veso == 1) break;
    }
    CloseAudioDevice(); 
    CloseWindow();  
}

void initGame(void){
    
    Texture parado2 = LoadTexture("textures/Hurt2.png");
    Texture parado = LoadTexture("textures/Hurt.png");
    Texture2D scarfy = LoadTexture("textures/Scarfy.png");
    Sound puloSom = LoadSound("sounds/pular.mp3");
    Sound passoSom = LoadSound("sounds/correr.mp3");

    int numFrames = 8, parou = 0;
    int frameLarg = scarfy.width / numFrames;
    int velocidade = 10, gravidade = 1;
    int pulouFrame = 0, caiuFrame = 2;
    int frameDelay = 5, frameDelayContador = 0, frameInicial = 0;
    int moveu=0;
    
    Rectangle frameRec = { 0.0f, 0.0f, (float)frameLarg, (float)scarfy.height };
    Vector2 scarfyPosicao = {largura / 2.0f, posYchao - scarfy.height};
    Vector2 scarfyVelocidade = {0.0f,0.0f};
    
    SetSoundVolume(passoSom, 0.1);
    
    while(!WindowShouldClose()){
        if (CheckCollisionPointRec(GetMousePosition(), pauseButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            //resume
        }
        if (tocouChao(&scarfy, &scarfyPosicao)){
            if (IsKeyDown(KEY_SPACE)){
                scarfyVelocidade.y = -2 * velocidade;
            }
            if (IsKeyDown(KEY_D)){
                scarfyVelocidade.x = velocidade;
                if (frameRec.width < 0)
                {
                    frameRec.width = -frameRec.width;
                }
                parou = 1;
            }
            else if (IsKeyDown(KEY_A)){
                scarfyVelocidade.x = -velocidade;
                if (frameRec.width > 0){
                    frameRec.width = -frameRec.width;
                }
                parou = 2;
            }
            else{
                scarfyVelocidade.x = 0;
            }
        }

        bool scarfyMover = scarfyVelocidade.x != 0.0f || scarfyVelocidade.y != 0.0f;
        bool SeTocouNoChao = tocouChao(&scarfy, &scarfyPosicao);
        scarfyPosicao = Vector2Add(scarfyPosicao, scarfyVelocidade);
        bool scarfyTocouNoChao = tocouChao(&scarfy, &scarfyPosicao);

        if (scarfyTocouNoChao){
            scarfyVelocidade.y = 0;
            scarfyPosicao.y = posYchao - scarfy.height;
            if (!SeTocouNoChao)
            {
                PlaySound(puloSom);
            }
        }
        else{
            scarfyVelocidade.y += gravidade;
        }

        ++frameDelayContador;
        if (frameDelayContador > frameDelay){
            frameDelayContador = 0;

            if (scarfyMover){
                if (scarfyTocouNoChao){
                    ++frameInicial;
                    frameInicial %= numFrames;

                    if (quadroPasso(frameInicial) && moveu == false){
                        PlaySound(passoSom);
                        moveu = true;
                    }
                }
                else{
                    PauseSound(passoSom);
                    if (scarfyVelocidade.y < 0){
                        frameInicial = pulouFrame;
                    }
                    else
                    {
                        frameInicial = caiuFrame;
                    }
                    moveu = false;
                }
                frameRec.x = (float)frameLarg * frameInicial;
            }
            else{
                PauseSound(passoSom);
                moveu = false;
            }
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A) || IsKeyDown(KEY_SPACE)){
                DrawTextureRec(scarfy, frameRec, scarfyPosicao, WHITE);
            }
            if (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A) && !IsKeyDown(KEY_SPACE)){
                if (parou == 1)
                    DrawTexture(parado, scarfyPosicao.x, scarfyPosicao.y, WHITE);
                else
                    DrawTexture(parado2, scarfyPosicao.x, scarfyPosicao.y, WHITE);
            }
            DrawRectangleRec(pauseButton, GREEN);
            DrawRectangleLinesEx(pauseButton, 3, BLACK);
            DrawText("| |", pauseButton.x + 20, pauseButton.y + 15, 20, BLACK);
        EndDrawing();
    }
}

bool quadroPasso(int frameInicial) {
    if(frameInicial == QuadroDePasso1 || frameInicial == QuadroDePasso) return true;
    else return false;
}

bool tocouChao(Texture2D *scarfy, Vector2 *scarfyPosicao) {
    if(scarfyPosicao->y + scarfy->height >= posYchao) return true;
    else return false;
}
