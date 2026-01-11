// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

AEGfxVertexList* pSquareMesh = 0;   // Standard Square Mesh for UI

// Player Properties 
float playerX = 0.0f;
float playerY = 0.0f;
float playerRadius = 25.0f;       // Radius for collision
float playerHealth = 100.0f;
float const MAX_HEALTH = 100.0f;
float const PlAYER_SPEED = 200.0f;

//Variables for Zone
const float zoneRadius = 300.0f;
const float zoneLeftX = -300.0f;
const float zoneLeftY = 0.0f;
const float zoneRightX = 300.0f;
const float zoneRightY = 0.0f;

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
	AEMtx33 scale{}, trans{}, transform{};

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
		float deltaTime = (float)AEFrameRateControllerGetFrameTime();


		// Your own rendering logic goes here
		AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// 1. Draw Left Zone (Red)
		AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 0.5f); // Red Tint
		AEMtx33Scale(&scale, zoneRadius, zoneRadius);
		AEMtx33Trans(&trans, zoneLeftX, zoneLeftY);
		AEMtx33Concat(&transform, &trans, &scale); // Order: Translate * Scale
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

		// 2. Draw Right Zone (Green)
		AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 0.5f); // Green Tint
		AEMtx33Scale(&scale, zoneRadius, zoneRadius);
		AEMtx33Trans(&trans, zoneRightX, zoneRightY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

		// 3. Draw Player (Blue)
		AEGfxSetColorToMultiply(0.0f, 0.5f, 1.0f, 1.0f); // Blue Tint
		AEMtx33Scale(&scale, playerRadius, playerRadius);
		AEMtx33Trans(&trans, playerX, playerY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);


		// --- UI Rendering ---

		float uiBarX = 0.0f;
		float uiBarY = 350.0f;
		float uiFullWidth = 400.0f;
		float uiHeight = 30.0f;

		// 4. Draw Health Bar Background (Grey)
		// This remains centered because it is the static container
		AEGfxSetColorToMultiply(0.3f, 0.3f, 0.3f, 1.0f);
		AEMtx33Scale(&scale, uiFullWidth, uiHeight);
		AEMtx33Trans(&trans, uiBarX, uiBarY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

		// 5. Draw Active Health Bar (Red) - LEFT ALIGNED SCALING
		float currentBarWidth = (playerHealth / MAX_HEALTH) * uiFullWidth;

		// [Step 1] Find the fixed left edge of the full bar
		float leftEdge = uiBarX - (uiFullWidth * 0.5f);

		// [Step 2] Calculate the new center point based on the current width
		// As width shrinks, the center moves left, keeping the left edge pinned.
		float newCenterX = leftEdge + (currentBarWidth * 0.5f);

		AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
		AEMtx33Scale(&scale, currentBarWidth, uiHeight);
		AEMtx33Trans(&trans, newCenterX, uiBarY); // Use newCenterX here
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

		// 6. Draw Health Icons (Rectangles)
		int numIcons = (int)((playerHealth / MAX_HEALTH) * 10.0f);
		float iconStartY = uiBarY - 40.0f;

		// Align icons to start exactly where the bar starts
		float iconStartX = leftEdge + 15.0f;
		float iconSpacing = 40.0f;

		for (int i = 0; i < numIcons; ++i)
		{
			float iconX = iconStartX + (i * iconSpacing);

			AEGfxSetColorToMultiply(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
			AEMtx33Scale(&scale, 30.0f, 10.0f);
			AEMtx33Trans(&trans, iconX, iconStartY);
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