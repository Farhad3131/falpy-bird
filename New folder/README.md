# Flappy Bird — C++ / WebAssembly

A small Flappy Bird-style game written in C++ with SDL2 and compiled to WebAssembly for browser play.

## Publish a playable link with GitHub Pages

1. Create a new **public** repository on GitHub (for example `flappy-bird`).
2. Upload the contents of this folder to the repository root. Keep `.github/workflows/pages.yml`.
3. Open **Settings → Pages**. Under **Build and deployment**, choose **GitHub Actions** as the source.
4. Open the **Actions** tab and wait for “Build and deploy Flappy Bird” to finish successfully.
5. Your shareable game URL will be:
   `https://YOUR-USERNAME.github.io/flappy-bird/`

Replace `YOUR-USERNAME` with your GitHub username and `flappy-bird` with the repository name.

## Controls
- Space / Up Arrow / mouse click / touch: flap
- After Game Over, tap or press Space to restart.

## Build locally (optional)
Install Emscripten, then run:
```bash
em++ main.cpp -o index.html -std=c++17 -O2 -sUSE_SDL=2 -sALLOW_MEMORY_GROWTH=1 -sEXIT_RUNTIME=1 --shell-file shell.html
```

This is an original small game inspired by the one-button pipe-dodging genre, not an official Flappy Bird release.
