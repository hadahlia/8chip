#include "raylib.h"
#include "chip.hpp"


#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

int main(int argc, char** argv)
{
	//int cycletime = 16667;
	char const* romName = argv[1];

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "emul8");
	SetWindowMinSize(64, 32);


	SetTargetFPS(60);

	Chip chip;
	chip.LoadROM(romName);
	

	for (Color i : chip.video) {
		i = colors[0];
		//chip.video[i] = colors[0];
	}
	
	
	//chip.TestDraw();

	
	Image pixelImage = GenImageColor(VIDEO_WIDTH, VIDEO_HEIGHT, colors[0]);
	ImageFormat(&pixelImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	//pixelImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	//pixelImage.data = chip.video;
	//pixelImage.width = VIDEO_WIDTH;
	//pixelImage.height = VIDEO_HEIGHT;
	//pixelImage.mipmaps = 1;

	Texture2D pixelTex = LoadTextureFromImage(pixelImage);
	UnloadImage(pixelImage);

	SetTextureFilter(pixelTex, TEXTURE_FILTER_POINT);

	auto prevTime = std::chrono::high_resolution_clock::now();

    while (!WindowShouldClose())
    {

		auto curTime = std::chrono::high_resolution_clock::now();
		float delta = std::chrono::duration<float, std::chrono::milliseconds::period>(curTime - prevTime).count();

		float scale = MIN((float)GetScreenWidth()/VIDEO_WIDTH, (float)GetScreenHeight()/VIDEO_HEIGHT);
		

		if (delta > (16.67f * 10.f)) {
			prevTime = curTime;
			
			chip.Cycle();
			//std::cout << "update texture!" << std::endl;
			UpdateTexture(pixelTex, chip.video);
			//std::cout << "60hz" << std::endl;
		}

		//std::cout << "delta : " << delta << std::endl;
		
		BeginDrawing();
				
			ClearBackground(BLACK);
			
			DrawTexturePro(pixelTex, (Rectangle){0.0f, 0.0f, (float)pixelTex.width, (float)-pixelTex.height},
								(Rectangle){((float)GetScreenWidth() - ((float)VIDEO_WIDTH*scale))*0.5, ((float)GetScreenHeight() - ((float)VIDEO_HEIGHT*scale))*0.5,
								(float)VIDEO_WIDTH*scale, (float)VIDEO_HEIGHT*scale}, (Vector2){0, 0}, 0.0f, WHITE);
			
     	EndDrawing();

    }
	UnloadTexture(pixelTex);
	//UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
