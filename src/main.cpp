#include <version.h>

class Application {
public:
  Application(HINSTANCE instance) : instance_(instance) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      throw std::runtime_error(SDL_GetError());
    }
  }

  Application(Application&& other) = delete;
  Application(const Application& other) = delete;

  Application& operator=(Application&& other) = delete;
  Application& operator=(const Application& other) = delete;

  ~Application() {
    if (renderer_) {
      SDL_DestroyRenderer(renderer_);
      renderer_ = nullptr;
    }
    if (window_) {
      SDL_DestroyWindow(window_);
      window_ = nullptr;
    }
    SDL_Quit();
  }

  void Create(std::string title, int cx, int cy, Uint32 flags = 0) {
    if (SDL_CreateWindowAndRenderer(cx, cy, flags, &window_, &renderer_) < 0) {
      throw std::runtime_error(SDL_GetError());
    }
    SDL_SetWindowTitle(window_, title.data());
    constexpr auto size = 32;
    if (const auto icon = static_cast<HICON>(LoadImage(instance_, MAKEINTRESOURCE(101), IMAGE_ICON, size, size, LR_SHARED))) {
      ICONINFO ici = {};
      if (GetIconInfo(icon, &ici)) {
        if (const auto dc = CreateCompatibleDC(nullptr)) {
          constexpr auto bc = 32;
          constexpr auto mr = Uint32(0x00ff0000);
          constexpr auto mg = Uint32(0x0000ff00);
          constexpr auto mb = Uint32(0x000000ff);
          constexpr auto ma = Uint32(0xff000000);
          if (const auto surface = SDL_CreateRGBSurface(0, size, size, bc, mr, mg, mb, ma)) {
            BITMAPINFO bmi = {};
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = size;
            bmi.bmiHeader.biHeight = -size;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = bc;
            bmi.bmiHeader.biCompression = BI_RGB;
            bmi.bmiHeader.biSizeImage = 0;
            SelectObject(dc, ici.hbmColor);
            GetDIBits(dc, ici.hbmColor, 0, size, surface->pixels, &bmi, DIB_RGB_COLORS);
            SDL_SetWindowIcon(window_, surface);
            SDL_FreeSurface(surface);
          }
          DeleteDC(dc);
        }
        DeleteObject(ici.hbmColor);
        DeleteObject(ici.hbmMask);
      }
      DestroyIcon(icon);
    }
  }

  template <typename RenderFunction>
  void Run(RenderFunction render) {
    SDL_Event event = {};
    while (true) {
      render(window_, renderer_);
      while (SDL_PollEvent(&event) > 0) {
        switch (event.type) {
        case SDL_QUIT:
          return;
        }
      }
    }
  }

private:
  HINSTANCE instance_ = nullptr;
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR cmd, int show) {
  try {
    Application application{ instance };
    application.Create(PROJECT_DESCRIPTION " " PROJECT_VERSION, 800, 600, SDL_WINDOW_RESIZABLE);
    application.Run([](SDL_Window* window, SDL_Renderer* renderer) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
      SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
      SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
      SDL_RenderPresent(renderer);
    });
  }
  catch (const std::exception& e) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
