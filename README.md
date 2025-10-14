## Mini-Shell (SO-Proyecto)

Pequeña shell en C++ que ejecuta comandos en modo foreground, con soporte para redirección, pipes, comandos internos y (opcional) navegación por historial con flechas usando GNU Readline.

### Requerimientos
- Linux
- Compilador C++17 (g++)
- Opcional para flechas: `libreadline-dev` (Ubuntu/Debian)

### Compilación y ejecución
- Modo básico (sin flechas):
  ```bash
  ./build.sh
  ./src/mi_shell
  ```
- Modo con flechas (Readline):
  ```bash
  ./build.sh readline
  ./src/mi_shell
  ```
  Si falta la dependencia:
  ```bash
  sudo apt-get install -y libreadline-dev
  ```

### Características implementadas

Base:
- Prompt personalizado y lectura de comandos.
- Resolución de rutas: absoluta tal cual; si no, fallback a `/bin/<cmd>` con verificación de permisos.
- Ejecución con procesos: `fork()` en hijo + `execv()`, padre espera con `waitpid()`.
- Manejo de errores con `perror/errno` y reporte si termina por señal.
- Redirección de salida `>` (crea/trunca, sin imprimir en pantalla).
- Salida con `salir`/`exit`.

Extensiones (2+):
- Pipes simples: `cmd1 | cmd2`.
- Comandos internos (built-ins): `cd`, `pwd`, `help`, `history`, `alias`.
- Opcional: navegación de historial con flechas (Readline) en modo interactivo.

Pendientes/Parcial:
- Background `&`: lanza sin esperar, falta recolección diferida (SIGCHLD/WNOHANG) para evitar zombies.
- Redirección `<` y `>>` (parser listo, falta integrar en ejecución).
- Señales, hilos (parallel), meminfo: no implementados.

### Ejemplos de uso

Sesión básica:
```bash
mi_shell> pwd
mi_shell> alias ll='ls -la'
mi_shell> ll
mi_shell> history
mi_shell> salir
```

Pipes y redirección:
```bash
mi_shell> ls | grep cpp
mi_shell> echo hola > out.txt
```

Con flechas (Readline):
- Usa flecha arriba/abajo para navegar comandos ya escritos.
- Edición de línea (Ctrl+A/Ctrl+E) y borrado interactivo.

---

### Capturas

- Prompt personalizado
  - ![Prompt personalizado](docs/capturas/promtpersonalizado.png)
- Resolución de rutas
  - ![Resolución de rutas](docs/capturas/resolucionderutas.png)
- Ejecución mediante procesos
  - ![Ejecución mediante procesos](docs/capturas/ejecucionmedianteprocesos.png)

- Manejo de errores
  - ![Manejo de errores](docs/capturas/manejodeerrores.png)
- Redirección de salida estándar (>)
  - ![Redirección de salida estándar](docs/capturas/redireccion.png)

- Comando de salida
  - ![Comando de salida](docs/capturas/salida.png)
- Pipes
  - ![Pipes](docs/capturas/pipes.png)
- Comandos internos (built-ins)
  - ![Comandos internos (built-ins)](docs/capturas/comandosinternos.png)

---

### Notas de desarrollo (Gitflow breve)
- Trabajar en ramas `feature/*` y abrir PR hacia `develop`.
- Evitar commits directos en `main`/`develop`.
- Mantener feature actualizada con `git fetch` + `git rebase origin/develop` antes del PR.