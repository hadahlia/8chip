#include "chip.hpp"
#include "raylib.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

int main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "um");
	SetWindowMinSize(64, 32);

	// const unsigned int gameWidth {64};
	// const unsigned int gameHeight {32};
	

	//Color* pixColorArray = new Color[gameWidth * gameHeight]{ 0 };

	//pixColorArray = &colors[0];


	SetTargetFPS(60);

	Chip chip;


	for (unsigned int i = 0; i < sizeof(chip.video); ++i) {
		chip.video[i] = colors[0];
	}

	chip.TestDraw();

	
	//pixColorArray = chip.video;
	//video_sprite_to_color(pixColorArray, chip.video);
	
	Image pixelImage = GenImageColor(VIDEO_WIDTH, VIDEO_HEIGHT, colors[0]);
	ImageFormat(&pixelImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	//pixelImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	//pixelImage.data = chip.video;
	//pixelImage.width = VIDEO_WIDTH;
	//pixelImage.height = VIDEO_HEIGHT;
	//pixelImage.mipmaps = 1;

	Texture2D pixelTex = LoadTextureFromImage(pixelImage);
	UnloadImage(pixelImage);

	//RenderTexture2D target = LoadRenderTexture(gameWidth, gameHeight);
	//target.texture = pixelTex;
	//RenderTexture2D target = LoadRenderTexture(gameWidth, gameHeight);
	//RenderTexture2D blip = LoadRenderTexture(1,1);
	SetTextureFilter(pixelTex, TEXTURE_FILTER_POINT);

    while (!WindowShouldClose())
    {
		float scale = MIN((float)GetScreenWidth()/VIDEO_WIDTH, (float)GetScreenHeight()/VIDEO_HEIGHT);

		//BeginTextureMode(target);
		
			//ClearBackground(colors[0]);
			//LoadImag
			//target.texture.format
			
			// for (uint32_t i = 0; i < gameWidth*gameHeight; i++) {
			// 	DrawRectangle(0x01, 0x10, 1, 1, colors[1]);
			// }

		//EndTextureMode();
		//chip.TestDraw();

		

		

        BeginDrawing();
			
			ClearBackground(BLACK);
			
			//target.texture.
			DrawTexturePro(pixelTex, (Rectangle){0.0f, 0.0f, (float)pixelTex.width, (float)-pixelTex.height},
								(Rectangle){(GetScreenWidth() - ((float)VIDEO_WIDTH*scale))*0.5, (GetScreenHeight() - ((float)VIDEO_HEIGHT*scale))*0.5,
								(float)VIDEO_WIDTH*scale, (float)VIDEO_HEIGHT*scale}, (Vector2){0, 0}, 0.0f, WHITE);
			
			UpdateTexture(pixelTex, chip.video);
            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
	UnloadTexture(pixelTex);
	//UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}

// void video_sprite_to_color(Color* array, uint32_t video[]) {
// 	for (unsigned int i = 0; i < sizeof(video); ++i) {

// 		//array[i] 
// 	}
	
// }