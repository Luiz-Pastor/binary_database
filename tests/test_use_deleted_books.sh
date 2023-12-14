#!/usr/bin/expect -f
# Test to check if the memory is reused.
# For example, if there is a deleted block of 28 bytes and you want to add a new block of 20 bytes,
# 20 bytes of the deleted block will be used, and the 8 will be kept for new blocks

set	timeout -1
set	filename "test"
set	programName "library"

# Delete the files involved to avoid problems
spawn	rm -f $filename.db $filename.ind

# Execute the program
spawn	../$programName first_fit $filename
expect	"Type command and argument/s."
expect	"exit"

# We added 3 books
send	"add 12345|978-2-12345680-5|El Quijote|Catedra\r"
expect	"Record with BookID=12345 has been added to the database"
expect	"exit"

send	"add 12346|978-2-12345680-6|A very large book title, very large|Any1 that want to write_\r"
expect	"Record with BookID=12346 has been added to the database"
expect	"exit"

send	"add 12348|978-2-12345680-8|el quijote|catedra\r"
expect	"Record with BookID=12348 has been added to the database"
expect	"exit"

# We make sure the information is well stored (information and indexes).
send	"printRec\n"
expect	"12345|978-2-12345680-5|El Quijote|Catedra"
expect	"12346|978-2-12345680-6|A very large book title, very large|Any1 that want to write_"
expect	"12348|978-2-12345680-8|el quijote|catedra"
expect	"exit"

send	"printInd\n"
expect	"Entry #0"
expect	"    key: #12345"
expect	"    offset: #0"
expect	"    size: #38"
expect	"Entry #1"
expect	"    key: #12346"
expect	"    offset: #46"
expect	"    size: #80"
expect	"Entry #2"
expect	"    key: #12348"
expect	"    offset: #134"
expect	"    size: #38"
expect	"exit"

# Also we check that there is no deleted element
send	"printLst\n"
expect	"exit"

# Now we deleted the big block, and check that the indexes dont change
send	"del 12346\n"
expect	"Record with BookID=12346 has been deleted"
expect	"exit"

send	"printInd\n"
expect	"Entry #0"
expect	"    key: #12345"
expect	"    offset: #0"
expect	"    size: #38"
expect	"Entry #1"
expect	"    key: #12348"
expect	"    offset: #134"
expect	"    size: #38"
expect	"exit"

send	"printLst\n"
expect	"Entry #0"
expect	"    offset: #46"
expect	"    size: #80"
expect	"exit"

# Vamos a añadir un bloque de 36 y ver que el bloque eliminado se actualiza correctamente
send	"add 12347|978-2-12345680-7|El Quijote|Catedra\r"
expect	"Record with BookID=12347 has been added to the database"
expect	"exit"

send	"printInd\n"
expect	"Entry #0"
expect	"    key: #12345"
expect	"    offset: #0"
expect	"    size: #38"
expect	"Entry #1"
expect	"    key: #12347"
expect	"    offset: #46"
expect	"    size: #38"
expect	"Entry #2"
expect	"    key: #12348"
expect	"    offset: #134"
expect	"    size: #38"
expect	"exit"

send	"printLst\n"
expect	"Entry #0"
expect	"    offset: #92"
expect	"    size: #42"
expect	"exit"

# Añadimos otro para ver si entra en el espacio sobrante (debería poder entrar)
send	"add 12346|978-2-12345680-6|Others|GonJLuP\n"
expect	"Record with BookID=12346 has been added to the database"
expect	"exit"

send	"printInd\n"
expect	"Entry #0"
expect	"    key: #12345"
expect	"    offset: #0"
expect	"    size: #38"
expect	"Entry #1"
expect	"    key: #12346"
expect	"    offset: #92"
expect	"    size: #34"
expect	"Entry #2"
expect	"    key: #12347"
expect	"    offset: #46"
expect	"    size: #38"
expect	"Entry #3"
expect	"    key: #12348"
expect	"    offset: #134"
expect	"    size: #38"
expect	"exit"

send	"printLst\n"
expect	"Entry #0"
expect	"    offset: #134"
expect	"    size: #8"
expect	"exit"

# Finally, we are going to add a block larger than the space of the deleted block, so
# the already deleted block should not be modified.
send	"add 12349|978-2-12345680-8|Last Book|2C\r"
expect	"Record with BookID=12349 has been added to the database"
expect	"exit"

send	"printInd\n"
expect	"Entry #0"
expect	"    key: #12345"
expect	"    offset: #0"
expect	"    size: #38"
expect	"Entry #1"
expect	"    key: #12346"
expect	"    offset: #92"
expect	"    size: #34"
expect	"Entry #2"
expect	"    key: #12347"
expect	"    offset: #46"
expect	"    size: #38"
expect	"Entry #3"
expect	"    key: #12348"
expect	"    offset: #134"
expect	"    size: #38"
expect	"Entry #4"
expect	"    key: #12349"
expect	"    offset: #180"
expect	"    size: #32"
expect	"exit"

send	"printLst\n"
expect	"Entry #0"
expect	"    offset: #134"
expect	"    size: #8"
expect	"exit"

# Si el programa ha llegado aqui según lo esperado, se realiza bien el reuso de memoria
send	"exit\n"
expect	"exit"



set green "\033\[1;32m"
set red "\033\[1;31m"

puts	"\n\t${green}#1. The program finishes execution correctly"

if {[file exists [file join $filename.db]]} {
    puts "\t${green}#2. File $filename.db exists"
} else {
    puts "\t${red}#2. File $filename.db NOT found"
}

set output "differ"
try {
set output [exec diff -s $filename.db test_control_use_deleted_books.db]
} trap CHILDSTATUS {} {}
if {[regexp -nocase "identical" $output] || [regexp -nocase "idénticos" $output]} {
    puts "\t${green}#3. The information is saved correctly"
} else {
    puts "\t${red}#3. Information is saved incorrectly"
}