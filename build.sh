#!/usr/bin/env bash
set -euo pipefail

# Script de compilación simple para la mini-shell
# Uso:
#   ./build.sh           # Compila modo básico (sin flechas)
#   ./build.sh readline  # Compila con soporte GNU Readline (flechas)
#   ./build.sh clean     # Elimina binario

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$DIR/src"
OUT="$SRC_DIR/mi_shell"

CXX="${CXX:-g++}"
CXXFLAGS_DEFAULT=(-std=c++17 -Wall -Wextra -O2)
LDLIBS=()

MODE="basic"
if [[ "${1:-}" == "readline" ]]; then
  MODE="readline"
elif [[ "${1:-}" == "clean" ]]; then
  rm -f "$OUT"
  echo "Limpieza completa: $OUT"
  exit 0
elif [[ "${1:-}" == "help" || "${1:-}" == "-h" || "${1:-}" == "--help" ]]; then
  echo "Uso: $0 [readline|clean]"
  exit 0
fi

SRCS=(
  "$SRC_DIR/main.cpp"
  "$SRC_DIR/shell.cpp"
  "$SRC_DIR/line_reader.cpp"
  "$SRC_DIR/parser.cpp"
  "$SRC_DIR/executor.cpp"
  "$SRC_DIR/pipe_simple.cpp"
  "$SRC_DIR/redirection.cpp"
  "$SRC_DIR/builtins.cpp"
)

if [[ "$MODE" == "readline" ]]; then
  CXXFLAGS_DEFAULT+=( -DUSE_READLINE )
  LDLIBS+=( -lreadline -lncurses )
fi

echo "Compilando ($MODE)..."
"$CXX" "${CXXFLAGS_DEFAULT[@]}" -o "$OUT" "${SRCS[@]}" "${LDLIBS[@]}"
echo "OK -> $OUT"
#!/usr/bin/env bash
set -euo pipefail

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRCDIR="$DIR/src"
CXX="${CXX:-g++}"
CXXFLAGS_DEFAULT="-std=c++17 -Wall -Wextra -O2"
BIN="$SRCDIR/mi_shell"

# Fuentes
SRC=(
  main.cpp
  shell.cpp
  line_reader.cpp
  parser.cpp
  executor.cpp
  pipe_simple.cpp
  redirection.cpp
  builtins.cpp
)

READLINE=0
if [[ "${1:-}" == "readline" ]]; then
  READLINE=1
fi

CXXFLAGS="$CXXFLAGS_DEFAULT"
LDLIBS=""
if [[ $READLINE -eq 1 ]]; then
  CXXFLAGS+=" -DUSE_READLINE"
  LDLIBS+=" -lreadline -lncurses"
fi

echo "> Compilando ${READLINE:+(con Readline)}"
"$CXX" $CXXFLAGS -o "$BIN" "${SRC[@]/#/$SRCDIR/}" $LDLIBS
echo "> Listo: $BIN"
