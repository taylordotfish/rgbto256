    .global approxdb_compressed
    .global approxdb_compressed_size
    .section .rodata
approxdb_compressed:
    .incbin "approxdb/compressed-db.bin"
1:
    .align 8
approxdb_compressed_size:
    .quad 1b - approxdb_compressed
