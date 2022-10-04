#include <3ds.h>
#include <citro2d.h>
#include "Dialog.h"
#include "Grille.h"
#include "images.h"

using std::string;
using std::vector;

int main(int argc, char* argv[])
{
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/images.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);
	Grille grillePlayer(GrilleTypePlayer, spriteSheet);
	Grille grilleEnnemy(GrilleTypeEnnemy, spriteSheet);

	grilleEnnemy.marquerTouche((coord){5,5});
	grilleEnnemy.marquerTouche((coord){6,5});

	grilleEnnemy.marquerRate((coord){1,3});
	grilleEnnemy.marquerRate((coord){7,4});
	grilleEnnemy.marquerRate((coord){3,6});

	touchPosition touch;
	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		hidTouchRead(&touch);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(top);
		grillePlayer.afficherGrille();
		C2D_SceneBegin(bottom);
		grilleEnnemy.afficherGrille();

		C3D_FrameEnd(0);

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}

	C2D_SpriteSheetFree(spriteSheet);
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
