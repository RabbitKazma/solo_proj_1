// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

AEGfxVertexList* pCircleMesh { 0 };
AEGfxVertexList* pSquareMesh { 0 };   // Standard Square Mesh for UI

// Player Properties 
f32 playerX = 0.0f;
f32 playerY = 0.0f;
f32 playerRadius = 25.0f;       // Radius for collision
f32 playerHealth = 100.0f;
f32 const MAX_HEALTH = 100.0f;
f32 const PlAYER_SPEED = 200.0f;

//Variables for Zone
f32 const zoneRadius = 200.0f;
f64 const zoneLeftX = -300.0f;
f64 const zoneLeftY = 0.0f;
f64 const zoneRightX = 300.0f;
f64 const zoneRightY = 0.0f;

f32 const uiBarX = 0.0f;
f32 const uiBarY = 350.0f;
f32 const uiFullWidth = 1000.0f;
f32 const uiHeight = 50.0f;


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

void CreateCircleMesh()
{
	AEGfxMeshStart();

	int steps = 32;
	float angleStep = (2.0f * 3.14159f) / (float)steps;

	for (int i = 0; i < steps; ++i)
	{
		float theta1 = i * angleStep;
		float theta2 = (i + 1) * angleStep;

		// Center (0,0), Point A, Point B - White Color (0xFFFFFFFF)
		AEGfxTriAdd(
			0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
			cosf(theta1), sinf(theta1), 0xFFFFFFFF, 0.0f, 0.0f,
			cosf(theta2), sinf(theta2), 0xFFFFFFFF, 0.0f, 0.0f
		);
	}

	pCircleMesh = AEGfxMeshEnd();
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
	CreateCircleMesh();

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
		double deltaTime = AEFrameRateControllerGetFrameTime();

		if (AEInputCheckCurr(AEVK_W)) {
			printf("W key pressed");
			playerY += f32(deltaTime) * PlAYER_SPEED;
		}

		if (AEInputCheckCurr(AEVK_S)) {
			printf("S key pressed");
			playerY -= f32(deltaTime) * PlAYER_SPEED;
		}

		if (AEInputCheckCurr(AEVK_A)) {
			printf("A key pressed");
			playerX -= f32(deltaTime) * PlAYER_SPEED;
		}

		if (AEInputCheckCurr(AEVK_D)) {
			printf("D key pressed");
			playerX += f32(deltaTime) * PlAYER_SPEED;
		}

		// 1. Check Left Zone (Damage)
		f64 distLeft = (playerX - zoneLeftX) * (playerX - zoneLeftX)  + (playerY - zoneLeftY)  * (playerY - zoneLeftY);
		if (distLeft < ((playerRadius + zoneRadius) * (playerRadius + zoneRadius)))
		{
			playerHealth += 20.0f * f32(deltaTime);
		}

		// 2. Check Right Zone (Heal)
		f64 distRight = (playerX - zoneRightX) * (playerX - zoneRightX) + (playerY - zoneRightY) * (playerY - zoneRightY);
		if (distRight < ((playerRadius + zoneRadius) * (playerRadius + zoneRadius)))
		{
			playerHealth -= 20.0f * f32(deltaTime);
		}

		// Clamp Health
		if (playerHealth < 0.0f) playerHealth = 0.0f;
		if (playerHealth > MAX_HEALTH) playerHealth = MAX_HEALTH;



		// Your own rendering logic goes here
		AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// 1. Draw Left Zone (Red)
		AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 1.0f); // Red Tint
		AEMtx33Scale(&scale, zoneRadius, zoneRadius);
		AEMtx33Trans(&trans, zoneLeftX, zoneLeftY);
		AEMtx33Concat(&transform, &trans, &scale); // Order: Translate * Scale

		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pCircleMesh, AE_GFX_MDM_TRIANGLES);

		// 2. Draw Right Zone (Green)
		AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f); // Green Tint
		AEMtx33Scale(&scale, zoneRadius, zoneRadius);
		AEMtx33Trans(&trans, zoneRightX, zoneRightY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pCircleMesh, AE_GFX_MDM_TRIANGLES);

		// 3. Draw Player (Blue)
		AEGfxSetColorToMultiply(0.0f, 0.5f, 1.0f, 1.0f); // Blue Tint
		AEMtx33Scale(&scale, playerRadius, playerRadius);
		AEMtx33Trans(&trans, playerX, playerY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pCircleMesh, AE_GFX_MDM_TRIANGLES);


		// --- UI Rendering ---


		// 4. Draw Health Bar Background (Grey)
		// This remains centered because it is the static container
		AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
		AEMtx33Scale(&scale, uiFullWidth, uiHeight);
		AEMtx33Trans(&trans, uiBarX, uiBarY);
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

		// 5. Draw Active Health Bar (Red) - LEFT ALIGNED SCALING
		f32 currentBarWidth = (playerHealth / MAX_HEALTH) * uiFullWidth;

		// [Step 1] Find the fixed left edge of the full bar
		float leftEdge = uiBarX - (uiFullWidth * 0.5f);

		// [Step 2] Calculate the new center point based on the current width
		// As width shrinks, the center moves left, keeping the left edge pinned.
		f32 newCenterX = leftEdge + (currentBarWidth * 0.5f);

		AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 1.0f);
		AEMtx33Scale(&scale, currentBarWidth, uiHeight);
		AEMtx33Trans(&trans, newCenterX, uiBarY); // Use newCenterX here
		AEMtx33Concat(&transform, &trans, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

		// 6. Draw Health Icons (Rectangles)
		int numIcons = (int)((playerHealth / MAX_HEALTH) * 10.0f);
		float iconStartY = uiBarY - 80.0f;

		// Align icons to start exactly where the bar starts
		float iconStartX = leftEdge + 200.0f;
		float iconSpacing = 70.0f;

		for (int i = 0; i < numIcons; ++i)
		{
			float iconX = iconStartX + (i * iconSpacing);

			AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f); // Yellow
			AEMtx33Scale(&scale, 50.0f, 30.0f);
			AEMtx33Trans(&trans, iconX, iconStartY);
			AEMtx33Concat(&transform, &trans, &scale);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);
		}



		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	if (pSquareMesh) AEGfxMeshFree(pSquareMesh);
	if (pCircleMesh) AEGfxMeshFree(pCircleMesh);
	// free the system
	AESysExit();
}