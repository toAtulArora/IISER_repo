import random

source=[];
batchsize=[100,100,100,80,100]
for j in range(8,13):
    for i in range(1,batchsize[j-8]+1):
        source.append('MS' + (str(j).zfill(2)) + (str(i)).zfill(3))
    #print 'printing this for fun', s

#source.remove('MS1')

#print source

#pool = set(["foo", "bar", "baz", "123", "456", "789"]) # your 240 elements here
pool = set(source)
slen = len(pool) / 3 # we need 3 subsets
set1 = set(random.sample(pool, slen)) # 1st random subset
pool -= set1
set2 = set(random.sample(pool, slen)) # 2nd random subset
pool -= set2
set3 = pool # 3rd random subset

print set1
print set2
print set3
