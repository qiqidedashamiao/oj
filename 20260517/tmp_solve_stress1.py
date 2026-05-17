from math import inf


def main():
    in_path = '20260517/yongli/stress1.in'
    out_path = '20260517/yongli/stress1.out'
    with open(in_path, 'r', encoding='utf-8') as f:
        data = list(map(int, f.read().split()))

    it = iter(data)
    n = next(it)
    m = next(it)
    q = next(it)
    P = next(it)
    critical = [next(it) for _ in range(q)]

    sw = [[0] * (n + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(i + 1, n + 1):
            sw[i][j] = next(it)

    phase = [0] * (n + 1)
    base = [0] * (n + 1)
    cover = [0] * (n + 1)

    for i in range(1, n + 1):
        ph = next(it)
        op = next(it)
        rk = next(it)
        c = next(it)
        phase[i] = ph
        base[i] = op + rk
        mask = 0
        for _ in range(c):
            x = next(it)
            mask |= 1 << (x - 1)
        cover[i] = mask

    full = (1 << m) - 1
    best = inf
    best_seq = None

    for s in range(1, 1 << n):
        cost = 0
        cov = 0
        prev = 0
        kp = [0] * q
        seq = []
        bad = False

        for i in range(n):
            if (s >> i) & 1:
                x = i + 1
                seq.append(x)
                cost += base[x]
                if prev:
                    cost += sw[prev][x]
                if cost >= best:
                    bad = True
                    break
                prev = x
                cov |= cover[x]
                cm = cover[x]
                for ci, cx in enumerate(critical):
                    if (cm >> (cx - 1)) & 1:
                        kp[ci] |= 1 << (phase[x] - 1)

        if bad or cov != full:
            continue

        ok = True
        for v in kp:
            if v.bit_count() < 2:
                ok = False
                break

        if ok and cost < best:
            best = cost
            best_seq = seq[:]

    # with open(out_path, 'w', encoding='utf-8') as f:
    #     f.write(str(best) + '\n')

    print(best)
    print(best_seq)


if __name__ == '__main__':
    main()
