mkdir "Builds"
em++ main.cpp ./raylib_web/lib/libraylib.web.a -o Builds/index.html -Os -Wall \
  -I./raylib_web/include \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  -s SINGLE_FILE \
  --shell-file shell.html