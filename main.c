#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Change these numbers to anything you want! The entire UI scales to fit them.
#define ROWS 18
#define COLS 65

char canvas[ROWS][COLS];
int totalPixelsDrawn = 0;
char lastAction[50] = "System Initialized";

// Requirement 3: Initialize canvas with background dots
void initializeCanvas() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            canvas[i][j] = '.';
        }
    }
    totalPixelsDrawn = 0;
    strcpy(lastAction, "Canvas Cleared");
}

// Calculate active drawing points
void recalculateStats() {
    totalPixelsDrawn = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (canvas[i][j] != '.') {
                totalPixelsDrawn++;
            }
        }
    }
}

// Function 4: Fully Dynamic, Self-Aligning Display Workspace
void displayCanvas() {
    system("cls"); 
    recalculateStats();

    // 1. Dynamic Header Formatting
    char statsBuffer[100];
    sprintf(statsBuffer, " [STUDIO] Pixels: %d | Status: %s", totalPixelsDrawn, lastAction);
    int statsLen = strlen(statsBuffer);

    // Ensure the main layout border is at least wide enough for the canvas or text frame
    int uiWidth = (COLS > 58) ? COLS : 58;

    // Print Top Header block
    printf("\n   +");
    for (int j = 0; j < uiWidth; j++) printf("=");
    printf("+\n");

    printf("   |%s", statsBuffer);
    for (int j = 0; j < (uiWidth - statsLen); j++) printf(" ");
    printf("|\n");

    printf("   +");
    for (int j = 0; j < uiWidth; j++) printf("=");
    printf("+\n");
    
    // 2. Dynamic Canvas Rendering with adjusted padding
    printf("   +");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");

    for (int i = 0; i < ROWS; i++) {
        printf("   |"); 
        for (int j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("| %02d\n", i); 
    }

    printf("   +");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");
    
    // 3. Dynamic Axis Helper Sub-labels
    printf("    ");
    for (int j = 0; j < COLS; j++) {
        if (j % 10 == 0) printf("%d", j / 10);
        else printf(" ");
    }
    printf("\n");
}

// Helper to keep drawing points inside bounds safely
void drawPixel(int r, int c, char ch) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
        canvas[r][c] = ch;
    }
}

// Feature: Dynamic File Storage Output
void saveToFile() {
    FILE *file = fopen("artwork.txt", "w");
    if (file == NULL) {
        strcpy(lastAction, "Error saving file!");
        return;
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(file, "%c", canvas[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    strcpy(lastAction, "Saved to 'artwork.txt'");
}

// Function 1: Core Shape Plotting Engines
void drawLine() {
    int r1, c1, r2, c2;
    char brush;
    printf("\n   [LINE] Brush: ");   scanf(" %c", &brush);
    printf("   Start (Row Col): "); scanf("%d %d", &r1, &c1);
    printf("   End (Row Col): ");   scanf("%d %d", &r2, &c2);

    if (r1 == r2) {
        int start = (c1 < c2) ? c1 : c2;
        int end = (c1 < c2) ? c2 : c1;
        for (int j = start; j <= end; j++) drawPixel(r1, j, brush);
    } 
    else if (c1 == c2) {
        int start = (r1 < r2) ? r1 : r2;
        int end = (r1 < r2) ? r2 : r1;
        for (int i = start; i <= end; i++) drawPixel(i, c1, brush);
    } 
    else {
        int steps = abs(r2 - r1) > abs(c2 - c1) ? abs(r2 - r1) : abs(c2 - c1);
        float rInc = (float)(r2 - r1) / steps;
        float cInc = (float)(c2 - c1) / steps;
        float r = r1, c = c1;
        for (int i = 0; i <= steps; i++) {
            drawPixel((int)round(r), (int)round(c), brush);
            r += rInc;
            c += cInc;
        }
    }
    strcpy(lastAction, "Line Plotted");
}

void drawRectangle() {
    int r, c, width, height, fill;
    char brush;
    printf("\n   [RECTANGLE] Brush: ");   scanf(" %c", &brush);
    printf("   Top-Left (Row Col): "); scanf("%d %d", &r, &c);
    printf("   Width & Height: ");     scanf("%d %d", &width, &height);
    printf("   Fill? (1=Yes, 0=No): "); scanf("%d", &fill);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (fill == 1) {
                drawPixel(r + i, c + j, brush);
            } else {
                if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                    drawPixel(r + i, c + j, brush);
                }
            }
        }
    }
    strcpy(lastAction, "Rectangle Plotted");
}

