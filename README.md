
1. Clona el repositorio:

   ```bash
   git clone https://github.com/LARMD-2/SO-Proyecto.git
   ```



MANEJO DE GITFLOW:


🔹 1. Crear tu rama de feature
Siempre parte desde `develop` actualizado:
`bash(en el terminal)
git checkout develop
git pull origin develop
git checkout -b feature/nombre-feature
``
Ejemplos:

* `feature/login`
* `feature/reservas`
* `feature/perfil-usuario`

---

### 🔹 2. Subir tus cambios

Cuando termines avances en tu feature:

```bash
git add .
git commit -m "Feature: descripción breve de lo que hiciste" (si es correcion de algo poner "fix:descripción")
git push origin feature/nombre-feature
```

---

  3. Mantener tu feature actualizada (rebase)

Si `develop` tuvo cambios mientras trabajabas, actualiza tu rama con: (ojo dentro de tu misma rama)

```bash
git fetch origin
git rebase origin/develop
```

Esto reorganiza tus commits encima de los últimos cambios.
 Hazlo siempre **antes de abrir tu Pull Request** para evitar conflictos.

---

 🔹 4. Pull Request

Cuando termines tu feature:

1. Sube tu rama (`git push origin feature/nombre-feature`).
2. Ve a GitHub y crea un **Pull Request de `feature/nombre-feature` → `develop`**.
3. El administrador del repo revisará y hará el merge.

---

 **Importante:**

* Nunca trabajes directo en `main` o `develop`.
* Todo tu trabajo va en una rama `feature/`.
* El merge a `develop` lo hace el **admin del repo**.