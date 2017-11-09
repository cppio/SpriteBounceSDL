#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else // _WIN32
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#else // __APPLE__
#include <SDL2/SDL_image.h>
#endif // __APPLE_
#endif // _WIN32

static SDL_Texture *loadTexture(const char *file, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(file);
    if (!surface) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTextureFromSurface: %s", SDL_GetError());
        return nullptr;
    }
    return texture;
}

static const int sprite_w = 32, sprite_h = 32;
static const int window_w = 1280, window_h = 720;

static int scale = 1;

static double x = 0, y = 0, vx = 250, vy = 250;
static SDL_Texture *sprite;
static SDL_Renderer *renderer;

static void update(double delta) {
    x += vx * delta;
    if (x < 0) {
        vx = -vx;
        x = -x;
    } else if (x >= window_w - sprite_w) {
        vx = -vx;
        x = 2 * (window_w - sprite_w) - x;
    }

    y += vy * delta;
    if (y < 0) {
        vy = -vy;
        y = -y;
    } else if (y >= window_h - sprite_h) {
        vy = -vy;
        y = 2 * (window_h - sprite_h) - y;
    }
}

static void draw() {
    SDL_Rect position;
    position.x = static_cast<int>(x * scale);
    position.y = static_cast<int>(y * scale);
    position.w = sprite_w * scale;
    position.h = sprite_h * scale;
    SDL_RenderCopy(renderer, sprite, nullptr, &position);
}

static void loop() {
    static unsigned int last = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event))
        if (event.type == SDL_MOUSEBUTTONDOWN)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "mouse button %d pressed at (%d, %d)", event.button.button, event.button.x, event.button.y);
        else if (event.type == SDL_KEYDOWN)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "key %s pressed", SDL_GetKeyName(event.key.keysym.sym));

    unsigned int current = SDL_GetTicks();
    update((current - last) / 1000.0);
    last = current;

    SDL_RenderClear(renderer);
    draw();
    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv) {
    (void) argc; (void) argv;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

    SDL_Window   *window;
    if (SDL_CreateWindowAndRenderer(window_w, window_h, SDL_WINDOW_ALLOW_HIGHDPI, &window, &renderer)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindowAndRenderer: %s", SDL_GetError());
        return 1;
    }

    int new_w, new_h;
    if (SDL_GetRendererOutputSize(renderer, &new_w, &new_h)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetRendererOutputSize: %s", SDL_GetError());
        return 1;
    }
    if (new_w / window_w != new_h / window_h)
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "horizontal scale (%d) != vertical scale (%d)", new_w / window_w, new_h / window_h);
    scale = new_w / window_w;
    if (!scale) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "scale = 0");
        scale = 1;
    }

    sprite = loadTexture("Smiley.png", renderer);
    if (!sprite)
        return 1;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    while (!SDL_QuitRequested())
        loop();

    return 0;
}
