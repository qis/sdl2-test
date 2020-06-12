int main(int argc, char* argv[]) {
  try {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      throw std::runtime_error(SDL_GetError());
    }
  }
  catch (const std::exception& e) {
    std::fputs(e.what(), stderr);
    std::fputc('\n', stderr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
