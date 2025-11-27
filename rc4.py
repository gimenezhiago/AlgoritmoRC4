import sys

def ksa(key):
    key_length = len(key)
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % key_length]) % 256
        S[i], S[j] = S[j], S[i]
    return S

def prga(S, data):
    i = 0
    j = 0
    output = []
    for byte in data:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        t = (S[i] + S[j]) % 256
        K = S[t]
        output.append(byte ^ K)
    return output

def rc4(key, data):
    S = ksa(key)
    return prga(S, data)

def parse_input(text):
    text = text.strip()

    # Detectar formato \xAA\xBB
    if text.startswith("\\x"):
        try:
            parts = text.split("\\x")[1:]
            return bytes(int(p, 16) for p in parts), True
        except:
            pass

    # Detectar formato "AA BB CC"
    parts = text.split()
    if all(len(p) == 2 and all(c in "0123456789ABCDEFabcdef" for c in p) for p in parts):
        try:
            return bytes(int(p, 16) for p in parts), True
        except:
            pass

    # Caso contrário, é texto normal
    return text.encode(), False

def try_decode(data):
    try:
        return data.decode("utf-8")
    except:
        return None

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Uso: python {sys.argv[0]} <chave>")
        sys.exit(1)

    key = sys.argv[1].encode()

    text = input("Digite o texto para cifrar/decifrar: ")
    data, is_hex_input = parse_input(text)

    output = bytes(rc4(key, data))

    # Só mostra "Texto decifrado" se a entrada for HEX
    if is_hex_input:
        decoded = try_decode(output)
        if decoded is not None:
            print("\nTexto decifrado:", decoded)

    print("\nResultado (HEX): ", end="")
    for b in output:
        print(f"{b:02X} ", end="")

    print("\nResultado (bytes): ", end="")
    for b in output:
        print(f"\\x{b:02X}", end="")

    print()
