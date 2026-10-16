#pragma once

#include <SDL.h>
#include <algorithm>

// Source art contains three 6-pixel cells. Repeat individual cells so larger
// footprints are represented without clipping or stretching the original grid.
inline void drawStructureSizePreview(SDL_Renderer* target, SDL_Texture* lattice,
    SDL_Texture* concrete, int x, int y, int columns, int rows) {
    constexpr int step=6;
    const auto drawCells=[&](SDL_Texture* texture,int width,int height) {
        for(int column=0;column<width;++column) {
            for(int row=0;row<height;++row) {
                SDL_Rect source{0,0,step+1,step+1};
                SDL_Rect destination{x+column*step,y+row*step,step+1,step+1};
                SDL_RenderCopy(target,texture,&source,&destination);
            }
        }
    };
    drawCells(lattice,std::max(3,columns),std::max(3,rows));
    drawCells(concrete,columns,rows);
}
