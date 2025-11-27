import sys

def ksa(key):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) & 0xFF
        S[i], S[j] = S[j], S[i]
    return S

def prga(S, data):
    i = 0
    j = 0
    output = bytearray()
    for byte in data:
        i = (i + 1) & 0xFF
        j = (j + S[i]) & 0xFF
        S[i], S[j] = S[j], S[i]
        t = (S[i] + S[j]) & 0xFF
        k = S[t]
        output.append(byte ^ k)
    return bytes(output)

def rc4(key, data):
    S = ksa(key)
    return prga(S, data)

def main():
    if len(sys.argv) < 2:
        print("Uso: python rc4.py <chave>")
        sys.exit(1)

    key = sys.argv[1].encode()

    text = input("Digite o texto para cifrar/decifrar: ").encode()

    out = rc4(key, text)

    print("\nResultado (HEX):", out.hex(" ").upper())
    print("Resultado (bytes):", "".join(f"\\x{b:02X}" for b in out))

if __name__ == "__main__":
    main()
