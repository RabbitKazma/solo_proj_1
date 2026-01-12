// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

AEGfxVertexList* pSquareMesh = 0;   // Standard Square Mesh for UI

// Player Properties 
f32 playerX = 0.0f;
f32 playerY = 0.0f;
f32 playerRadiusX = 25.0f;       // Radius for collision
f32 playerRadiusY = 25.0f;
f32 playerHealth = 100.0f;
f32 const MAX_HEALTH = 100.0f;
f32 const PlAYER_SPEED = 200.0f;

//Variables for Zone
f32 const zoneRadiusX = 300.0f;
f32 const zoneRadiusY = 300.0f;
f32 const zoneLeftX = -300.0f;
f32 const zoneLeftY = 0.0f;
f32 const zoneRightX = 300.0f;
f32 const zoneRightY = 0.0f;


f32 const healthbarscaleX = 1000.0f;
f32 const healthbarscaleY = 50.0f;
f32 const healthbarposX = 0.0f;
f32 const healthbarposY = 300.0f;

f32 const healthiconscaleX = 50.0f;
f32 const healthiconscaleY = 50.0f;
f32 const healthiconposX = -500.0f;
f32 const healthiconposY = 200.0f;

f32 healthDecrease = 0.0f;


// Standard Square Mesh for UI
void CreateSquareMesh()
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);
	pSquareMesh = AEGfxMeshEnd();
}


// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Solo Project");

	CreateSquareMesh();

	// reset the system modules
	AESysReset();

	printf("Hello World\n");

	// Variables for Matrix 
	AEMtx33 scale{0}, trans{0}, transform{0};

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// Your own update logic goes here
		float deltaTime = AEFrameRateControllerGetFrameTime();

		if (AEInputCheckCurr(AEVK_W)) {
			printf("W key pressed");
			playerY += deltaTime * PlAYER_SPEED;
		}

		if (AEInputCheckCurr(AEVK_S)) {
			printf("S key pressed");
			playerY -= deltaTime * PlAYER_SPEED;
		}

		if (AEInputCheckCurr(AEVK_A)) {
			printf("A key pressed");
			playerX -= deltaTime * PlAYER_SPEED;
		}

		if (AEInputCheckCurr(AEVK_D)) {
			printf("D key pressed");
			playerX += deltaTime * PlAYER_SPEED;
		}

		f32 diff_X = playerRadiusX - zoneRightX;
		f32	diff_Y = playerRadiusY - zoneRightY;

		//a square + b square <= square
		if ((diff_X && diff_Y) <= 0 ) 
		{
			printf("test");
			healthDecrease = 30.0f;
		}



		// Your own rendering logic goes here
		AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// 1. Draw Left Zone (Red)
		AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f); // Red Tint
		AEMtx33Scale(&scale, zoneRadiusX, zoneRadiusY);
		AEMtx33Trans(&trans, zoneLeftX, zoneLeftY);
		AEMtx33Concat(&transform, &trans, &scale); // Order: Translate * Scale

		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

		// 2. Draw Right Zone (Green)
		AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 1.0f); // Green Tint
		AEMtx33Scale(&scale, zoneRadiusX, zoneRadiusY);
		AEMtx33Trans(&trans, zoneRightX, zoneRightY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

		// 3. Draw Player (Blue)
		AEGfxSetColorToMultiply(0.0f, 0.5f, 1.0f, 1.0f); // Blue Tint
		AEMtx33Scale(&scale, playerRadiusX, playerRadiusY);
		AEMtx33Trans(&trans, playerX, playerY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);


		// --- UI Rendering ---
		AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
		AEMtx33Scale(&scale, healthbarscaleX-healthDecrease, healthbarscaleY);
		AEMtx33Trans(&trans, healthbarposX, healthbarposY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);



		for (int i = 0; i <= 10; ++i) {

			AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
			AEMtx33Scale(&scale, healthiconscaleX, healthiconscaleY);
			AEMtx33Trans(&trans, healthiconposX + (i * 100.0f), healthiconposY);
			AEMtx33Concat(&transform, &trans, &scale);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);
		}



		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	if(pSquareMesh) AEGfxMeshFree(pSquareMesh);
	// free the system
	AESysExit();
}