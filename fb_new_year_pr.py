ans=False
def subset_sum(numbers1,numbers2,numbers3, target1,target2,target3, partial1=[],partial2=[],partial3=[],ind=[]):
    global ans
    s1=sum(partial1)
    s2=sum(partial2)
    s3=sum(partial3)

    # check if the partial sum is equals to target
    if s1 == target1 and s2==target2 and s3==target3:
        ans=True
        return
    if s1 >= target1 or s2>=target2 or s3>=target3:
        return  # if we reach the number why bother to continue

    for i in range(len(numbers1)):
        n1 = numbers1[i]
        n2 = numbers2[i]
        n3 = numbers3[i]
        remaining1 = numbers1[i+1:]
        remaining2 = numbers2[i+1:]
        remaining3 = numbers3[i+1:]
        if not ans:
            subset_sum(remaining1,remaining2,remaining3, target1,target2,target3, partial1 + [n1],partial2+[n2],partial3+[n3],ind+[i])
        else:
            return True

import sys
#f=open("input.txt",'r')
#f1=open("output.txt",'w')
#sys.stdin=f
#sys.stdout=f1
t=int(raw_input())
for test in xrange(t):
    target1,target2,target3=map(int,raw_input().split())
    n=int(raw_input())
    numbers1=[]
    numbers2=[]
    numbers3=[]
    s=[0,0,0]
    for i in xrange(n):
        n1,n2,n3=map(int,raw_input().split())
        if n1==target1 and n2==target2 and n3==target3:
            print "Case #"+str(test+1)+": yes"
        else:
            numbers1.append(n1)
            numbers2.append(n2)
            numbers3.append(n3)
            s[0]+=n1
            s[1]+=n2
            s[2]+=n3
    if len(numbers1)==n:
        if s[0]==target1 and s[1]==target2 and s[2]==target3:
            print "Case #"+str(test+1)+": yes"
        elif s[0]<target1 or s[1]<target2 or s[2]<target3:
            print "Case #"+str(test+1)+": no"
        else:
            ans=False
            if subset_sum(numbers1,numbers2,numbers3,target1,target2,target3)==True:
                print "Case #"+str(test+1)+": yes"
            else:
                print "Case #"+str(test+1)+": no"
#f.close()
#f1.close()