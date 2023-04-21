#include "raylib.h"

#define screen_width 1280
#define screen_hight 800

int main(){

    InitWindow(screen_width, screen_hight, "Run");

    Texture2D texture = LoadTexture("Área_de_trabalho/Enchantress/walk.png");
    

    //posição casa
    
    Rectangle casa = {-250, 20, 300, 300};

    //lista de lugar onde havera solo para o personagem andar

    Rectangle solo[] = {{-1000, 320, 2000, 20 },
        {1200, 420, 500, 20},
        {1800, 320, 1000, 20},
        {2900, 220, 1500, 20},
        {3000, 140, 140, 20},
        {4500, 220, 140, 20},
        {4800, 140, 140, 20},
        {5000, 60, 140, 20},
        {5300, 20, 140, 20},
        {5600, -40, 140, 20},
        {6115, 420, 2000, 20},
        {6200, 340, 140, 20},
        {6340, 260, 140, 20},
        {6480, 180, 140, 20},
        {6620, 100, 140, 20},
        {6860, 20, 140, 20},
        {7000, -60, 140, 20},
        {8300, 350, 1000, 20},
        {9450, 400, 400, 20},
        {10000, 320, 3000, 20},
        
    };
    
    //lista de lugar onde havera parede
    
    Rectangle parede[] = {{-1000, 340, 2000, 8000},
        {1200, 440, 500, 8000},
        {1800, 340, 1000, 8000},
        {2900, 240, 1500, 8000},
        {3000, 160, 140, 8000},
        {4500, 240, 140, 8000},
        {4800, 160, 140, 8000},
        {5000, 80, 140, 8000},
        {5300, 40, 140, 8000},
        {5600, -20, 140, 8000},
        {6115, 440, 2000, 8000},
        {6200, 360, 140, 8000},
        {6340, 280, 140, 8000},
        {6480, 200, 140, 8000},
        {6620, 120, 140, 8000},
        {6860, 40, 140, 8000},
        {7000, -40, 140, 8000},
        {8300, 370, 1000, 8000},
        {9450, 420, 400, 8000},
        {10000, 340, 3000, 8000},
    };
    
    //cracha
    
    Rectangle cracha = { -50, 300, 20, 20};
    
    //struct da onda
    Rectangle onda = {-2600, -100, 1200, 8000};
    
    //redbull
    
    Rectangle redbull = {7300, -50, 30, 30};
    
    //struct onde tem a posição do personagem (x,Y), sua largura e tamanho

    Rectangle player = {100, 270, 50, 50};
    
    //struct pro cin
    
    Rectangle cin = {11200, -80, 400, 400};
    
    bool fim = false;
    bool podeEntrar = false;
    bool pegar = false;
    int atual;
    bool pulo = false;
    float puloSpeed = 10.0f;
    float gravidade = 0.5f;
    float playerPosition;
    float playerspeed = 5;
    float playerspeedonda = 5.5;
    
    playerPosition = player.y;
    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.x + 250.0f, playerPosition - 50};
    camera.offset = (Vector2){ screen_width/2.0f, screen_hight/2.0f };
    camera.zoom = 1.0f;

    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        
    // verificar se o botao direito foi clicado
    if(IsKeyDown(KEY_RIGHT)){
        player.x += playerspeed;
    }
    // verificar se o botao esquerdo foi clicado
    if(IsKeyDown(KEY_LEFT)){
        player.x -= playerspeed;
    }
    //movimento da onda
    onda.x += playerspeedonda;
    
    // verificar se o botao pular/para cima foi clicado
    if(IsKeyDown(KEY_UP) && !pulo){
        pulo = true;     
        puloSpeed = 10.0f;
        gravidade = 0.5f;            
    }
   
    if(pulo){
        player.y -= puloSpeed;
        puloSpeed -= gravidade;
        
        // detectar colisão com o solo e redefinir pulo
        for (int i = 0; i < 20; i++){
            if (CheckCollisionRecs(player, solo[i])){
                pulo = false;
                player.y = solo[i].y - player.height;
                break;
            }
        }
    }
    else {
        // Detectar colisão com o solo e permitir pular novamente
        for (int i = 0; i < 20; i++){
            if (CheckCollisionRecs(player, solo[i])){
                pulo = false;
                break;
            }
            else {
                //caso o personagem nao tenha pulado e passo do tamanho do solo e caiu no buraco entao ele so vai cair
                pulo = true;
                puloSpeed = 0;
            }
        }
    }
    
    // detectar colisão com a parede e parar o jogador
    for (int i = 0; i < 20 && fim == false; i++){
        if (CheckCollisionRecs(player, parede[i])){
            playerspeed = 0;
            atual = i;
            break;
        }
        else {
            playerspeed = 5;
        }
    }
    
    if(CheckCollisionRecs(player, onda) || player.y > 700){
        CloseWindow();
    }
    
    if(CheckCollisionRecs(player, cracha)){
        podeEntrar = true;
        //se alguem conseguir colocar pra escrever na tela a mensagem (voce pegou o cracha!)
    }
    
    if(CheckCollisionRecs(player, cin)){
        fim = true;
        if(podeEntrar == true){
            CloseWindow();   
                //você ganhou (mas essa aqui pode ser uma tela pos finalizar o jogo)
        }else if(podeEntrar == false){
            playerspeed = 0;
            //se alguem conseguir colocar pra escrever na tela a mensagem ("Você esqueceu seu crachá!");
        }
    }
    
    //movimentação da camera    
    camera.target = (Vector2){player.x + 250, playerPosition - 50};
    
    BeginDrawing();
    BeginMode2D(camera);

    ClearBackground(WHITE);
    
    //desenhar os objetos na tela
    DrawRectangle(player.x, player.y, player.width, player.height, RED);
    //quando colocar a textura usar essa função ->DrawTexture(texture, player.x, player.y, WHITE);
    for (int i = 0; i < 20; i++){
        DrawRectangle(solo[i].x, solo[i].y, solo[i].width, solo[i].height, DARKGREEN);
        DrawRectangle(parede[i].x, parede[i].y, parede[i].width, parede[i].height, BROWN);
    }
    if(CheckCollisionRecs(player, redbull)){
        pegar = true;   
    }
    if(!pegar){
        DrawRectangle(redbull.x, redbull.y, redbull.width, redbull.height, BLUE);
    }else{
        playerspeedonda = 5;
        playerspeed = 5.33;
    }
    DrawRectangle(cracha.x, cracha.y, cracha.width, cracha.height, RED);
    DrawRectangle(casa.x, casa.y, casa.width, casa.height, ORANGE);
    DrawRectangle(cin.x, cin.y, cin.width, cin.height, DARKGRAY);
    DrawRectangle(onda.x, onda.y, onda.width, onda.height, BLUE);
    
    EndMode2D();
    EndDrawing();
}


    CloseWindow();
    
    
    
    
    return 0;
}