from sys import stdin

seen = set()
for line in stdin:
    seen.add(line.strip())

print len(seen)
