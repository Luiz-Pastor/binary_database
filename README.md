# Binary database
## Información sobre el proyecto
El objetivo del proyecto es crear un programa capaz de funcionar como si fuera un gesto de bases de datos, pudiendo almacenar en diferentes ficheros información sobre los elementos que se menten en esta bd.
En este caso, nuestro gesto funcionara insertando libros en una biblioteca. Estos libros tienen 4 campos:
- `bookID/key` (número entero) [clave primaria].
- `ISBN` (16 carácteres).
- `title` (texto variable).
- `printedBy` (texto variable).

Además, cada libro contiene, de forma interna, dos campos:
- `size`: tamaño en bytes del bloque de memoria correspondiente.
- `offset`: posición en memoria en la que se encuentra el libro en la base de datos.

Una vez finalizado el programa, se guardan en 3 archivos diferentes la información de la ejecución:
- `"file".db`: información sobre los registros/elementos/libros de la base de datos
- `"file".ind`: información sobre los índices de los registros de la base de datos
- `"file".lst`: información sobre los elementos eliminados durante la ejecución del programa

Ante la carga de la base de datos, se tienen en cuenta los archivos con extensiones .db y .ind, para colocar los elementos en su posición correspondiente, pero no se usa en ningún momento los elementos eliminados de otras ejecuciones

## Estructuración
```
/root
 |-- /inc (localización de los archivos de cabecera)
 |-- /src (localización de los archivos fuente)
 |-- /tests (localización de los tests automáticos)
 |
 |- Makefile
 |- README.md
```

## Compilación
Para poder compilar el programa basta con ejecutar el comando:
```bash
make
```
En caso de no tener dicho comando instalado, instalar el recurso a través de uno de los siguientes comandos:
- Linux
```bash
sudo apt-get install make
```
- MaxOS (brew)
```bash
brew install make
```


Una vez compilado, se crearán una carpeta "obj" con los objetos (.o) creados al compilar. El makefile está preparado para no poder relink.

## Forma de usar
Para poder ejecutar el programa, es necesario introducir 2 parámetros:
- Forma de inserción de los elementos en la base de datos. Existen 3 posibilidades
	- first_fit: inserta los elementos en el primer espacio vacío en el que quepa
	- best_fit: inserta los elementos en el espacio más pequeño que encuentre
	- worst_fit: inserta los elementos en el espacio más grande que encuentre
- Nombre del archivo (sin extensión) del que se va a leer la base de datos.
```
~ ./library "mode" "file"
```

Ejemplo:
```bash
~ ./library first_fit test
```

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