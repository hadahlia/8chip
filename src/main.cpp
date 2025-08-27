#include "chip.hpp"
#include "raylib.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

int main(int argc, char* argv[])
{

	char const* romName = argv[0];

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "emul8");
	SetWindowMinSize(64, 32);


	SetTargetFPS(60);

	Chip chip;
	//chip.LoadROM(romName);
	

	for (unsigned int i = 0; i < sizeof(chip.video); ++i) {
		chip.video[i] = colors[0];
	}
	

	chip.TestDraw();

	
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
		
		if (delta > 16.67) {
			prevTime = curTime;

			//chip.Cycle();

			UpdateTexture(pixelTex, chip.video);
			//std::cout << "60hz" << std::endl;
		}
        
		BeginDrawing();
				
			ClearBackground(BLACK);
			
			DrawTexturePro(pixelTex, (Rectangle){0.0f, 0.0f, (float)pixelTex.width, (float)-pixelTex.height},
								(Rectangle){(GetScreenWidth() - ((float)VIDEO_WIDTH*scale))*0.5, (GetScreenHeight() - ((float)VIDEO_HEIGHT*scale))*0.5,
								(float)VIDEO_WIDTH*scale, (float)VIDEO_HEIGHT*scale}, (Vector2){0, 0}, 0.0f, WHITE);
			
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