void drawTriangle() {
    int r, c, size;
    char brush;
    printf("\n   [TRIANGLE] Brush: ");  scanf(" %c", &brush);
    printf("   Apex (Row Col): ");   scanf("%d %d", &r, &c);
    printf("   Height/Size: ");      scanf("%d %d", &size);

    for (int i = 0; i < size; i++) {
        drawPixel(r + i, c - i, brush);
        drawPixel(r + i, c + i, brush);
        if (i == size - 1) {
            for (int j = c - i; j <= c + i; j++) {
                drawPixel(r + i, j, brush);
            }
        }
    }
    strcpy(lastAction, "Triangle Plotted");
}

void drawCircle() {
    int centerR, centerC, radius, fill;
    char brush;
    printf("\n   [CIRCLE] Brush: ");    scanf(" %c", &brush);
    printf("   Center (Row Col): ");  scanf("%d %d", &centerR, &centerC);
    printf("   Radius: ");            scanf("%d %d", &radius);
    printf("   Fill? (1=Yes, 0=No): "); scanf("%d", &fill);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            double distance = sqrt(pow(i - centerR, 2) + pow(j - centerC, 2));
            if (fill == 1) {
                if (distance <= radius) drawPixel(i, j, brush);
            } else {
                if (fabs(distance - radius) < 0.6) drawPixel(i, j, brush);
            }
        }
    }
    strcpy(lastAction, "Circle Plotted");
}

// Requirement 2: Modifying and erasing objects
void deleteArea() {
    int r1, c1, r2, c2;
    printf("\n   [ERASER] Top-Left (Row Col): ");  scanf("%d %d", &r1, &c1);
    printf("   [ERASER] Bottom-Right (Row Col): "); scanf("%d %d", &r2, &c2);

    for (int i = r1; i <= r2; i++) {
        for (int j = c1; j <= c2; j++) {
            drawPixel(i, j, '.');
        }
    }
    strcpy(lastAction, "Area Cleared");
}

void modifyPixel() {
    int r, c;
    char newChar;
    printf("\n   [MODIFY] Target Location (Row Col): "); scanf("%d %d", &r, &c);
    printf("   New Character: ");                      scanf(" %c", &newChar);
    drawPixel(r, c, newChar);
    strcpy(lastAction, "Pixel Patched");
}

int main() {
    int choice;
    initializeCanvas();

    // Dynamically size the lower tool menu wrapper box
    int menuWidth = (COLS > 58) ? COLS : 58;

    while (1) {
        displayCanvas();
        printf("\n   +"); for(int j=0; j<menuWidth; j++) printf("-"); printf("+\n");
        printf("   |              ENGINE WORKSPACE SELECTIONS               |\n");
        printf("   +"); for(int j=0; j<menuWidth; j++) printf("-"); printf("+\n");
        printf("   |  1. Line Tool             |  5. Precision Block Eraser |\n");
        printf("   |  2. Rectangle Tool (Fill) |  6. Direct Pixel Modifier  |\n");
        printf("   |  3. Triangle Tool         |  7. Reset Canvas State     |\n");
        printf("   |  4. Circle Tool (Fill)    |  8. Flush Canvas to File   |\n");
        printf("   |                           |  9. Close Workspace Safe   |\n");
        printf("   +"); for(int j=0; j<menuWidth; j++) printf("-"); printf("+\n");
        printf("   Command Line: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 9) break;

        switch (choice) {
            case 1: drawLine(); break;
            case 2: drawRectangle(); break;
            case 3: drawTriangle(); break;
            case 4: drawCircle(); break;
            case 5: deleteArea(); break;
            case 6: modifyPixel(); break;
            case 7: initializeCanvas(); break;
            case 8: saveToFile(); break;
            default:
                strcpy(lastAction, "Invalid Entry Command!");
        }
    }

    printf("\n   System teardown complete. Canvas closed safely.\n");
    return 0;
}