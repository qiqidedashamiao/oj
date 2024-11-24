from typing import List
from collections import defaultdict
 
 
def sum_of_subsequences(nums: List[int], k: int) -> int:
    MOD = 1000000007
    f = defaultdict(int)
    cnt = defaultdict(int)
    for x in nums:
        # x - k后: f[x - k] + x * cnt[x - k]
        # x + k后: f[x + k] + x * cnt[x + k]
        # x单独: x * 1
        c = cnt[x - k] + cnt[x + k] + 1
        f[x] = (f[x] + f[x - k] + f[x + k] + x * c) % MOD
        cnt[x] = (cnt[x] + c) % MOD
    return sum(f.values()) % MOD
 
 
# n, k = list(map(int, input().split()))
# nums = list(map(int, input().split()))

nums = list()
#n,k,nums读文件test3.in获取
with open('test3.in','r') as f:
    n,k = list(map(int, f.readline().strip().split()))
    print(n,k)
    lines = f.readline().strip().split()
    print(len(lines))
    for i in range(n):
        nums.append(int(lines[i]))

print(sum_of_subsequences(nums, k))