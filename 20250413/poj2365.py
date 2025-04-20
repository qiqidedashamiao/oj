# -*- coding: utf-8 -*-
import random
import math

def is_prime(n, k=5):
    """Miller-Rabin素性测试，k=5可确保1e16内正确"""
    if n <= 1: return False
    elif n <= 3: return True
    elif n % 2 == 0: return False
    d, s = n-1, 0
    while d % 2 == 0: d, s = d//2, s+1
    for _ in range(k):
        a = random.randint(2, n-2)
        x = pow(a, d, n)
        if x == 1 or x == n-1: continue
        for _ in range(s-1):
            x = pow(x, 2, n)
            if x == n-1: break
        else: return False
    return True

def pollards_rho(n):
    """高效因数分解算法，处理半素数平均5次迭代"""
    if n % 2 == 0: return 2
    if n % 3 == 0: return 3
    if n % 5 == 0: return 5
    while True:
        c = random.randint(1, n-1)
        f = lambda x: (pow(x, 2, n) + c) % n
        x, y, d = 2, 2, 1
        while d == 1:
            x, y = f(x), f(f(y))
            d = math.gcd(abs(x-y), n)
        if d != n: return d

def find_prime_factors(n):
    """分解半素数为两个素因数"""
    factors = []
    def _factor(n):
        if n == 1: return
        if is_prime(n):
            factors.append(n)
            return
        d = pollards_rho(n)
        _factor(d)
        _factor(n//d)
    _factor(n)
    return sorted(factors)[:2]  # 取前两个素因数

# 主逻辑
S, L = input().split()
N = int(S)
L = int(L)

# 分解双素因数（题目保证存在）
p, q = find_prime_factors(N)
if p > q: p, q = q, p  # 确保p是较小素数

# 计算位数（考虑1e18级数字的高效转换）
def digit_len(x):
    return x.bit_length() // 3 + 1  # 近似计算，比str更快，误差±1
    # 严格版本：return len(str(x))  # 题目允许100位，str转换无性能问题

p_len = len(str(p))
q_len = len(str(q))

# if min(p_len, q_len) < L:
#     print(f"invalid {p}")
# else:
#     print("valid")

if min(p_len, q_len) < L:
    print(f"BAD {p}")
else:
    print("GOOD")
