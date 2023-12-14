# Práctica 3: base de datos binaria



## Compilación
Para poder compilar el programa basta con ejecutar el comando:
```bash
make
```
Se crearán una carpeta "obj" con los objetos (.o) creados al compilar. El makefile está preparado para no poder realizar relink.

## Comandos del makefile

```bash
# Reglas de compilación
~ make	all:	compilará el proyecto, generando un ejecutable llamado "library". Los objetos son creados en la carpeta 'obj'

~ make	re:		eliminará todos los objetos y ejecutables creados, y los volverá a compilar
```
```bash
# Reglas de limpiza
~ make	clean:	elimina todo los objetos '.o', incluyendo la caprte 'obj'

~ make	fclean:	eliminar todos los objetos y el ejecutable creado
```
```bash
# Reglas de ejecución (si el ejecutable no está creado, se compila)
~ make	first_fit:	ejecutará el programa con el modo "first_fit". El nombre del archivo con la base de datos es "test" por defecto, pero puede ser cambiado en el propio Makefile en la parte de las variables (parte superior, variable llamada "TEST_FILE")

~ make	worst_fit:	ejecutará el programa con el modo "worst_fit". El nombre del archivo con la base de datos es "test" por defecto, pero puede ser cambiado en el propio Makefile en la parte de las variables (parte superior, variable llamada "TEST_FILE")

~ make	best_fit:	ejecutará el programa con el modo "best_fit". El nombre del archivo con la base de datos es "test" por defecto, pero puede ser cambiado en el propio Makefile en la parte de las variables (parte superior, variable llamada "TEST_FILE")

~ make	valgrind:	ejecutará el programa con el depurador valgrind en el modo "first_fit"
```
```bash
# Reglas de tests
~ make	test:			ejecutará todos los tests proporcionados para la práctica

~ make	personal_test:	ejecuta el test creado por nosotros
```
NOTA: para poder correr los tests es necesario tener instalado `expect`. Para instalarlo puede usar el siguiente comando:
- Linux
```bash
sudo apt-get install expect
```
- MacOS (con `brew`)
```bash
brew install expect
```