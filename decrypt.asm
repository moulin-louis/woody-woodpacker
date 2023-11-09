section .text
    global decrypt ; export decrypt symbol
decrypt: ; void decrypt(void *key[rdi], size_t key_len[rsi], void *data[rdx], size_t data_len[rcx])
